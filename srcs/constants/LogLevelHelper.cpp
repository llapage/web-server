#include "../../includes/constants/LogLevelHelper.hpp"
#include "../../includes/exception/WebservExceptions.hpp"

/*
 * LogLevelHelper.hpp
 *
 * The LogLevelHelper class provides functionality for working with log levels.
 * It defines an enum LogLevel to represent log levels and provides methods
 * to retrieve string representations of log levels and vice versa.
 *
 */

// Constructor initializes member variables using helper functions
LogLevelHelper::LogLevelHelper()
    : m_log_level_list(m_setLogLevelList()),
      m_string_log_level_map(m_setStringLogLevelMap()),
      m_log_level_string_map(m_setLogLevelStringMap())
{
}

// Get string representation of LogLevel enum value
const std::string &LogLevelHelper::logLevelStringMap(LogLevel logLevel) const
{
    if (m_log_level_string_map.find(logLevel) != m_log_level_string_map.end())
    {
        return m_log_level_string_map.at(logLevel);
    }
    else
    {
        throw UnknownLogLevelError();
    }
}

// Get LogLevel enum value from string representation
LogLevel LogLevelHelper::stringLogLevelMap(const std::string &logLevel) const
{
    if (m_string_log_level_map.find(logLevel) != m_string_log_level_map.end())
    {
        return m_string_log_level_map.at(logLevel);
    }
    else
    {
        throw UnknownLogLevelError(logLevel);
    }
}

// Helper function to initialize m_log_level_list with string representations of
// HTTP logLevels
std::vector<std::string> LogLevelHelper::m_setLogLevelList()
{
    std::vector<std::string> log_level_list;

    // Add string representations of HTTP logLevels to LogLevelList
    log_level_list.push_back("exhaustive");
    log_level_list.push_back("verbose");
    log_level_list.push_back("debug");
    log_level_list.push_back("info");
    log_level_list.push_back("warn");
    log_level_list.push_back("error");
    log_level_list.push_back("critical");
    log_level_list.push_back("unknown");

    return log_level_list;
}

// Helper function to initialize m_string_log_level_map with mappings from
// string representations to LogLevel enum values
std::map<std::string, LogLevel> LogLevelHelper::m_setStringLogLevelMap()
{
    std::map<std::string, LogLevel> string_log_level_map;

    // Add mappings from string representations to LogLevel enum values
    string_log_level_map[ "exhaustive" ] = EXHAUSTIVE;
    string_log_level_map[ "verbose" ] = VERBOSE;
    string_log_level_map[ "debug" ] = DEBUG;
    string_log_level_map[ "info" ] = INFO;
    string_log_level_map[ "warn" ] = WARN;
    string_log_level_map[ "error" ] = ERROR;
    string_log_level_map[ "critical" ] = CRITICAL;
    string_log_level_map[ "unknown" ] = UNKNOWN;

    return string_log_level_map;
}

// Helper function to initialize m_log_level_string_map with mappings from
// LogLevel enum values to string representations
std::map<LogLevel, std::string> LogLevelHelper::m_setLogLevelStringMap()
{
    std::map<LogLevel, std::string> log_level_string_map;

    // Add mappings from LogLevel enum values to string representations
    log_level_string_map[ EXHAUSTIVE ] = "exhaustive";
    log_level_string_map[ VERBOSE ] = "verbose";
    log_level_string_map[ DEBUG ] = "debug";
    log_level_string_map[ INFO ] = "info";
    log_level_string_map[ WARN ] = "warn";
    log_level_string_map[ ERROR ] = "error";
    log_level_string_map[ CRITICAL ] = "critical";
    log_level_string_map[ UNKNOWN ] = "unknown";

    return log_level_string_map;
}

// Path: includes/constants/LogLevelHelper.hpp
