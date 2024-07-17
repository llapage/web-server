#ifndef POLLINGSERVICE_HPP
#define POLLINGSERVICE_HPP

#include "../logger/ILogger.hpp"
#include "../pollfd/IPollfdManager.hpp"
#include "IPollingService.hpp"
#include <poll.h>

class PollingService : public IPollingService
{
private:
    IPollfdManager &m_pollfd_manager;
    ILogger &m_logger;
    int m_timeout;

public:
    PollingService(IPollfdManager &pollfd_manager, ILogger &logger,
                   int timeout = 100);
    ~PollingService();

    virtual void pollEvents();
    virtual void setPollingTimeout(int timeout);
};

#endif // POLLINGSERVICE_HPP
// Path: includes/core/PollingService.hpp
