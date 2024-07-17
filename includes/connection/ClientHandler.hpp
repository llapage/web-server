#ifndef CLIENTHANDLER_HPP
#define CLIENTHANDLER_HPP

/*
 * Summary:
 * ClientHandler class is responsible for handling communication with clients.
 * It reads requests from clients and sends responses back. It also handles
 * errors that occur during communication and logs them appropriately.
 */

#include <string>

#include "../logger/ILogger.hpp"
#include "../network/ISocket.hpp"
#include "IClientHandler.hpp"

class ClientHandler : public IClientHandler
{
private:
    const ISocket &m_socket; // Reference to the Socket instance
    int m_socket_descriptor; // Socket descriptor for client connection
    ILogger &m_logger;       // Reference to the error logger object

public:
    // Constructor: Initializes the ClientHandler with an error logger and
    // exception handler
    ClientHandler(const ISocket &socket, ILogger &logger);

    // Destructor
    ~ClientHandler();

    // Setter method to set the socket descriptor
    void setSocketDescriptor(int socket_descriptor);

    // Method to read a request from the client
    const std::vector<char> readRequest() const;

    // Method to send a response to the client as a vector of characters
    ssize_t sendResponse(const std::vector<char> &response) const;

    // Method to send a response to the client as a string
    ssize_t sendResponse(const std::string &response) const;
};

#endif // CLIENTHANDLER_HPP
       // Path: includes/connection/ClientHandler.hpp
