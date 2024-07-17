#ifndef ILOGGER_HPP
#define ILOGGER_HPP

/*
 * ILogger.hpp
 * Abstract base class for Logger in webserv
 *
 * This file defines the Logger interface, which serves as the abstract base
 * class for the Logger in webserv.
 * ILogger allows for polymorphic behavior and dependency injection,
 * thereby enabling us to create a MockLogger class for isolated unit testing.
 *
 */

#include "../constants/LogLevelHelper.hpp"
#include <string>

// Forward declarations
class IConnection;
class ILoggerConfiguration;

// Logger interface
class ILogger
{
public:
    virtual ~ILogger() {};

    // Logging methods
    virtual int log(const std::string &) = 0; // Method to log error messages
    virtual int log(const LogLevel,
                    const std::string &) = 0; // Method to log error messages
    virtual int log(const IConnection &) = 0; // Method to log access events

    // Configuration method
    virtual void configure(ILoggerConfiguration &) = 0; // Method to configure
                                                        // the Logger instance
};

#endif // ILOGGER_HPP
       // Path: includes/ILogger.hpp
