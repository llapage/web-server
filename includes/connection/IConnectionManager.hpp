#ifndef ICONNECTIONMANAGER_HPP
#define ICONNECTIONMANAGER_HPP

/*
 * ConnectionManager Interface
 *
 * This class is responsible for managing connections and sessions.
 * A connection in this context represents an active interaction with a client,
 * which ends when the response is sent. A session represents a session with a
 * client, which can span multiple connections, and lasts until the inactivity
 * timeout.
 *
 */

#include "IConnection.hpp"
#include <string>

class IConnectionManager
{
public:
    virtual ~IConnectionManager() {};

    // Methods related to connections
    virtual void
        addConnection(std::pair<int, std::pair<std::string, std::string> >) = 0;
    virtual void removeConnection(SocketDescriptor_t) = 0;
    virtual IConnection &getConnection(SocketDescriptor_t) = 0;
    virtual IRequest &getRequest(SocketDescriptor_t) = 0;
    virtual IResponse &getResponse(SocketDescriptor_t) = 0;

    // Methods related to sessions
    virtual SessionId_t addSession() = 0;
    virtual void removeSession(SessionId_t) = 0;
    virtual ISession &getSession(SessionId_t) = 0;
    virtual void setSessionData(SessionId_t, const std::string &,
                                const std::string &) = 0;
    virtual std::string getSessionData(SessionId_t, const std::string &) = 0;
    virtual size_t getNumberOfSessions() const = 0;

    // Method to assign a session to a connection
    virtual void assignSessionToConnection(IConnection &, const IRequest &,
                                           IResponse &) = 0;

    // Method to retire idle sessions
    virtual void collectGarbage() = 0;
};

#endif // ICONNECTIONMANAGER_HPP
// Path: includes/connection/IConnectionManager.hpp
