#include "../../includes/connection/Connection.hpp"
#include "../../includes/utils/Converter.hpp"
#include <csignal>
#include <ctime>
#include <unistd.h>

/*
 * Connection class
 *
 * This class represents a network connection and interaction with a client.
 * It stores the socket descriptor, client IP address, port number, and remote
 * address, as well as pointers to the request and response objects. Its
 * lifetime is determined either by the 'Connection' header in the request or
 * the timeout value.
 *
 * It is owned by the ConnectionManager.
 *
 */

// Constructor
Connection::Connection(
    std::pair<int, std::pair<std::string, std::string> > client_info,
    ILogger &logger, IRequest *request, IResponse *response, time_t timeout)
    : m_socket_descriptor(client_info.first), m_ip(client_info.second.first),
      m_port(Converter::toInt(client_info.second.second)),
      m_remote_address(m_ip + ":" + client_info.second.second),
      m_cgi_output_pipe_read_end(-1), m_cgi_pid(-1), m_logger(logger),
      m_request(request), m_response(response), m_timeout(timeout)
{
    m_last_access = time(NULL);
}

// Destructor
Connection::~Connection()
{
    delete m_request;
    delete m_response;
    if (m_cgi_pid != -1)
    {
        kill(m_cgi_pid, SIGKILL);
    }
    close(m_cgi_output_pipe_read_end);
}

// Set session
void Connection::setSession(ISession *session) { m_session = session; }

// Clear CGI PID
void Connection::clearCgiInfo()
{
    m_cgi_pid = -1;
    m_cgi_output_pipe_read_end = -1;
}

// Getters
int Connection::getSocketDescriptor() const { return m_socket_descriptor; }

std::string Connection::getIp() const { return m_ip; }

int Connection::getPort() const { return m_port; }

std::string Connection::getRemoteAddress() const { return m_remote_address; }

int Connection::getCgiOutputPipeReadEnd() const
{
    return m_cgi_output_pipe_read_end;
}

IRequest &Connection::getRequest() const { return *m_request; }

IResponse &Connection::getResponse() const { return *m_response; }

ISession &Connection::getSession() const { return *m_session; }

int Connection::getCgiPid() const { return m_cgi_pid; }

void Connection::setCgiInfo(int cgi_pid, int cgi_output_pipe_read_end)
{
    m_cgi_pid = cgi_pid;
    m_cgi_start_time = time(NULL);
    m_cgi_output_pipe_read_end = cgi_output_pipe_read_end;
}

// Connection management
void Connection::touch()
{
    // Get the current time
    time_t now = time(NULL);

    // Set time strings for logging
    std::string last_access_string = ctime(&m_last_access);
    std::string now_string = ctime(&now);

    // Remove the newline characters from the strings; introduced by ctime
    last_access_string.erase(last_access_string.length() - 1);
    now_string.erase(now_string.length() - 1);

    // Log the last access update
    m_logger.log(VERBOSE, "Updating last access for connection with: " +
                              m_remote_address + " from " + last_access_string +
                              " to " + now_string);

    // Update the last access time
    m_last_access = now;
}

bool Connection::hasExpired() const
{
    return time(NULL) - m_last_access > m_timeout;
}

bool Connection::cgiHasExpired() const
{
    if (m_cgi_pid == -1)
        return false;
    return time(NULL) - m_cgi_start_time > CGI_DEFAULT_TIMEOUT;
}

// Path: srcs/Connection.cpp
