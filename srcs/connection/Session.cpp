#include "../../includes/connection/Session.hpp"

/*
 * Session
 *
 * Session class is used to store session data.
 * Its id is set and read from the session cookie.
 * Its lifetime is determined by the timeout since the last access.
 *
 * It is owned by the ConnectionManager.
 *
 */

// Constructor
Session::Session(SessionId_t id, time_t timeout) : m_id(id), m_timeout(timeout)
{
    this->touch();
}

// Destructor
Session::~Session() {}

// Touch session updates last access time
void Session::touch() { m_last_access = time(NULL); }

// Check if session has expired
bool Session::hasExpired() const
{
    return time(NULL) - m_last_access > m_timeout;
}

// Set session data
void Session::setData(const std::string &key, const std::string &value)
{
    m_data[ key ] = value;
}

// Get session data
std::string Session::getData(const std::string &key) const
{
    std::map<std::string, std::string>::const_iterator it = m_data.find(key);

    if (it == m_data.end())
        return "";
    return it->second;
}

// Get session id
SessionId_t Session::getId() const { return m_id; }

// Path: srcs/connection/Session.cpp
