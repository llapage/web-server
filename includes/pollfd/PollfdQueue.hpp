#ifndef POLLFDQUEUE_HPP
#define POLLFDQUEUE_HPP

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
 * PollfdQueue pollfdQueue(10);
 * pollfd new_pollfd = {fd, events, revents};
 * pollfdQueue.push(new_pollfd);
 * pollfdQueue.erase(index);
 * pollfdQueue.pollout(index);
 * if (pollfdQueue.hasReachedCapacity()){}
 *
 * ATTENTION: Performance Disclaimer
 * This library provides no safety mechanisms whatsoever. There are no
 * safeguards, error checks, or validations implemented for performance
 * optimization purposes. Any improper handling of inputs, or errors in usage
 * may result in undefined behavior, crashes, or security vulnerabilities. Use
 * with caution and ensure proper error handling and testing in production code.
 */

#include <poll.h>
#include <unistd.h>
#include <vector>

// PollfdQueue class: Provides a lightweight and efficient container for
// managing pollfd objects.
class PollfdQueue
{
private:
    std::vector<pollfd> m_pollfd_array; // Internal storage for pollfd objects
    size_t m_size;     // Current number of elements in the queue
    size_t m_capacity; // Maximum capacity of the queue
    short m_poll_mask; // Poll mask for polling events;

public:
    // Constructor: Initializes a PollfdQueue object with the specified
    // capacity.
    PollfdQueue(size_t capacity);

    // Destructor: Clears the internal array of pollfd objects when the
    // PollfdQueue object is destroyed.
    ~PollfdQueue();

    // Operator []: Provides access to pollfd objects in the PollfdQueue by
    // index.
    pollfd &operator[](size_t index);

    // Push: Adds a new pollfd object to the PollfdQueue.
    void push(const pollfd &new_pollfd);

    // Erase: Removes the pollfd object at the specified index from the
    // PollfdQueue.
    void erase(size_t index);

    // Pollout: Adds the POLLOUT event to the revents field of the pollfd object
    // at the specified index.
    void pollout(size_t index);

    // HasReachedCapacity: Checks if the PollfdQueue has reached its maximum
    // capacity. Returns true if the size equals the capacity, indicating that
    // no more pollfd objects can be added.
    bool hasReachedCapacity() const;

    // Size: Returns the current number of pollfd objects stored in the
    // PollfdQueue.
    size_t size() const;

    // Capacity: Returns the maximum number of pollfd objects that can be stored
    // in the PollfdQueue.
    size_t capacity() const;

    // data: Returns a pointer to the internal array of pollfd objects.
    pollfd *data();
};

#endif // POLLFDQUEUE_HPP
       // Path: includes/PollfdQueue.hpp
