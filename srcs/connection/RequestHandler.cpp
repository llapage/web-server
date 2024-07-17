#include "../../includes/connection/RequestHandler.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include "../../includes/utils/Converter.hpp"
#include <cstdlib>
#include <fcntl.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <utility>

/*
 * RequestHandler class
 *
 * Responsible for handling client requests:
 * - Reads the raw request from the client
 * - Parses it into a Request object
 * - Passes the Request object to the Router
 *
 * The response is either served statically or dynamically, depending on the
 * route:
 * - Served statically: Response sent immediately to buffer
 * - Served dynamically: Response obtained from a separate process
 *   (read end of the pipe is returned)
 */

// Constructor
RequestHandler::RequestHandler(IBufferManager &buffer_manager,
                               IConnectionManager &connection_manager,
                               const IConfiguration &configuration,
                               IRouter &router, ILogger &logger,
                               const IExceptionHandler &exception_handler,
                               IClientHandler &client_handler)
    : m_buffer_manager(buffer_manager),
      m_connection_manager(connection_manager),
      m_client_handler(client_handler), m_request_parser(configuration, logger),
      m_router(router), m_http_helper(configuration), m_logger(logger),
      m_exception_handler(exception_handler)
{
    // Log the creation of the RequestHandler instance.
    m_logger.log(VERBOSE, "RequestHandler instance created.");
}

// Destructor
RequestHandler::~RequestHandler()
{
    // Log the destruction of the RequestHandler instance.
    m_logger.log(VERBOSE, "RequestHandler instance destroyed.");
}

// Handles a client request
// Returns Cgi Info for dynamic content or -1 for static content and invalid
// requests
Triplet_t RequestHandler::handleRequest(int socket_descriptor)
{
    // Give the 'ClientHandler' the current socket descriptor
    m_client_handler.setSocketDescriptor(socket_descriptor);

    // Get a reference to the Connection
    IConnection &connection =
        m_connection_manager.getConnection(socket_descriptor);

    // Update the connection's last activity time
    connection.touch();

    // Get a reference to the Request
    IRequest &request = connection.getRequest();

    // Get a reference to the Response
    IResponse &response = connection.getResponse();

    RequestState &state = request.getState();

    try
    {
        // Read the raw request from the client
        std::vector<char> raw_request = m_client_handler.readRequest();

        // Check if the client has disconnected
        if (raw_request.empty())
        {
            // EOF read, indicating an orderly disconnect
            return Triplet_t(-3, std::pair<int, int>(-1, -1));
        }

        // Append the raw request to the request buffer
        request.appendBuffer(raw_request);

        if (state.initial())
        {
            // If raw request contains CRLF CRLF, we move to the next stage
            // CRLF CRLF (\r\n\r\n) marks the end of the headers
            std::vector<char> buffer = request.getBuffer();
            std::string buffer_str(buffer.begin(), buffer.end());
            if (buffer_str.find("\r\n\r\n") != std::string::npos)
            {
                state.initial(false);
                state.headers(true); // Because we now have all the headers
            }
            else
            {
                // log the situation
                m_logger.log(VERBOSE,
                             "RequestHandler::handleRequest: Request is "
                             "incomplete - state: initial. Buffer: " +
                                 buffer_str.substr(0, 10));
                return Triplet_t(-2, std::pair<int, int>(-1, -1));
            }
        }
        if (state.headers()) // Parse the headers etc.
        {
            m_request_parser.parseRequest(request);
            state.headers(false);
            // Assign session to connection
            m_connection_manager.assignSessionToConnection(connection, request,
                                                           response);
            if (!state.finished())
            {
                // log the situation
                m_logger.log(VERBOSE, "RequestHandler::handleRequest: Request "
                                      "is incomplete - state: headers done.");
                return Triplet_t(-2, std::pair<int, int>(-1, -1));
            }
        }
        else if (!state.finished())
        {
            m_request_parser.parseBody(request);
            if (!state.finished())
            {
                // log the situation
                m_logger.log(VERBOSE, "RequestHandler::handleRequest: Request "
                                      "is incomplete - state: partial body.");
                return Triplet_t(-2, std::pair<int, int>(-1, -1));
            }
        }

        state.setRoute(m_router.getRoute(&request, &response));

        // in case of cgi, create a temporary file
        if (state.getRoute()->isCGI())
        {
            // create a body file
            std::string body_file_path;
            int fd;
            do
            {
                // Set a unique file path for the body file
                int num = rand() % 1000000;
                body_file_path = "tmp/body_file_" + Converter::toString(num);
                fd = open(body_file_path.c_str(), O_CREAT | O_WRONLY, 0666);
            } while (fd == -1); // if the file already exists, try again

            // set the file descriptor in the request
            request.setBodyFilePath(body_file_path);

            // set the file descriptor to non-blocking
            fcntl(fd, F_SETFL, O_NONBLOCK);

            // push the request body to the buffermanager
            m_buffer_manager.pushFileBuffer(fd, request.getBody(), 0);

            // add the file descriptor to the descriptor-to-client-socket map
            m_pipe_routes[ fd ] = socket_descriptor;

            // return -4 and the fd of the temp file
            return Triplet_t(-4, std::pair<int, int>(fd, -1));
        }
        else
        {
            // If the route is not CGI, we can execute the route
            m_router.execRoute(state.getRoute(), &request, &response);

            state.reset();

            // Push the response to the buffer
            m_sendResponse(socket_descriptor);

            // return -1 to indicate that the content is static
            return Triplet_t(-1, std::pair<int, int>(-1, -1));
        }
    }

    catch (const WebservException &e)
    {
        // Set the request state to finished
        state.finished(true);

        // Get the status code
        int status_code;
        if (dynamic_cast<const HttpStatusCodeException *>(&e))
            status_code =
                e.getErrorCode(); // An HttpStatusCodeException was thrown
        else if (dynamic_cast<const HttpRedirectException *>(&e))
            status_code = 301; // Moved Permanently (Redirect
        else
            status_code = 500; // Internal Server Error; Default status code for
                               // other exceptions

        // Log the exception
        m_exception_handler.handleException(
            e, "RequestHandler::processRequest socket=\"" +
                   Converter::toString(socket_descriptor) + "\"");

        if (status_code == 301) // Redirect
        {
            // Handle redirect response
            this->handleRedirectResponse(socket_descriptor, e.what());
        }
        else
        {
            // Handle error response
            this->handleErrorResponse(socket_descriptor, status_code);
        }

        // return -1
        return Triplet_t(-1, std::pair<int, int>(-1, -1));
    }
}
#include <iostream>
// Execute a Cgi route
Triplet_t RequestHandler::executeCgi(int body_descriptor)
{
    // Close the file descriptor
    close(body_descriptor);

    // Get the client socket descriptor linked to the body_fd
    int socket_descriptor = m_pipe_routes[ body_descriptor ];

    // Remove the file descriptor from the descriptor-to-client-socket map
    m_pipe_routes.erase(body_descriptor);

    // Get a reference to the Connection
    IConnection &connection =
        m_connection_manager.getConnection(socket_descriptor);

    // Update the connection's last activity time
    connection.touch();

    // Get a reference to the Request
    IRequest &request = connection.getRequest();

    // Get a reference to the Response
    IResponse &response = connection.getResponse();

    // Get a reference to the RequestState
    RequestState &state = request.getState();

    // Execute the route
    Triplet_t cgi_info =
        m_router.execRoute(state.getRoute(), &request, &response);

    state.reset();

    // Get CGI Info
    int cgi_pid = cgi_info.first;
    int cgi_output_pipe_read_end = cgi_info.second.first;

    // Record the cgi info
    connection.setCgiInfo(cgi_pid, cgi_output_pipe_read_end);

    // Record the pipes to connection socket mappings
    m_pipe_routes[ cgi_output_pipe_read_end ] = socket_descriptor;

    return cgi_info; // cgi content
}

