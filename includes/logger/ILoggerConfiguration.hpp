#ifndef ILOGGERCONFIGURATION_HPP
#define ILOGGERCONFIGURATION_HPP

#include "../buffer/IBufferManager.hpp"
#include "../constants/LogLevelHelper.hpp"

#include <string>

class ILoggerConfiguration
{
public:
    virtual ~ILoggerConfiguration() {}

    virtual void setErrorLogEnabled(bool) = 0;
    virtual void setAccessLogEnabled(bool) = 0;
    virtual int getErrorLogFileDescriptor() const = 0;
    virtual int getAccessLogFileDescriptor() const = 0;
    virtual IBufferManager &getBufferManager() const = 0;
    virtual std::string getErrorLogFile() const = 0;
    virtual std::string getAccessLogFile() const = 0;
    virtual LogLevel getLogLevel() const = 0;
    virtual bool getErrorLogEnabled() const = 0;
    virtual bool getAccessLogEnabled() const = 0;
    virtual void requestFlush(int descriptor) = 0;
};

#endif // ILOGGERCONFIGURATION_HPP
       // Path: includes/ILoggerConfiguration.hpp
