#include "../../includes/exception/ExceptionHandler.hpp"
#include "../../includes/network/Server.hpp"

/*
 * ExceptionHandler Class:
 * Provides functionality to handle webserv exceptions thrown during execution.
 * Logs exception details using the webserv logger instance.
 * When it encounters a webserv spcific exception (WebserveException),
 * it will handle them  based on the specified log level, and terminates the
 * server if necessary (e.getLogLevel() == CRITICAL).
 *
 * Usage:
 * -through the Webserv ExceptionHandler instance
 *
 * Example:
 * try {
 *     // Some code that may throw exceptions
 *     throw Exception();
 * } catch (const WebservException& e) {
 *      exceptionHandler.handleException(e, "Main function"); // returns the
 * error code of the exception
 * }
 *
 * Note: 'CRITICAL' log level will cause webserv to terminate (in case of a
 * WebservException). Note: To use Webserv custom exceptions, include the
 * WebservExceptions.hpp file.
 */

// Constructor: Initializes ExceptionHandler with an logger instance, m_server
// linking is deferred
ExceptionHandler::ExceptionHandler(ILogger &logger)
    : m_logger(logger), m_server(NULL)
{
    // Log debug message indicating the creation of an ExceptionHandler
    // instance.
    m_logger.log(VERBOSE, "ExceptionHandler created.");
}

// Constructor: Initializes ExceptionHandler with an logger instance and a
// pointer to the server instance.
ExceptionHandler::ExceptionHandler(ILogger &logger, Server *server)
    : m_logger(logger), m_server(server)
{
    // Log debug message indicating the creation of an ExceptionHandler
    // instance.
    m_logger.log(VERBOSE, "ExceptionHandler created.");
}

// Destructor: No dynamic memory management, so a default destructor is
// sufficient.
ExceptionHandler::~ExceptionHandler()
{
    // Log debug message indicating the destruction of an ExceptionHandler
    // instance.
    m_logger.log(VERBOSE, "ExceptionHandler destroyed.");
}

// m_handleWebservException method: Logs exception details and handles critical
// exceptions.
int ExceptionHandler::m_handleWebservException(const WebservException &e,
                                               const std::string &context) const
{
    // Log the exception details, including context and error message.
    m_logger.log(e.getLogLevel(), "[EXCEPTION] " + context +
                                      (context.empty() ? ": " : " : ") +
                                      e.what());

    // Check if the exception's log level is critical.
    if (e.getLogLevel() == CRITICAL)
    {
        // If critical, log termination message and terminate the server.
        m_logger.log(CRITICAL, "!! webserv will now terminate.");
        if (m_server)
            m_server->terminate(e.getErrorCode());
    }
    return e.getErrorCode();
}

// m_handleStandardException method: Logs standard exception details
int ExceptionHandler::m_handleStandardException(
    const std::exception &e, const std::string &context) const
{
    // Log the exception details, including context and error message.
    m_logger.log(UNKNOWN, "[EXCEPTION] " + context +
                              (context.empty() ? ": " : " : ") + e.what());
    return 0;
}

// handleException method: Verifies the type of exception and calls the
// appropriate handler.
int ExceptionHandler::handleException(const std::exception &e,
                                      const std::string &context) const
{
    if (const WebservException *wse =
            dynamic_cast<const WebservException *>(&e))
    {
        return m_handleWebservException(*wse, context);
    }
    else
    {
        return m_handleStandardException(e, context);
    }
}

// Path: srcs/ExceptionHandler.cpp
