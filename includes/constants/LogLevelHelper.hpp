#ifndef LOGLEVELHELPER_HPP
#define LOGLEVELHELPER_HPP

/*
 * LogLevelHelper.hpp
 *
 * The LogLevelHelper class provides functionality for working with log levels.
 * It defines an enum LogLevel to represent log levels and provides methods
 * to retrieve string representations of log levels and vice versa.
 *
 */

#include <map>
#include <string>
#include <vector>

// Forward declaration of WebServer exceptions
class WebservExceptions;

enum LogLevel
{
    EXHAUSTIVE = 0, // TRACE messages
    VERBOSE = 1,    // VERBOSE messages
    DEBUG = 2,      // Debugging information
    INFO = 3,       // Informational messages
    WARN = 4,       // Warnings
    ERROR = 5,      // Errors
    CRITICAL = 6,   // Critical errors
    UNKNOWN = 7     // Unknown log level
};

class LogLevelHelper
{
private:
    // Member variables
    const std::vector<std::string>
        m_log_level_list; // List of string representations of log levels
    const std::map<std::string, LogLevel>
        m_string_log_level_map; // Map of string representations to LogLevel
                                // enum values
    const std::map<LogLevel, std::string>
        m_log_level_string_map; // Map of LogLevel enum values to string
                                // representations

    // Private member functions for initialization
    static std::vector<std::string> m_setLogLevelList();
    static std::map<std::string, LogLevel> m_setStringLogLevelMap();
    static std::map<LogLevel, std::string> m_setLogLevelStringMap();

public:
    // Constructor
    LogLevelHelper();

    // Member functions to access data
    const std::string &logLevelStringMap(
        LogLevel log_level) const; // Get string representation of LogLevel
    LogLevel stringLogLevelMap(const std::string &log_level)
        const; // Get LogLevel enum value from string representation
};

#endif // LOGLEVELHELPER_HPP
// Path: includes/constants/LogLevelHelper.hpp
