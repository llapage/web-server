#ifndef WEBSERVEXCEPTIONS_HPP
#define WEBSERVEXCEPTIONS_HPP

/*
 * Webserv Error Handling System
 *
 * Error handling, logging, and if necessary gracefully terminating is taken
 * care of by the Webserv IExceptionHandler instance in combination with custom
 * error classes.
 *
 * This file contains the definitions of custom error classes used by webserv.
 * All custom error classes are inherited from the base class WebservException,
 * which is a subclass of std::exception.
 *
 * How to Add New Errors:
 * - To add a new error, create a new class inherited from WebservException.
 * - Define the error message, log level, and error code in the constructor of
 * the new error class.
 * - Optionally, you can add additional methods or members specific to the new
 * error type.
 *
 * Usage:
 * - Throw custom errors when exceptional conditions occur during the execution
 * of the server.
 * - Catch custom errors and pass them to the IExceptionHandler for logging and
 * handling.
 * - NOTE: Setting the log level to CRITICAL will cause the IExceptionHandler to
 * terminate Webserv.
 *
 * Example:
 * // Define a new custom error class
 * class NoConfigFileError : public WebservException {
 * public:
 *     NoConfigFileError()
 *         : WebservException(CRITICAL, "No IConfiguration file found.", 1) {}
 * };
 *
 * // Throw and catch a custom error
 * try {
 *     throw NoConfigFileError();
 * } catch (const WebservException& e) {
 *     exceptionHandler.handleException(e, "Main function");
 * }
 */

#include "../constants/LogLevelHelper.hpp"
#include "../utils/Converter.hpp"
#include <exception>
#include <sstream>
#include <string>

// Base class for all custom exceptions in Webserv
class WebservException : public std::exception
{
private:
    LogLevel m_log_level;
    const std::string m_message;
    int m_error_code;

public:
    // Constructor
    WebservException(LogLevel log_level, const std::string &message,
                     int error_code)
        : m_log_level(log_level), m_message(message),
          m_error_code(error_code) {};

    // Destructor
    virtual ~WebservException() throw() {}

    // Override what() method
    virtual const char *what() const throw() { return m_message.c_str(); }

    // Getter methods
    LogLevel getLogLevel() const { return m_log_level; }
    int getErrorCode() const { return m_error_code; }
};

// Derived error classes
class NoConfigFileError : public WebservException
{
public:
    NoConfigFileError()
        : WebservException(CRITICAL, "No Configuration file found.", 1) {};
};

class InvalidConfigFileError : public WebservException
{
public:
    InvalidConfigFileError()
        : WebservException(CRITICAL, "Invalid Configuration file.", 1) {};
};

class ConfigSyntaxError : public WebservException
{
public:
    ConfigSyntaxError(LogLevel log_level, const std::string &message,
                      int error_code)
        : WebservException(log_level, message, error_code) {};
};

class MaximumConnectionsReachedError : public WebservException
{
public:
    MaximumConnectionsReachedError()
        : WebservException(WARN, "Maximum Connections reached.", 1) {};
};

class ConnectionEstablishingError : public WebservException
{
public:
    ConnectionEstablishingError()
        : WebservException(CRITICAL, "Failed to accept Connection.", 1) {};
};

class PollError : public WebservException
{
public:
    PollError() : WebservException(CRITICAL, "Failed to poll events.", 1) {};
};

class SocketCreateError : public WebservException
{
public:
    SocketCreateError()
        : WebservException(CRITICAL, "Failed to create socket.", 1) {};
};

class SocketSetError : public WebservException
{
public:
    SocketSetError()
        : WebservException(CRITICAL, "Failed to set socket to non-blocking.",
                           1) {};
};

class SocketListenError : public WebservException
{
public:
    SocketListenError()
        : WebservException(CRITICAL, "Failed to listen on socket.", 1) {};
};

class SocketBindError : public WebservException
{
public:
    SocketBindError()
        : WebservException(CRITICAL, "Failed to bind socket.", 1) {};
    SocketBindError(int socket_descriptor, int ip, int port)
        : WebservException(CRITICAL,
                           "Failed to bind socket #" +
                               Converter::toString(socket_descriptor) + " to " +
                               Converter::toString(ip) + ":" +
                               Converter::toString(port),
                           1) {};
};

