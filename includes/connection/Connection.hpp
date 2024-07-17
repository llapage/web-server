#ifndef CONNECTION_HPP
#define CONNECTION_HPP

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

#define DEFAULT_TIMEOUT 300     // 5 minutes
#define CGI_DEFAULT_TIMEOUT 300 // 20 seconds

#include "../logger/ILogger.hpp"
#include "IConnection.hpp"
#include <ctime>

class Connection : public IConnection
{
private:
    SocketDescriptor_t
        m_socket_descriptor;        // Socket descriptor for the connection
    std::string m_ip;               // Client's IP address
    int m_port;                     // Port number
    std::string m_remote_address;   // Remote address
    int m_cgi_output_pipe_read_end; // Read pipe descriptor for the response
    int m_cgi_pid;                  // PID of the CGI process
    ILogger &m_logger;              // Reference to the logger
    IRequest *m_request;            // Pointer to the request object
    IResponse *m_response;          // Pointer to the response object
    ISession *m_session;            // Pointer to the session object
    const time_t m_timeout;         // Timeout for the connection
    time_t m_last_access;           // Last access time
    time_t m_cgi_start_time;        // Cgi start time

public:
    Connection(std::pair<int, std::pair<std::string, std::string> > client_info,
               ILogger &logger, IRequest *request, IResponse *response,
               time_t timeout = DEFAULT_TIMEOUT);
    virtual ~Connection();

    // Setters
    virtual void setSession(ISession *session);
    virtual void clearCgiInfo();

    // Getters
    virtual int getSocketDescriptor() const;
    virtual std::string getIp() const;
    virtual int getPort() const;
    virtual std::string getRemoteAddress() const;
    virtual int getCgiOutputPipeReadEnd() const;
    virtual IRequest &getRequest() const;
    virtual IResponse &getResponse() const;
    virtual ISession &getSession() const;
    virtual int getCgiPid() const;
    virtual void setCgiInfo(int pid, int response_read_pipe_fd);

    // Connection management
    virtual void touch();               // Update the last access time
    virtual bool hasExpired() const;    // Check if the connection has expired
    virtual bool cgiHasExpired() const; // Check if the CGI process has expired
};

#endif // CONNECTION_HPP
// Path: includes/connection/Connection.hpp
