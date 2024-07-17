#include "../../includes/core/EventManager.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include "../../includes/utils/Converter.hpp"
#include <exception>
#include <unistd.h>

#define NO_EVENTS 0xC0
#define FILE_TYPE_MASK 0xC0
#define KEEP_DESCRIPTOR 0x01

EventManager::EventManager(IPollfdManager &pollfd_manager,
                           IBufferManager &buffer_manager,
                           IConnectionManager &connection_manager,
                           IServer &server, IRequestHandler &request_handler,
                           ILogger &logger)
    : m_pollfd_manager(pollfd_manager), m_buffer_manager(buffer_manager),
      m_connection_manager(connection_manager), m_server(server),
      m_request_handler(request_handler), m_logger(logger)
{
}

EventManager::~EventManager() {}

void EventManager::handleEvents()
{
    m_logger.log(EXHAUSTIVE, "[EVENTMANAGER] Handling events");

    for (ssize_t pollfd_index = 0;
         pollfd_index <
         static_cast<ssize_t>(m_pollfd_manager.getPollfdQueueSize());
         pollfd_index++)
    {
        short events = m_pollfd_manager.getEvents(pollfd_index);

        if (events == NO_EVENTS)
            continue;

        short fileType = events & NO_EVENTS;
        if (fileType == SERVER_SOCKET)
        {
            m_handleServerSocketEvents(pollfd_index, events);
        }
        else if (fileType == CLIENT_SOCKET)
        {
            m_handleClientSocketEvents(pollfd_index, events);
        }
        else if (fileType == PIPE)
        {
            m_handlePipeEvents(pollfd_index, events);
        }
        else if (fileType == REGULAR_FILE)
        {
            m_handleRegularFileEvents(pollfd_index, events);
        }
    }
}

void EventManager::m_handleRegularFileEvents(ssize_t &pollfd_index,
                                             short events)
{
    m_logger.log(EXHAUSTIVE, "[EVENTMANAGER] Handling regular file events");

    // Check if file is ready for writing
    if (events & POLLOUT)
    {
        // if the file is a body file for the CGI process
        if (m_pollfd_manager.isBodyFile(pollfd_index))
        {
            // Get the body descriptor
            int body_descriptor = m_pollfd_manager.getDescriptor(pollfd_index);

            // Log the writing to the body file
            m_logger.log(VERBOSE, "[EVENTMANAGER] Writing to body file '" +
                                      Converter::toString(body_descriptor) +
                                      "'");

            // Write to the body file and check if done
            if (!m_flushBuffer(pollfd_index, KEEP_DESCRIPTOR))
            {
                m_logger.log(ERROR,
                             "[EVENTMANAGER] Finished writing to body file '" +
                                 Converter::toString(body_descriptor) + "'");

                // Let RequestHandler handle the CGI process
                Triplet_t info = m_request_handler.executeCgi(body_descriptor);

                // Get the info
                int cgi_pid = info.first;
                int cgi_output_pipe_read_end = info.second.first;

                // Log the dynamic serving
                m_logger.log(VERBOSE,
                             "[EVENTMANAGER] GGI process launched with id " +
                                 Converter::toString(cgi_pid) +
                                 " (CGI output pipe Read end: " +
                                 Converter::toString(cgi_output_pipe_read_end));

                // Add the CGI output pipe Read end to the poll set
                pollfd pollfd;
                pollfd.fd = cgi_output_pipe_read_end;
                pollfd.events = POLLIN;
                pollfd.revents = 0;
                m_pollfd_manager.addPipePollfd(pollfd);
            }
        }
        else
        {
            // simply flush the buffer
            m_flushBuffer(pollfd_index, KEEP_DESCRIPTOR);
        }
    }

    // Check for error on the file
    if (events & POLLNVAL)
    {
        // Get the file descriptor
        int file_descriptor = m_pollfd_manager.getDescriptor(pollfd_index);

        // Log the error
        m_logger.log(ERROR,
                     "Error on file: " + Converter::toString(file_descriptor));

        // Clear buffer, remove from polling and close file
        m_cleanUp(pollfd_index, file_descriptor);
    }
}

void EventManager::m_handleServerSocketEvents(ssize_t pollfd_index,
                                              short events)
{
    m_logger.log(EXHAUSTIVE, "[EVENTMANAGER] Handling server socket events");
    // Check for errors on server socket
    if (events & POLLERR)
        throw ServerSocketError();

    // Check for server socket closed
    if (events & POLLHUP)
        throw ServerSocketClosedError();

    // Check for invalid request on server socket
    if (events & POLLNVAL)
        throw ServerSocketInvalidError();

    // Check if server socket is ready to accept incoming connections
    if (events & POLLIN)
    {
        // Accept incoming connection
        int server_socket_descriptor =
            m_pollfd_manager.getDescriptor(pollfd_index);
        m_server.acceptConnection(server_socket_descriptor);
    }
}

