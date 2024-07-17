#ifndef ISESSION_HPP
#define ISESSION_HPP

/*
 * Session Interface
 *
 * Session class is used to store session data.
 * Its id is set and read from the session cookie.
 * Its lifetime is determined by the timeout since the last access.
 *
 * It is owned by the ConnectionManager.
 *
 */

#include <cstddef>
#include <string>

typedef size_t SessionId_t;

class ISession
{
public:
    virtual ~ISession() {};

    virtual void touch() = 0;            // Update last access time
    virtual bool hasExpired() const = 0; // Check if session has expired
    virtual void setData(const std::string &,
                         const std::string &) = 0; // Set session data
    virtual std::string
    getData(const std::string &) const = 0; // Get session data
    virtual SessionId_t getId() const = 0;  // Get session id
};

#endif // ISESSION_HPP
// Path: includes/connection/ISession.hpp
