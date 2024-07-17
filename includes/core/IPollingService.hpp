#ifndef IPOLLINGSERVICE_HPP
#define IPOLLINGSERVICE_HPP

/*
 * IPollingService.hpp
 *
 * Abstract class for handling polling events.
 */

class IPollingService
{
public:
    virtual ~IPollingService() {};

    virtual void pollEvents() = 0;
    virtual void setPollingTimeout(int) = 0;
};

#endif // IPOLLINGSERVICE_HPP
// Path: includes/core/IPollingService.hpp
