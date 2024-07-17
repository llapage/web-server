#ifndef LOGGERCONFIGURATION_HPP
#define LOGGERCONFIGURATION_HPP

/*
 * LoggerConfiguration.hpp
 * This class is responsible for configuring the logger module
 *
 * It expects the following configuration directives:
 *   - in the main block: error_log <path> <level>
 *   - in the http block: access_log <path>
 *
 * To disable error logging, set the error_log directive to "off": 'error_log
 * off' To disable access logging, set the access_log directive to "off":
 * 'access_log off'
 *
 * NOTE: supported log levels currently are: exhaustive, verbose, debug, info,
 * warn, error, critical
 */

#define LOG_BUFFER_SIZE 4096 // 4KB
#define DEFAULT_LOG_LEVEL                                                      \
    WARN // Default log level - Delete this line once Configuration default
         // values are implemented

#include "../configuration/BlockList.hpp"
#include "../configuration/IConfiguration.hpp"
#include "../constants/LogLevelHelper.hpp"
#include "../pollfd/IPollfdManager.hpp"
#include "ILoggerConfiguration.hpp"
#include <fcntl.h>

class LoggerConfiguration : public ILoggerConfiguration
{
private:
    std::string m_error_log_file;
    std::string m_access_log_file;
    IBufferManager &m_buffer_manager;
    IPollfdManager &m_pollfd_manager;
    size_t m_buffer_size;
    LogLevel m_log_level;
    int m_error_log_file_descriptor;
    int m_access_log_file_descriptor;
    bool m_error_log_enabled;
    bool m_access_log_enabled;
    LogLevelHelper m_log_level_helper;

public:
    LoggerConfiguration(IBufferManager &buffer_manager,
                        IConfiguration &configuration,
                        IPollfdManager &pollfd_manager);
    ~LoggerConfiguration();

    virtual void setErrorLogEnabled(bool enabled);
    virtual void setAccessLogEnabled(bool enabled);
    virtual int getErrorLogFileDescriptor() const;
    virtual int getAccessLogFileDescriptor() const;
    virtual IBufferManager &getBufferManager() const;
    virtual std::string getErrorLogFile() const;
    virtual std::string getAccessLogFile() const;
    virtual LogLevel getLogLevel() const;
    virtual bool getErrorLogEnabled() const;
    virtual bool getAccessLogEnabled() const;
    virtual void requestFlush(int descriptor);
};

#endif // LOGGERCONFIGURATION_HPP
       // Path: includes/LoggerConfiguration.hpp
