#ifndef BUFFERMANAGER_HPP
#define BUFFERMANAGER_HPP

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

#define DEFAULT_FLUSH_THRESHOLD 32768 // 32 KB

#include "IBuffer.hpp"
#include "IBufferManager.hpp"
#include <map>

// Forward declarations
class ISocket;
class FileBuffer;
class SocketBuffer;

class BufferManager : public IBufferManager
{
private:
    std::map<int, IBuffer *> m_buffers;
    size_t m_flush_threshold;
    ISocket &m_socket;

public:
    // Constructor
    BufferManager(ISocket &socket);

    // Destructor
    ~BufferManager();

    // Push into a file buffer
    ssize_t pushFileBuffer(int file_descriptor, const std::vector<char> &data,
                           size_t flush_threshold = DEFAULT_FLUSH_THRESHOLD);

    // Push into a socket buffer
    ssize_t pushSocketBuffer(int socket_descriptor,
                             const std::vector<char> &data);

    // Flush the buffer for a specific descriptor
    ssize_t flushBuffer(int descriptor, bool blocking = false);

    // Flush all buffers
    void flushBuffers();

    // Destroy the buffer for a specific descriptor
    void destroyBuffer(int descriptor);

    // Peek at the buffer for a specific descriptor
    std::vector<char> peekBuffer(int descriptor) const;

    // Set the flush threshold
    void setFlushThreshold(size_t threshold);

    // Transfer the buffer from one descriptor to another
    void transferBuffer(int source_descriptor, int target_descriptor);
};

#endif // BUFFERMANAGER_HPP

// Path: includes/buffer/BufferManager.hpp
