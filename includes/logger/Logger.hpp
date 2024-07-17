#ifndef LOGGER_HPP
#define LOGGER_HPP

/*
 * Logger Class:
 * Provides functionality to log errors and access events to separate files.
 * Default log files are "./error.log" and "./access.log"; this can be
 * overridden through the config file. It supports logging to specified files
 * with customizable log levels. Error and access logs are handled separately.
 * Log messages are not immediately written to the log files but are deferred
 * until an appropriate time, after poll() returns.
 *
 * Logger Buffer Usage Summary:
 * The Logger buffer serves as a centralized mechanism for collecting log
 * messages generated throughout the system. How it Works:
 * - Logging Messages: Various components within the system generate log
 * messages during their operation.
 * - Buffering Log Messages: Log messages are buffered into the Logger buffer
 * instead of directly written to an output file or stream.
 * - Deferred Writing: Log messages remain in the buffer until it's time to
 * write them to the output medium, ensuring non-blocking behavior.
 * - Batch Processing: Log messages are processed in batches rather than
 * individually to improve efficiency. There are two instances of the Logger
 * class in webserv:
 * 1. An error Logger, referenced for use in most classes.
 * 2. An access Logger, mainly intended for logging by the ResponseGenerator
 * class.
 *
 * Usage:
 * - When adding logging functionality to new components or modules, use the
 * log() and log() methods to add log messages to the buffer.
 * - Classes with access to the error Logger instance can log errors using the
 * log() method.
 * - Classes with access to the access Logger instance can log access events
 * using the log() method.
 *
 * Example error log:
 * m_logger.log(INFO, "listening on port 8080");
 * Output in error log: timestamp="2011-01-01T01:11:11" level="[info]"
 * message="listening on port 8080"
 *
 * Example access log:
 * m_logger.log(request, response);
 * Output in access log: timestamp="2011-01-01T01:11:11" clientIP="127.0.0.1"
 * method="GET" requestURI="/index.php" httpVersion="HTTP/1.1" etc.
 */

#include "../buffer/IBufferManager.hpp"
#include "../connection/IConnection.hpp"
#include "../constants/LogLevelHelper.hpp"
#include "ILogger.hpp"
#include "ILoggerConfiguration.hpp"
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

class Logger : public ILogger
{
private:
    ILoggerConfiguration *m_configuration;
    IBufferManager &m_buffer_manager;
    const LogLevelHelper m_log_level_helper;

    // Private methods
    std::string
    m_getCurrentTimestamp() const; // Method to get the current timestamp
    void m_appendMapToLog(std::ostringstream &ss, const std::string &field_name,
                          const std::map<std::string, std::string> &data_map)
        const; // Method to append a map to the log message
    int m_pushToBuffer(const std::string &log_message,
                       const int file_descriptor); // Method to push log
                                                   // messages to the buffer

public:
    // Constructors and Destructor
    Logger(IBufferManager &buffer_manager); // Constructor
    virtual ~Logger();                      // Destructor

    // Getter method
    // virtual int getLogFileDescriptor() const; // Getter method for log file
    // descriptor

    // Logging methods
    virtual int
    log(const std::string &message); // Default method to log error messages
    virtual int log(const LogLevel logLevel,
                    const std::string &message); // Method to log error messages
    virtual int
    log(const IConnection &connection); // Method to log access events

    // Configuration method
    virtual void
    configure(ILoggerConfiguration
                  &configuration); // Method to configure the Logger instance
};

#endif // LOGGER_HPP
       // Path: includes/Logger.hpp
