#include "../../includes/buffer/BufferManager.hpp"
#include "../../includes/buffer/FileBuffer.hpp"
#include "../../includes/buffer/SocketBuffer.hpp"

/*
 * BufferManager Class
 *
 * The BufferManager serves as a centralized mechanism for collecting write and
 * send buffers generated throughout the system.
 *
 * Purpose:
 *   To not block the main thread when sending responses or writing to files.
 *
 * How it Works:
 * - Logging Messages: Various components within the system generate log
 * messages during their operation and register them with the BufferManager.
 * - Sending Responses: The RequestHandler generates responses to client
 * requests and registers them with the BufferManager.
 * - Deferred Writing: Log messages and client responses remain in the buffer
 * until it's time to write them to the output medium, ensuring non-blocking
 * behavior.
 * - Batch Processing: Log messages are processed in batches rather than
 * individually to improve efficiency.
 */

// Constructor
BufferManager::BufferManager(ISocket &socket)
    : m_flush_threshold(DEFAULT_FLUSH_THRESHOLD), m_socket(socket)
{
}

// Destructor
BufferManager::~BufferManager()
{
    // Clean up all buffers
    std::map<int, IBuffer *>::iterator it = m_buffers.begin();
    while (it != m_buffers.end())
    {
        // Flush remaining data in each buffer and then destroy it
        while (it->second->flush(it->first, true) >
               0) // flush remaining data blockingly (true)
            ;

        // Duplicate itertor to avoid invalidating it
        std::map<int, IBuffer *>::iterator it2 = it;

        // Increment iterator before destroying buffer
        it++;

        // Destroy buffer
        this->destroyBuffer(it2->first);
    }
}

// Push a file buffer into the manager
ssize_t BufferManager::pushFileBuffer(int file_descriptor,
                                      const std::vector<char> &data,
                                      size_t flush_threshold)
{
    // If the buffer for this file descriptor doesn't exist, create it
    if (m_buffers.find(file_descriptor) == m_buffers.end())
    {
        m_buffers[ file_descriptor ] = new FileBuffer(flush_threshold);
    }
    // Push data into the file buffer
    return m_buffers[ file_descriptor ]->push(
        data); // returns 1 if a flush is requested
}

// Push a socket buffer into the manager
ssize_t BufferManager::pushSocketBuffer(int socket_descriptor,
                                        const std::vector<char> &data)
{
    // If the buffer for this socket descriptor doesn't exist, create it
    if (m_buffers.find(socket_descriptor) == m_buffers.end())
    {
        m_buffers[ socket_descriptor ] = new SocketBuffer(m_socket);
    }
    // Push data into the socket buffer
    return m_buffers[ socket_descriptor ]->push(
        data); // returns the number of bytes pushed
}

// Flush the buffer for a specific descriptor
// Returns bytes remaining in buffer, or -1 in case of error
ssize_t BufferManager::flushBuffer(int descriptor, bool blocking)
{
    if (m_buffers.find(descriptor) != m_buffers.end())
    {
        // Flush the buffer and get the number of remaining bytes
        ssize_t remaining_bytes =
            m_buffers[ descriptor ]->flush(descriptor, blocking);

        // If the buffer is completely flushed, destroy it
        if (remaining_bytes == 0)
            this->destroyBuffer(descriptor);

        // Return the number of remaining bytes
        return remaining_bytes;
    }
    return -1;
}

// Flush all buffers
void BufferManager::flushBuffers()
{
    std::map<int, IBuffer *>::iterator it;
    for (it = m_buffers.begin(); it != m_buffers.end(); it++)
    {
        // Flush each buffer
        this->flushBuffer(it->first);
    }
}

// Destroy the buffer for a specific descriptor
void BufferManager::destroyBuffer(int descriptor)
{
    if (m_buffers.find(descriptor) != m_buffers.end())
    {
        // Delete the buffer and remove it from the map
        delete m_buffers[ descriptor ];
        m_buffers.erase(descriptor);
    }
}

// Peek at the buffer for a specific descriptor
std::vector<char> BufferManager::peekBuffer(int descriptor) const
{
    if (m_buffers.find(descriptor) != m_buffers.end())
    {
        return m_buffers.at(descriptor)->peek();
    }
    return std::vector<char>();
}

// Set the flush threshold
void BufferManager::setFlushThreshold(size_t threshold)
{
    m_flush_threshold = threshold;
}

// Transfer the buffer from one descriptor to another
void BufferManager::transferBuffer(int from_descriptor, int to_descriptor)
{
    if (m_buffers.find(from_descriptor) != m_buffers.end())
    {
        // Transfer the buffer to the new descriptor
        m_buffers[ to_descriptor ] = m_buffers[ from_descriptor ];
        m_buffers.erase(from_descriptor);
    }
}

// Path: srcs/FileBuffer.cpp
