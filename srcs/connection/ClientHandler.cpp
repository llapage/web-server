#include "../../includes/connection/ClientHandler.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include "../../includes/utils/Converter.hpp"

/*
 * Summary:
 * ClientHandler class is responsible for handling communication with clients.
 * It reads requests from clients and sends responses back. It also handles
 * errors that occur during communication and logs them appropriately.
 */

// Constructor
ClientHandler::ClientHandler(const ISocket &socket, ILogger &logger)
    : m_socket(socket), m_logger(logger)
{
    // Log the creation of the ClientHandler
    m_logger.log(VERBOSE, "Clienthandler created.");
}

// Destructor
ClientHandler::~ClientHandler()
{
    // Log the destruction of the ClientHandler
    m_logger.log(VERBOSE, "Clienthandler destroyed.");
}

// Setter method to set the socket descriptor
void ClientHandler::setSocketDescriptor(int socket_descriptor)
{
    m_socket_descriptor = socket_descriptor;
}

// Method to read a request from the client
const std::vector<char> ClientHandler::readRequest() const
{
    size_t offset = 0;
    size_t buffer_size = 4096;
    ssize_t max_read_size = buffer_size - offset;
    ssize_t bytes_read = 0;
    std::vector<char> buffer(buffer_size);

    // Read data from the client socket
    // Double the buffer as long as necessary
    while ((bytes_read = m_socket.recv(m_socket_descriptor, &buffer[ offset ],
                                       max_read_size)) == max_read_size)
    {
        // Move the offset to the end of the buffer
        offset += bytes_read;
        // Double the buffer size
        buffer_size *= 2;
        // Calculate the new maximum read size
        max_read_size = buffer_size - offset;
        // Resize the buffer
        buffer.resize(buffer_size);
    }

    // Handle read errors
    // NOTE that MSG_DONTWAIT might cause -1 to be returned
    // it is unclear if this is an error (ECONNRESET) or not
    // (EWOULDBLOCK/EAGAIN) without reading errno
    if (bytes_read == -1)
    {
        buffer.clear();
        throw WebservException(ERROR,
                               "Error reading request from client on socket: " +
                                   Converter::toString(m_socket_descriptor),
                               1);
    }

    // Resize the buffer to the actual size of the data read
    buffer.resize(offset + bytes_read);

    // Log the request read from the client
    m_logger.log(EXHAUSTIVE,
                 "[CLIENTHANDLER] Request on socket: " +
                     Converter::toString(m_socket_descriptor) + ": \"" +
                     std::string(buffer.begin(), buffer.end()) + "\"");

    // Return the buffer containing the request
    return buffer;
}

// Method to send a response to the client as a vector of characters
ssize_t ClientHandler::sendResponse(const std::vector<char> &response) const
{
    ssize_t bytesSent = m_socket.send(m_socket_descriptor, response);
    if (bytesSent == -1)
        m_logger.log(ERROR, "Error sending response to client on socket: " +
                                Converter::toString(m_socket_descriptor));

    // Log the response sent to the client
    m_logger.log(VERBOSE,
                 "[CLIENTHANDLER] Responded on socket: " +
                     Converter::toString(m_socket_descriptor) + ": \"" +
                     std::string(response.begin(), response.end()) + "\"");

    // Return the number of bytes sent
    return bytesSent;
}

// Method to send a response to the client as a string
ssize_t ClientHandler::sendResponse(const std::string &response) const
{
    return this->sendResponse(
        std::vector<char>(response.begin(), response.end()));
}

// Path: includes/ClientHandler.hpp
