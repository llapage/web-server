#ifndef SOCKETBUFFER_HPP
#define SOCKETBUFFER_HPP

/*
 * SocketBuffer.hpp
 *
 * Holds buffers intended for socket descriptors.
 */

#include "../network/ISocket.hpp"
#include "IBuffer.hpp"
#include <cstring>
#include <vector>

class SocketBuffer : public IBuffer
{
private:
    std::vector<char> m_buffer; // Buffer to hold data
    ISocket &m_socket;          // Socket object for sending data

public:
    // Constructor
    SocketBuffer(ISocket &socket);

    // Destructor
    ~SocketBuffer();

    // Push data into the buffer
    ssize_t push(const std::vector<char> &data);

    // Send the buffer to a socket descriptor
    ssize_t flush(int socket_descriptor, bool blocking = false);

    // Peek at the buffer
    std::vector<char> peek() const;
};

#endif // SOCKETBUFFER_HPP

// Path: includes/buffer/SocketBuffer.hpp
