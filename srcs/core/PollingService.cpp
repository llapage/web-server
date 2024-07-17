#include "../../includes/core/PollingService.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include "../../includes/utils/Converter.hpp"
#include <cerrno>

PollingService::PollingService(IPollfdManager &pollfd_manager, ILogger &logger,
                               int timeout)
    : m_pollfd_manager(pollfd_manager), m_logger(logger), m_timeout(timeout)
{
}

PollingService::~PollingService() {}

void PollingService::pollEvents()
{
    // Get the pollfd array
    pollfd *pollfd_array = m_pollfd_manager.getPollfdArray();

    // Get pollfd queue size
    size_t pollfd_queue_size = m_pollfd_manager.getPollfdQueueSize();

    // Poll registered file descriptors for events
    int poll_result = ::poll(pollfd_array, pollfd_queue_size, m_timeout);
    if (poll_result < 0)
    {
        if (errno == EINTR)
            m_logger.log(VERBOSE,
                         "[POLLINGSERVICE] Poll interrupted by signal");
        throw PollError();
    }

    // Log poll result
    if (poll_result == 0) // Timeout occurred
        m_logger.log(EXHAUSTIVE,
                     "[POLLINGSERVICE] Poll returned after timeout (0 events)");
    else // Events occurred
        m_logger.log(VERBOSE, "[POLLINGSERVICE] Poll returned " +
                                  Converter::toString(poll_result) +
                                  " events.");
}

void PollingService::setPollingTimeout(int timeout) { m_timeout = timeout; }
