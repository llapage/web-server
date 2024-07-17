#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include "../buffer/IBufferManager.hpp"
#include "../connection/IConnectionManager.hpp"
#include "../connection/IRequestHandler.hpp"
#include "../logger/ILogger.hpp"
#include "../network/IServer.hpp"
#include "../pollfd/IPollfdManager.hpp"
#include "IEventManager.hpp"

typedef std::pair<int, std::pair<int, int> > Triplet_t;

class EventManager : public IEventManager
{
private:
    IPollfdManager &m_pollfd_manager;
    IBufferManager &m_buffer_manager;
    IConnectionManager &m_connection_manager;
    IServer &m_server;
    IRequestHandler &m_request_handler;
    ILogger &m_logger;

    // Event handling functions for different types of files
    void m_handleRegularFileEvents(ssize_t &pollfd_index, short events);
    void m_handleServerSocketEvents(ssize_t pollfd_index, short events);
    void m_handleClientSocketEvents(ssize_t &pollfd_index, short events);
    void m_handlePipeEvents(ssize_t &pollfd_index, short events);

    // helper functions
    void m_handleRequest(ssize_t &pollfd_index);
    void m_handleClientException(ssize_t &pollfd_index, short events);
    ssize_t m_flushBuffer(ssize_t &pollfd_index, short options = 0);
    void m_cleanUp(ssize_t &pollfd_index, int descriptor, short options = 0);

public:
    EventManager(IPollfdManager &pollfd_manager, IBufferManager &buffer_manager,
                 IConnectionManager &connection_manager, IServer &server,
                 IRequestHandler &request_handler, ILogger &logger);
    ~EventManager();

    virtual void handleEvents();
};

#endif // EVENTMANAGER_HPP
// Path: includes/core/EventManager.hpp
