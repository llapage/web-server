#include "../../includes/pollfd/PollfdQueue.hpp"

/*
 * The PollfdQueue class provides a lightweight and efficient container for
 * managing pollfd objects, tailored with performance in mind. It is designed to
 * store pollfd objects in a fixed-size array, offering constant-time operations
 * for adding and removing elements.
 *
 * Key features:
 *
 * 1. Efficient Memory Usage:
 *    - PollfdQueue uses a fixed-size array of pollfd objects, eliminating
 * dynamic memory allocations and reducing memory fragmentation.
 *
 * 2. Constant-Time Operations:
 *    - Pushing and erasing pollfd objects in PollfdQueue have constant-time
 * complexity (O(1)), ensuring predictable performance even under heavy load.
 *    - When pushing a new pollfd, it is inserted at the current size position,
 * indicated by the size parameter. This avoids the need for reallocations and
 * ensures efficient memory utilization.
 *    - Erasing a pollfd involves copying the last element to the position being
 * removed and decrementing the size parameter. This operation maintains a
 * compact storage layout and avoids shifting elements, contributing to improved
 * cache locality and reduced overhead.
 *
 * 3. Simplified Lookup:
 *    - By focusing solely on index-based access, PollfdQueue reduces overhead
 * and improves cache locality.
 *
 * Usage:
 * - Instantiate PollfdQueue with a specified capacity.
 * - Use push() to add a new pollfd object to the queue.
 * - Use erase() to remove a pollfd object at a specific index.
 * - Access individual pollfd objects using the subscript operator [].
 *
 * Example:
 *
 * PollfdQueue pollfdQueue(1000);
 * pollfd new_pollfd = {fd, events, revents};
 * pollfdQueue.push(new_pollfd);
 * pollfdQueue.erase(index);
 * pollfdQueue.pollout(index); // sets the events field to POLLOUT
 * pollfdQueue.hasReachedCapacity() // check if the queue is full
 *
 * ATTENTION: Performance Disclaimer
 * This library provides no safety mechanisms whatsoever. There are no
 * safeguards, error checks, or validations implemented for performance
 * optimization purposes. Any improper handling of inputs, or errors in usage
 * may result in undefined behavior, crashes, or security vulnerabilities. Use
 * with caution and ensure proper error handling and testing in production code.
 */

// Constructor: Initializes a PollfdQueue object with the specified capacity.
// The size and capacity parameters are set to zero and the specified capacity,
// respectively.
PollfdQueue::PollfdQueue(size_t capacity)
    : m_pollfd_array(capacity), m_size(0), m_capacity(capacity),
      m_poll_mask(POLLOUT | POLLERR | POLLHUP | POLLNVAL)
{
}

// Operator []: Provides access to pollfd objects in the PollfdQueue by index.
pollfd &PollfdQueue::operator[](size_t index)
{
    return m_pollfd_array[ index ];
}

// Destructor: Clears the internal array of pollfd objects when the PollfdQueue
// object is destroyed.
PollfdQueue::~PollfdQueue() { m_pollfd_array.clear(); }

// Push: Adds a new pollfd object to the PollfdQueue.
// The new pollfd object is inserted at the current size position, and the size
// is incremented.
void PollfdQueue::push(const pollfd &new_pollfd)
{
    m_pollfd_array[ m_size ] = new_pollfd;
    m_size++;
}

// Erase: Removes the pollfd object at the specified index from the PollfdQueue.
// The pollfd object at the specified index is replaced with the last pollfd
// object in the queue, and the size is decremented. The erased file descriptor
// is closed.
void PollfdQueue::erase(size_t index)
{
    m_pollfd_array[ index ] = m_pollfd_array[ m_size - 1 ];
    m_size--;
}

// Pollout: Adds POLLOUT to the events field of the pollfd object at the
// specified index.
void PollfdQueue::pollout(size_t index)
{
    m_pollfd_array[ index ].events |= m_poll_mask;
}

// HasReachedCapacity: Checks if the PollfdQueue has reached its maximum
// capacity. Returns true if the size equals the capacity, indicating that no
// more pollfd objects can be added.
bool PollfdQueue::hasReachedCapacity() const { return m_size == m_capacity; }

// Size: Returns the current number of pollfd objects stored in the PollfdQueue.
size_t PollfdQueue::size() const { return m_size; }

// Capacity: Returns the maximum number of pollfd objects that can be stored in
// the PollfdQueue.
size_t PollfdQueue::capacity() const { return m_capacity; }

// data: Returns a pointer to the internal array of pollfd objects.
pollfd *PollfdQueue::data() { return m_pollfd_array.data(); }

// Path: srcs/PollfdQueue.cpp
