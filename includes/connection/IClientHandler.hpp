#ifndef ICLIENTHANDLER_HPP
#define ICLIENTHANDLER_HPP

/*
 * IClientHandler.hpp
 * Abstract base class for ClientHandler in webserv
 *
 * This file defines the ClientHandler interface, which serves as the abstract
 * base class for the ClientHandler in webserv. IClientHandler allows for
 * polymorphic behavior and dependency injection, thereby enabling us to create
 * a MockClientHandler class for isolated unit testing.
 *
 */

#include <string>
#include <sys/socket.h>
#include <vector>

class IClientHandler
{
public:
    // Virtual destructor
    virtual ~IClientHandler() {};

    // Setter
    virtual void setSocketDescriptor(int socket_descriptor) = 0;

    // Methods to read and send requests and responses
    virtual const std::vector<char> readRequest() const = 0;
    virtual ssize_t sendResponse(const std::vector<char> &response) const = 0;
    virtual ssize_t sendResponse(const std::string &response) const = 0;
};

#endif // ICLIENTHANDLER_HPP
// Path: includes/connection/IClientHandler.hpp