void EventManager::m_handleClientSocketEvents(ssize_t &pollfd_index,
                                              short events)
{
    m_logger.log(EXHAUSTIVE, "[EVENTMANAGER] Handling client socket events");
    // Check for exceptions
    if (events & (POLLHUP | POLLERR | POLLNVAL))
    {
        m_handleClientException(pollfd_index, events);
    }

    // Read and process a new request if ready
    else if (events & POLLIN)
    {
        m_handleRequest(pollfd_index);
    }

    // Send response
    else if (events & POLLOUT)
    {
        m_flushBuffer(pollfd_index);
    }
}

void EventManager::m_handleRequest(ssize_t &pollfd_index)
{
    int client_socket_descriptor = m_pollfd_manager.getDescriptor(pollfd_index);

    Triplet_t info = m_request_handler.handleRequest(client_socket_descriptor);

    if (info.first == -1) // served static files or bad request
    {
        // Log the static serving
        m_logger.log(VERBOSE,
                     "[EVENTMANAGER] Statically served client socket: " +
                         Converter::toString(client_socket_descriptor));

        // Add the POLLOUT event for the socket
        m_pollfd_manager.addPollOut(pollfd_index);
    }
    else if (info.first == -2) // chunked data...
    {
        m_logger.log(
            VERBOSE,
            "[EVENTMANAGER] Incomplete request received for client socket: " +
                Converter::toString(client_socket_descriptor));
    }
    else if (info.first == -3) // Client closed the connection
    {
        // Log the client disconnection
        m_logger.log(VERBOSE,
                     "Client disconnected socket: " +
                         Converter::toString(client_socket_descriptor));

        // Clear buffer, remove from polling and close socket
        m_cleanUp(pollfd_index, client_socket_descriptor);
    }
    else if (info.first == -4) // Add the  body file for the CGI process to poll
    {
        int body_file_descriptor =
            info.second.first; // the file descriptor of the body file

        // Log the situation
        m_logger.log(VERBOSE, "[EVENTMANAGER] Adding body file descriptor '" +
                                  Converter::toString(body_file_descriptor) +
                                  "' to poll");
        pollfd pollfd;
        pollfd.fd = body_file_descriptor;
        pollfd.events = POLLOUT;
        pollfd.revents = 0;
        m_pollfd_manager.addBodyFilePollfd(pollfd);
    }
    else // read pipe returned
    {
        // Get the info
        int cgi_pid = info.first;
        int cgi_output_pipe_read_end = info.second.first;

        // Log the dynamic serving
        m_logger.log(VERBOSE,
                     "[EVENTMANAGER] Dynamically serving client socket: " +
                         Converter::toString(client_socket_descriptor) +
                         " waiting for process " +
                         Converter::toString(cgi_pid) +
                         " (CGI output pipe Read end: " +
                         Converter::toString(cgi_output_pipe_read_end));

        // Add the CGI output pipe Read end to the poll set
        pollfd pollfd;
        pollfd.fd = cgi_output_pipe_read_end;
        pollfd.events = POLLIN | POLLHUP | POLLERR;
        pollfd.revents = 0;
        m_pollfd_manager.addPipePollfd(pollfd);
    }
}

ssize_t EventManager::m_flushBuffer(ssize_t &pollfd_index, short options)
{
    // Get the socket descriptor
    int descriptor = m_pollfd_manager.getDescriptor(pollfd_index);

    // Touch the connection
    try
    {
        m_connection_manager.getConnection(descriptor).touch();
    }
    catch (std::exception &e)
    {
    }

    // Flush the buffer
    ssize_t return_value = m_buffer_manager.flushBuffer(descriptor);
    if (return_value == -1) // check for errors
    {
        // Log the error
        m_logger.log(ERROR, "Error flushing buffer for descriptor: " +
                                Converter::toString(descriptor));

        // Clear buffer, remove from polling and close socket
        m_cleanUp(pollfd_index, descriptor);
    }
    else if (return_value == 0) // check if all bytes were sent
    {
        // Log the flush
        m_logger.log(VERBOSE, "Flushed buffer for descriptor: " +
                                  Converter::toString(descriptor));

        // Clear buffer, remove from polling and close socket
        m_cleanUp(pollfd_index, descriptor, options);
    }
    else
    {
        // Log the flush
        m_logger.log(VERBOSE, "Partially Flushed buffer for descriptor: " +
                                  Converter::toString(descriptor) + " with " +
                                  Converter::toString(return_value) +
                                  " bytes remaining");
    }
    return return_value;
}