// Handles exceptions related to pipe events - returns the client socket
// descriptor destination for the response
int RequestHandler::handlePipeException(int pipe_descriptor)
{
    // Get the client socket descriptor linked to the pipe
    int client_socket = m_pipe_routes[ pipe_descriptor ];

    // Remove the pipe descriptor from the map
    m_pipe_routes.erase(pipe_descriptor);

    // Handle error response
    this->handleErrorResponse(client_socket, INTERNAL_SERVER_ERROR);

    // Return the client socket descriptor
    return client_socket;
}

// Handles read input from pipe
// Read into the response buffer until the pipe is empty or blocks
// returns the client socket descriptor destination for the response
// or -1 in case of blocking
int RequestHandler::handlePipeRead(int cgi_output_pipe_read_end)
{
    // Get the client socket descriptor linked to the pipe
    int client_socket = m_pipe_routes[ cgi_output_pipe_read_end ];

    // Give the ClientHandler the current socket descriptor
    m_client_handler.setSocketDescriptor(client_socket);

    // Get a reference to the Response
    IResponse &response = m_connection_manager.getResponse(client_socket);

    // Get a reference to the Response Buffer
    std::vector<char> &response_buffer = response.getBuffer();

    // Read the response from the pipe
    size_t read_buffer_size = 4096;
    ssize_t read_return_value;

    // Read the response from the pipe until the pipe is empty or blocks
    do
    {
        // Resize the response buffer
        response_buffer.resize(response_buffer.size() + read_buffer_size);

        // Read the response from the pipe
        read_return_value = read(cgi_output_pipe_read_end,
                                 response_buffer.data() +
                                     response_buffer.size() - read_buffer_size,
                                 read_buffer_size);
    } while (read_return_value == static_cast<ssize_t>(read_buffer_size));

    // Handle blocking read
    if (read_return_value < 0)
    {
        // Resize the response buffer to the actual size
        response_buffer.resize(response_buffer.size() - read_buffer_size);

        // return -1 to indicate that we are not done reading
        return -1;
    }

    // Resize the response buffer to the actual size
    response_buffer.resize(response_buffer.size() - read_buffer_size +
                           read_return_value);

    // print the response
    m_logger.log(VERBOSE, "CGI response received 100%");

    // Get the child process exit status without blocking
    int cgi_pid = m_connection_manager.getConnection(client_socket).getCgiPid();
    int child_exit_status;
    int exit_code = -3;
    int status;

    // Wait for the child process to exit no blocking
    status = waitpid(cgi_pid, &child_exit_status, WNOHANG);

    if (status == -1) // waitpid failed
    {
        m_logger.log(ERROR, "waitpid failed");
    }
    else if (status == 0) // Child process is still running
    {
        m_logger.log(ERROR, "Cig process ID " + Converter::toString(cgi_pid) +
                                " is still running");
    }
    else
    {
        // Get a reference to the Request
        IRequest &request =
            m_connection_manager.getConnection(client_socket).getRequest();

        // Delete the body file
        std::string body_file_path = request.getBodyFilePath();

        if (body_file_path != "")
            remove(body_file_path.c_str());

        // Close the pipe
        close(cgi_output_pipe_read_end);

        if (WIFEXITED(child_exit_status))
        {
            exit_code = WEXITSTATUS(child_exit_status);
            m_logger.log(VERBOSE, "CGI process ID " +
                                      Converter::toString(cgi_pid) +
                                      " exited normally with exit code " +
                                      Converter::toString(exit_code) + ".");
        }
        else if (WIFSIGNALED(child_exit_status))
        {
            exit_code = WTERMSIG(child_exit_status);
            m_logger.log(ERROR, "CGI process ID " +
                                    Converter::toString(cgi_pid) +
                                    " exited abnormaly with signal " +
                                    Converter::toString(exit_code) + ".");
        }
    }

    if (exit_code != 0 &&
        exit_code != -3) // Check if the CGI process exited normally
        response.setErrorResponse(INTERNAL_SERVER_ERROR); // 500
    else if (response_buffer.empty()) // Check if the response is empty
        response.setErrorResponse(INTERNAL_SERVER_ERROR); // 500
    else
        response.setCgiResponse(response_buffer); // Good response

    // Push the response to the buffer
    m_sendResponse(client_socket);

    // Clean up

    // Remove the descriptors from the pipe;socket map
    m_pipe_routes.erase(cgi_output_pipe_read_end);

    // Reset the CGI info
    m_connection_manager.getConnection(client_socket).clearCgiInfo();

    // Return the client socket descriptor
    return client_socket;
}

