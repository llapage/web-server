#include "../../includes/logger/Logger.hpp"
#include <ctime>
#include <iostream>

/*
 * Logger Class:
 * Provides functionality to log errors and access events.
 * By default logs to std err; this can be overridden through the config file.
 * It supports logging to specified files with customizable log levels. Error
 * and access logs are handled separately. Log messages are not immediately
 * written but instead pused to the BufferManager who handles writing without
 * blocking.
 *
 * Usage:
 * - When adding logging functionality to new components or modules, use the
 * log() methods to add log messages to the buffer.
 * - Use the appropriate log() override
 *
 * Example error log:
 * m_logger.log(INFO, "listening on port 8080");
 * Output in error log: 2011-01-01T01:11:11 [INFO] "listening on port 8080"
 *
 * Example access log:
 * m_logger.log(request, response);
 * Output in access log: timestamp="2011-01-01T01:11:11" clientIP="127.0.0.1"
 * method="GET" requestURI="/index.php" httpVersion="HTTP/1.1" etc.
 */

Logger::Logger(IBufferManager &buffer_manager)
    : m_configuration(NULL), m_buffer_manager(buffer_manager),
      m_log_level_helper()
{
    // Log the initialization of the Logger
    this->log(EXHAUSTIVE, "Logger initialized.");
}

// Destructor: Handles cleanup tasks like flushing buffer and closing log file
// descriptor
Logger::~Logger() {}

// Method to get the current timestamp
std::string Logger::m_getCurrentTimestamp() const
{
    std::stringstream stream;
    std::time_t current_time = std::time(NULL);
    char buffer[ 80 ];

    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&current_time));
    stream << buffer;
    return stream.str();
}

// Default Method to log error messages
int Logger::log(const std::string &message)
{
    // If the Logger is disabled, return without logging
    if (m_configuration != NULL &&
        m_configuration->getErrorLogEnabled() == false)
        return -1;

    // Construct the log message string
    std::string log_message = m_getCurrentTimestamp() + " " + message + "\n";

    // Push the log message to the log file buffer if configured, otherwise push
    // to stderr buffer
    return m_pushToBuffer(log_message,
                          m_configuration
                              ? m_configuration->getErrorLogFileDescriptor()
                              : STDERR_FILENO);
}

// Method to log error messages
int Logger::log(LogLevel logLevel, const std::string &message)
{
    // If the Logger is disabled or the log level below threshold, return
    // without logging
    if (m_configuration != NULL &&
        (m_configuration->getErrorLogEnabled() == false ||
         logLevel < m_configuration->getLogLevel()))
        return -1;

    // Construct the log message string
    std::string log_level_string =
        "[" + m_log_level_helper.logLevelStringMap(logLevel) + "]";
    log_level_string.append(12 - log_level_string.length(),
                            ' '); // Fix width of log level string
    std::string log_message =
        m_getCurrentTimestamp() + " " + log_level_string + " " + message + "\n";

    // Push the log message to the log file buffer if configured, otherwise push
    // to stderr buffer
    return m_pushToBuffer(log_message,
                          m_configuration
                              ? m_configuration->getErrorLogFileDescriptor()
                              : STDERR_FILENO);
}

// Method to log access events
int Logger::log(const IConnection &connection)
{
    // If the Logger is configured and confirmed disabled, return without
    // logging
    if (m_configuration != NULL &&
        m_configuration->getAccessLogEnabled() == false)
        return -1;

    // Get a reference to the Request and Response objects
    IRequest &request = connection.getRequest();
    const IResponse &response = connection.getResponse();

    std::string log_message;
    try // Will fail in case of incorrect request/response
    {
        // Create a temporary stringstream object to construct the log message
        std::ostringstream log_buffer_stream;
        log_buffer_stream
            << "{\n"
            << "\ttimestamp=\"" << m_getCurrentTimestamp() << "\",\n"
            << "\tclient_ip=\"" << connection.getIp() << "\",\n"
            << "\tclient_port=\"" << connection.getPort() << "\",\n"
            << "\tauthority=\"" << request.getAuthority() << "\",\n"
            << "\tmethod=\"" << request.getMethodString() << "\",\n"
            << "\trequest_uri=\"" << request.getUri() << "\",\n"
            << "\thttp_version=\"" << request.getHttpVersionString() << "\",\n"
            << "\tstatus_code=\"" << response.getStatusCodeString()
            << "\tresponse_size=\"" << response.getResponseSizeString()
            << "\",\n"
            << "\tuser_agent=\"" << request.getHeaderValue(USER_AGENT)
            << "\",\n"
            << "\treferrer=\"" << request.getHeaderValue(REFERER) << "\n"
            << "\"\n";

        // Add request headers to the log message
        m_appendMapToLog(log_buffer_stream, "request_headers",
                         request.getHeadersStringMap());

        // Add response headers to the log message
        m_appendMapToLog(log_buffer_stream, "response_headers",
                         response.getHeadersStringMap());

        // Add request cookies to the log message
        m_appendMapToLog(log_buffer_stream, "request_cookies",
                         request.getCookies());

        // Add response cookies to the log message
        m_appendMapToLog(log_buffer_stream, "response_cookies",
                         response.getCookies());

        log_buffer_stream << "}\n";

        // Convert the log message string to a vector of chars
        log_message = log_buffer_stream.str();
    }
    catch (std::exception &e)
    {
        // Log the exception message
        this->log(ERROR, e.what());
    }

    // Push the log message to the access log file buffer if configured,
    // otherwise push to stderr buffer
    return m_pushToBuffer(log_message,
                          m_configuration
                              ? m_configuration->getAccessLogFileDescriptor()
                              : STDERR_FILENO);
}

// Method to append map to log message
void Logger::m_appendMapToLog(
    std::ostringstream &log_buffer_stream, const std::string &field_name,
    const std::map<std::string, std::string> &map) const
{
    std::ostringstream map_stream;

    map_stream << "\t" << field_name << "=\n\t{\n";
    for (std::map<std::string, std::string>::const_iterator it = map.begin();
         it != map.end(); ++it)
    {
        map_stream << "\t\t" << it->first << ": " << it->second;
        std::map<std::string, std::string>::const_iterator next = it;
        next++;
        if (next != map.end())
        {
            map_stream << ",\n";
        }
    }
    map_stream << "\n\t}\n";

    log_buffer_stream << map_stream.str(); // Append the constructed string to
                                           // the log_buffer_stream
}

// Configuration method
void Logger::configure(ILoggerConfiguration &configuration)
{
    m_configuration = &configuration;
}

// Method to push log messages to the buffer
// returns 1 if a flush is requested, or 0 otherwise
int Logger::m_pushToBuffer(const std::string &log_message,
                           const int file_descriptor)
{
    // Copy output to stderr
    std::cerr << log_message;

    // Convert the log message string to a vector of chars
    std::vector<char> log_message_vector(log_message.begin(),
                                         log_message.end());

    // Push the log message to the buffer, returns 1 if a flush is requested
    int return_value =
        m_buffer_manager.pushFileBuffer(file_descriptor, log_message_vector);
    if (return_value == 1 && m_configuration)
    {
        // If the buffer threshold is reached, request a flush
        m_configuration->requestFlush(file_descriptor);
    }

    // Return the return value
    return return_value;
}

// Path: includes/WebservExceptions.hpp