void EventManager::m_handleClientException(ssize_t &pollfd_index, short events)
{
    int descriptor = m_pollfd_manager.getDescriptor(pollfd_index);

    // Check for disconnection on the socket
    if (events & POLLHUP)
    {
        // Log the disconnection
        m_logger.log(ERROR, "Client unexpectedly disconnected socket: " +
                                Converter::toString(descriptor));

        // Clear buffer, remove from polling and close socket
        m_cleanUp(pollfd_index, descriptor);
    }

    // Check for invalid request on the socket
    if (events & POLLNVAL)
    {
        // Log the error
        m_logger.log(ERROR, "Invalid request on socket: " +
                                Converter::toString(descriptor));

        // Destroy the buffer associated with the descriptor
        m_buffer_manager.destroyBuffer(descriptor);

        try
        {
            // Let the request handler handle the error
            m_request_handler.handleErrorResponse(descriptor,
                                                  BAD_REQUEST); // 400

            // Add the POLLOUT event for the socket
            m_pollfd_manager.addPollOut(pollfd_index);
        }
        catch (const std::exception &e)
        {
            m_logger.log(ERROR, e.what());
        }
    }

    // Check for errors on the socket
    if (events & POLLERR)
    {
        // Log the error
        m_logger.log(ERROR,
                     "Error on socket: " + Converter::toString(descriptor));

        // Destroy the buffer associated with the descriptor
        m_buffer_manager.destroyBuffer(descriptor);

        // Let the request handler handle the error
        m_request_handler.handleErrorResponse(descriptor,
                                              INTERNAL_SERVER_ERROR); // 500

        // Add the POLLOUT event for the socket
        m_pollfd_manager.addPollOut(pollfd_index);
    }
}

void EventManager::m_cleanUp(ssize_t &pollfd_index, int descriptor,
                             short options)
{
    // Destroy the buffer associated with the descriptor
    m_buffer_manager.destroyBuffer(descriptor);

    // Close the descriptor
    if (options != KEEP_DESCRIPTOR)
    {
        // if it is a client socket, let request handler handle the cleanup
        if ((m_pollfd_manager.getEvents(pollfd_index) & FILE_TYPE_MASK) ==
            CLIENT_SOCKET)
            m_request_handler.removeConnection(descriptor);
        else
            close(descriptor);
    }

    // Remove the descriptor from the poll set
    m_pollfd_manager.removePollfd(pollfd_index);

    // Decrement i to compensate for the removal
    pollfd_index--;

    // Log the cleanup
    m_logger.log(VERBOSE,
                 "Cleaned up descriptor: " + Converter::toString(descriptor));
}

void EventManager::m_handlePipeEvents(ssize_t &pollfd_index, short events)
{
    // Get the pipe descriptor
    int pipe_descriptor = m_pollfd_manager.getDescriptor(pollfd_index);

    // Declare the client socket descriptor linked to the pipe
    int client_socket;

    // Check for exceptions
    if (events & (POLLHUP | POLLERR | POLLNVAL))
    {
        // Set the error description
        std::string error_description;
        if (events & POLLHUP)
            error_description = "Pipe POLLHUP - pipe is no longer connected";
        else if (events & POLLERR)
            error_description = "Pipe POLLERR - asynchronous error";
        else if (events & POLLNVAL)
            error_description = "Pipe POLLNVAL - file descriptor is not open";

        // Log the error
        m_logger.log(ERROR, error_description + " | pipe fd: " +
                                Converter::toString(pipe_descriptor));

        // Let the request handler handle the exception, returns the client
        // socket descriptor linked to the pipe
        client_socket = m_request_handler.handlePipeException(pipe_descriptor);

        // Add the POLLOUT event for the client socket since the error response
        // is ready
        ssize_t client_pollfd_index =
            m_pollfd_manager.getPollfdQueueIndex(client_socket);
        if (client_pollfd_index == -1)
            m_logger.log(ERROR,
                         "[EVENTMANAGER] Client socket not found in poll set");
        else
            m_pollfd_manager.addPollOut(client_pollfd_index);

        // Clear buffer, remove from polling and close pipe
        m_cleanUp(pollfd_index, pipe_descriptor);
    }

    // Read the response from the Response pipe if ready
    else if (events & POLLIN)
    {
        // Log the pipe read
        m_logger.log(VERBOSE, "Pipe read event on pipe: " +
                                  Converter::toString(pipe_descriptor));

        // Let the request handler handle the pipe read, returns the client
        // socket descriptor linked to the pipe
        client_socket = m_request_handler.handlePipeRead(pipe_descriptor);

        // Check if all data was read from the pipe
        if (client_socket ==
            -1) // -1 indicates that the pipe blocked at some point
        {
            m_logger.log(
                VERBOSE,
                "Pipe read buffered, waiting for unblocking on pipe: " +
                    Converter::toString(pipe_descriptor));

            // Return and wait for the next POLLIN event on the pipe
            return;
        }

        // Add the POLLOUT event for the client socket since the response is
        // ready
        ssize_t client_pollfd_index =
            m_pollfd_manager.getPollfdQueueIndex(client_socket);
        if (client_pollfd_index == -1)
            m_logger.log(ERROR,
                         "[EVENTMANAGER] Client socket not found in poll set");
        else
            m_pollfd_manager.addPollOut(client_pollfd_index);

        // Clear buffer, remove from polling and close pipe
        m_cleanUp(pollfd_index, pipe_descriptor);
    }

    // Write the request body to the Request pipe to the cgi process if ready
    else if (events & POLLOUT)
    {
        // Log the pipe write
        m_logger.log(VERBOSE, "Pipe write event on pipe: " +
                                  Converter::toString(pipe_descriptor));

        // Flush the buffer
        m_flushBuffer(pollfd_index);
    }
}

// Path: srcs/EventManager.cpp