// Sends the response to the buffer
int RequestHandler::m_sendResponse(int socket_descriptor)
{
    // Get a reference to the Response
    IResponse &response = m_connection_manager.getResponse(socket_descriptor);

    // Serialise the response
    std::vector<char> serialised_response = response.serialise();

    // Push the response to the buffer
    m_buffer_manager.pushSocketBuffer(socket_descriptor, serialised_response);

    // create an access log entry
    m_logger.log(m_connection_manager.getConnection(socket_descriptor));

    // return 0
    return (0);
}

// Handles error responses
void RequestHandler::handleErrorResponse(int socket_descriptor, int status_code)
{
    // Get a reference to the Response
    IResponse &response = m_connection_manager.getResponse(socket_descriptor);

    // Set the response to the error status code
    response.setErrorResponse(status_code);

    // Push the response to the buffer
    m_sendResponse(socket_descriptor);
}

// Handles redirect responses
void RequestHandler::handleRedirectResponse(int socket_descriptor,
                                            std::string location)
{
    // Get a reference to the Response
    IResponse &response = m_connection_manager.getResponse(socket_descriptor);

    // Set the response to the redirect status code
    response.setRedirectResponse(location);

    // Push the response to the buffer
    m_sendResponse(socket_descriptor);
}

// Handle error responses - HttpStatusCode input
void RequestHandler::handleErrorResponse(int socket_descriptor,
                                         HttpStatusCode status_code)
{
    // Get a reference to the Response
    IResponse &response = m_connection_manager.getResponse(socket_descriptor);

    // Set the response to the error status code
    response.setErrorResponse(status_code);

    // Push the response to the buffer
    m_sendResponse(socket_descriptor);
}

// Remove and close the connection
void RequestHandler::removeConnection(int socket_descriptor)
{
    // Get a reference to the Connection
    IConnection &connection =
        m_connection_manager.getConnection(socket_descriptor);

    // Get a reference to the Request
    IRequest &request = connection.getRequest();

    // Remove the connection if the 'Connection' header is set to 'close'
    if (request.getHeaderValue(CONNECTION) == "close")
        m_connection_manager.removeConnection(socket_descriptor);
    else // temp fix; close it anyway
        m_connection_manager.removeConnection(socket_descriptor);
}

// path: srcs/RequestHandler.cpp
