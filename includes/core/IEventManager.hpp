#ifndef IEVENTMANAGER_HPP
#define IEVENTMANAGER_HPP

/*
 * IEventManager.hpp
 *
 * Abstract class for handling events.
 */

class IEventManager
{
public:
    virtual ~IEventManager() {}

    virtual void handleEvents() = 0;
};

#endif // IEVENTMANAGER_HPP
// Path: includes/core/IEventManager.hpp
