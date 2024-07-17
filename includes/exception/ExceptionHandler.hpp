#ifndef EXCEPTIONHANDLER_HPP
#define EXCEPTIONHANDLER_HPP

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

#include "../constants/HttpStatusCodeHelper.hpp" // Include necessary dependencies
#include "../exception/WebservExceptions.hpp" // Include necessary dependencies
#include "../logger/ILogger.hpp"              // Include necessary dependencies
#include "IExceptionHandler.hpp"
#include <string>

// Forward declaration of Server class
class Server;

class ExceptionHandler : public IExceptionHandler
{
private:
    ILogger &m_logger; // Reference to the logger instance
    Server *m_server;  // Pointer to the server instance
    const HttpStatusCodeHelper
        m_http_status_code_helper; // Helper instance for HTTP status codes

    int m_handleWebservException(const WebservException &e,
                                 const std::string &context = "") const;
    int m_handleStandardException(const std::exception &e,
                                  const std::string &context = "") const;

public:
    // Constructor: Initializes ExceptionHandler with an logger instance.
    ExceptionHandler(ILogger &logger);

    // Constructor: Initializes ExceptionHandler with an logger instance and a
    // pointer to the server instance.
    ExceptionHandler(ILogger &logger, Server *server);

    // Destructor: No dynamic memory management, so a default destructor is
    // sufficient.
    ~ExceptionHandler();

    // handleException method: Logs exception details and handles critical
    // exceptions.
    int handleException(const std::exception &e,
                        const std::string &context = "") const;
};

#endif // EXCEPTIONHANDLER_HPP
       // Path: includes/exceoption/ExceptionHandler.hpp
