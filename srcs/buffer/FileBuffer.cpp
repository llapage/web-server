#include "../../includes/buffer/FileBuffer.hpp"

/*
 * FileBuffer.cpp
 *
 * Holds buffers intended for file descriptors.
 *
 */

// Constructor
FileBuffer::FileBuffer(size_t flush_threshold, size_t max_size)
    : m_flush_threshold(flush_threshold), m_max_size(max_size)
{
    // Reserve memory for the buffer based on the flush threshold
    m_buffer.reserve(flush_threshold);
}

// Destructor
FileBuffer::~FileBuffer()
{
    // Clear the buffer
    m_buffer.clear();
}

// Push data into the buffer
// Returns -1 if the max size is reached, 1 if the flush threshold is reached,
// or 0 otherwise
ssize_t FileBuffer::push(const std::vector<char> &data)
{
    // Check if the absolute max size of the buffer is reached
    if (m_buffer.size() + data.size() > m_max_size)
    {
        return -1; // Buffer full, cannot push more data
    }

    // Append the data to the buffer
    m_buffer.insert(m_buffer.end(), data.begin(), data.end());

    // Return 1 to request a flush if the buffer size exceeds the flush
    // threshold Otherwise, return 0
    return (m_buffer.size() > m_flush_threshold);
}

// Flush the buffer to the file descriptor
// Returns the remaining size of the buffer (or -1 in case of error)
ssize_t FileBuffer::flush(int file_descriptor, bool regardless_of_threshold)
{
    // Check if the buffer size is less than the flush threshold
    if (regardless_of_threshold == false && m_buffer.size() < m_flush_threshold)
    {
        return m_buffer.size(); // Not enough data to flush, do nothing, just
                                // return the remaining size of the buffer
    }

    // Write the buffer to the file descriptor
    ssize_t bytes_written =
        ::write(file_descriptor, m_buffer.data(), m_buffer.size());

    if (bytes_written == -1)
    {
        return -1; // Error writing to file descriptor
    }
    else
    {
        // Update buffer state after successful write
        size_t bytes_remaining = m_buffer.size() - bytes_written;
        // Shift the remaining data to the beginning of the buffer
        memmove(&m_buffer[ 0 ], &m_buffer[ bytes_written ], bytes_remaining);
        m_buffer.resize(bytes_remaining);
    }
    return m_buffer.size(); // Return the remaining size of the buffer
}

// Peek at the buffer
std::vector<char> FileBuffer::peek() const
{
    return m_buffer; // Return a copy of the buffer
}

// Path: includes/IBuffer.hpp