class ServerSocketError : public WebservException
{
public:
    ServerSocketError()
        : WebservException(CRITICAL, "Error on server socket.", 1) {};
};

class ServerSocketClosedError : public WebservException
{
public:
    ServerSocketClosedError()
        : WebservException(CRITICAL, "Server socket closed.", 1) {};
};

class ServerSocketInvalidError : public WebservException
{
public:
    ServerSocketInvalidError()
        : WebservException(CRITICAL, "Invalid server socket.", 1) {};
};

class LogFileOpenError : public WebservException
{
public:
    LogFileOpenError()
        : WebservException(CRITICAL, "Failed to open log file.", 1) {};
};

class ErrorPageOpenError : public WebservException
{
public:
    ErrorPageOpenError()
        : WebservException(CRITICAL, "Failed to open custom error page.", 1) {};
};

class LogFileWriteError : public WebservException
{
public:
    LogFileWriteError()
        : WebservException(ERROR, "Failed to write to log file.", 1) {};
};

class InvalidHeaderError : public WebservException
{
public:
    InvalidHeaderError() : WebservException(ERROR, "Invalid headers.", 1) {};
};

class InvalidMethodError : public WebservException
{
public:
    InvalidMethodError() : WebservException(ERROR, "Invalid method.", 1) {};
};

class InvalidHttpVersionError : public WebservException
{
public:
    InvalidHttpVersionError()
        : WebservException(ERROR, "Invalid HTTP version.", 1) {};
};

class UnknownMethodError : public WebservException
{
public:
    UnknownMethodError() : WebservException(ERROR, "Unknown method.", 1) {};
    UnknownMethodError(const std::string &method)
        : WebservException(ERROR, "Unknown method: \"" + method + "\"", 1) {};
};

class UnknownHttpVersionError : public WebservException
{
public:
    UnknownHttpVersionError()
        : WebservException(ERROR, "Unknown HTTP version.", 1) {};
    UnknownHttpVersionError(const std::string &version)
        : WebservException(ERROR, "Unknown HTTP version: \"" + version + "\"",
                           1) {};
};

class UnknownHeaderError : public WebservException
{
public:
    UnknownHeaderError() : WebservException(ERROR, "Unknown header.", 1) {};
    UnknownHeaderError(const std::string &header)
        : WebservException(ERROR, "Unknown header: \"" + header + "\"", 1) {};
};

class UnknownLogLevelError : public WebservException
{
public:
    UnknownLogLevelError()
        : WebservException(ERROR, "Unknown log level.", 1) {};
    UnknownLogLevelError(const std::string &log_level)
        : WebservException(ERROR, "Unknown log level: \"" + log_level + "\"",
                           1) {};
};

class UnknownHttpStatusCodeError : public WebservException
{
public:
    UnknownHttpStatusCodeError()
        : WebservException(ERROR, "Unknown HTTP status code.", 1) {};
    UnknownHttpStatusCodeError(const std::string &status_code)
        : WebservException(
              ERROR, "Unknown HTTP status code: \"" + status_code + "\"", 1) {};
};

class HttpStatusCodeException : public WebservException
{
private:
    const std::string m_getStatusCodeString(int status_code) const
    {
        std::ostringstream oss;
        oss << status_code;
        return oss.str();
    }

public:
    HttpStatusCodeException(int status_code)
        : WebservException(INFO,
                           ("Http Status Code: \"" +
                            m_getStatusCodeString(status_code) + "\""),
                           status_code) {};
    HttpStatusCodeException(int status_code, const std::string &message)
        : WebservException(INFO,
                           ("Http Status Code " +
                            m_getStatusCodeString(status_code) + ": " +
                            message),
                           status_code)
    {
    }
};

class SigintException : public WebservException
{
public:
    SigintException() : WebservException(CRITICAL, "SIGINT received.", 0) {};
};

class SigsegvException : public WebservException
{
public:
    SigsegvException() : WebservException(CRITICAL, "SIGSEGV received.", 0) {};
};

class HttpRedirectException : public WebservException
{
public:
    HttpRedirectException(const std::string &location)
        : WebservException(INFO, location, 0) {};
};

#endif // WEBSERVEXCEPTIONS_HPP
       // Path: includes/exeption/WebservExceptions.hpp
