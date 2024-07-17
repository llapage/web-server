#ifndef CONNECTIONMANAGER_HPP
#define CONNECTIONMANAGER_HPP

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

#define GARBAGE_COLLECTOR_INTERVAL 30 // every 30 seconds

#include <sys/wait.h>

#include <map>

#include "../factory/IFactory.hpp"
#include "../logger/ILogger.hpp"
#include "IConnection.hpp"
#include "IConnectionManager.hpp"
#include "ISession.hpp"

class ConnectionManager : public IConnectionManager
{
private:
    std::map<SocketDescriptor_t, IConnection *>
        m_connections;                            // active connections
    std::map<SessionId_t, ISession *> m_sessions; // active sessions
    time_t
        m_last_garbage_collection; // last time garbage collection was performed
    IFactory &m_factory; // factory object to create connections and sessions
    ILogger &m_logger;   // logger object

    // Generate a unique session ID
    SessionId_t m_generateSessionId() const;
    SessionId_t m_stringToSessionId(const std::string &string) const;

public:
    // Constructor
    ConnectionManager(ILogger &logger, IFactory &factory);

    // Destructor
    virtual ~ConnectionManager();

    // Methods related to connections
    virtual void addConnection(
        std::pair<int, std::pair<std::string, std::string> > client_info);
    virtual void removeConnection(SocketDescriptor_t socket_descriptor);
    virtual IConnection &getConnection(SocketDescriptor_t socket_descriptor);
    virtual IRequest &getRequest(SocketDescriptor_t socket_descriptor);
    virtual IResponse &getResponse(SocketDescriptor_t socket_descriptor);

    // Methods related to sessions
    virtual SessionId_t addSession();
    virtual void removeSession(SessionId_t id);
    virtual ISession &getSession(SessionId_t id);
    virtual void setSessionData(SessionId_t id, const std::string &key,
                                const std::string &value);
    virtual std::string getSessionData(SessionId_t id, const std::string &key);
    virtual size_t getNumberOfSessions() const;

    // Method to assign a session to a connection
    virtual void assignSessionToConnection(IConnection &connection,
                                           const IRequest &request,
                                           IResponse &response);

    // Method to retire idle sessions
    virtual void collectGarbage();
};

#endif // CONNECTIONMANAGER_HPP
// Path: includes/connection/ConnectionManager.hpp
