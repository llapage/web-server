#ifndef ICONNECTION_HPP
#define ICONNECTION_HPP

/*
 * Connection class
 *
 * This class represents a network connection and interaction with a client.
 * It stores the socket descriptor, client IP address, port number, and remote
 * address, as well as pointers to the request and response objects. Its
 * lifetime is tied to the active connection with the client.
 *
 * It is owned by the ConnectionManager.
 *
 */

typedef int SocketDescriptor_t;

#include "../connection/ISession.hpp"
#include "../request/IRequest.hpp"
#include "../response/IResponse.hpp"
#include <string>

class IConnection
{
public:
    virtual ~IConnection() {};

    // Setters
    virtual void setSession(ISession *session) = 0;
    virtual void clearCgiInfo() = 0;

    // Getters
    virtual int getSocketDescriptor() const = 0;
    virtual std::string getIp() const = 0;
    virtual int getPort() const = 0;
    virtual std::string getRemoteAddress() const = 0;
    virtual int getCgiOutputPipeReadEnd() const = 0;
    virtual IRequest &getRequest() const = 0;
    virtual IResponse &getResponse() const = 0;
    virtual ISession &getSession() const = 0;
    virtual int getCgiPid() const = 0;
    virtual void setCgiInfo(int, int) = 0;

    // Connection management
    virtual void touch() = 0;            // Update the last access time
    virtual bool hasExpired() const = 0; // Check if the connection has expired
    virtual bool
    cgiHasExpired() const = 0; // Check if the CGI process has expired
};

#endif // ICONNECTION_HPP
// Path: includes/connection/IConnection.hpp
