#include "../../includes/network/Socket.hpp"
#include <cerrno>
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>

/*
 * Socket - Class for socket operations.
 *
 * This class provides methods for socket-related operations such as
 * creating a socket, binding to an address, listening for connections,
 * accepting connections, sending and receiving data,
 * controlling socket file descriptor flags, and closing the socket.
 */

// Destructor
Socket::~Socket() {}

// Creates a IPv4 TCP socket
int Socket::socket() const
{
    // return the file descriptor for the socket
    // -1 is returned on error
    // domain: communication domain (AF_INET for IPv4)
    // type: communication semantics (SOCK_STREAM for TCP)
    // protocol: protocol to be used with the socket (0 for default protocol)
    return ::socket(AF_INET, SOCK_STREAM, 0);
}

// Binds the socket to an address
int Socket::bind(int socket_descriptor, int ip, int port) const
{
    // return the result of binding the socket to the address
    // 0 is returned on success, -1 on error
    // socket_descriptor: file descriptor of the socket
    // ip: IP address to bind to
    // port: port number to bind to

    if (port < 0 || port > 65535)
    {
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (ip == 0)
    {
        addr.sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        addr.sin_addr.s_addr = htonl(ip);
    }
    int return_value =
        ::bind(socket_descriptor, (struct sockaddr *)&addr, sizeof(addr));
    if (return_value == -1)
    {
        std::cerr << "FATAL Error binding socket to address" << std::endl;
        std::cerr << "Error code: " << errno << std::endl;
    }
    return return_value;
}

// Listens for incoming connections
int Socket::listen(int socket_descriptor, int max_connections) const
{
    // listen for incoming connections on the socket
    // max_connections: maximum number of pending connections in the socket's
    // listen queue
    return ::listen(socket_descriptor, max_connections);
}

// Performs file control operations on a socket
int Socket::fcntl(int socket_descriptor, int cmd, int arg) const
{
    // perform file control operations on the socket
    // cmd: operation to be performed
    // arg: argument to the operation
    return ::fcntl(socket_descriptor, cmd, arg);
}

// Sets the socket to non-blocking mode
int Socket::setNonBlocking(int socket_descriptor) const
{
    // Add non-blocking mode to the current flags of the socket
    // socket_descriptor: file descriptor of the socket
    // Returns: 0 on success, -1 on error
    int flags = ::fcntl(socket_descriptor, F_GETFL, 0);
    if (flags == -1)
    {
        return -1;
    }
    return ::fcntl(socket_descriptor, F_SETFL, flags | O_NONBLOCK);
}

// Accept an incoming connection
std::pair<int, std::pair<std::string, std::string> >
Socket::accept(int server_socket_fd) const
{
    // server_socket_fd: file descriptor of the listening socket
    // Returns: file descriptor for the new client socket (int) (-1 on error)
    //          client's IP address (std::string)
    //          client's port number (std::string)

    // Address structure for the client
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Accept the incoming connection and get the client socket descriptor
    int client_socket_fd = ::accept(
        server_socket_fd, (struct sockaddr *)&client_addr, &client_addr_len);

    // Check for errors
    if (client_socket_fd == -1)
    {
        return std::make_pair(-1, std::make_pair("", ""));
    }

    // Convert the binary IP address to a string
    std::string client_ip = inet_ntoa(client_addr.sin_addr);

    // Retrieve the port number and convert it to host byte order
    uint16_t client_port_uint_16 = ntohs(client_addr.sin_port);

    // Convert the port number to a string
    std::ostringstream ss;
    ss << client_port_uint_16;
    std::string client_port = ss.str();

    return std::make_pair(client_socket_fd,
                          std::make_pair(client_ip, client_port));
}

// Sends data over the socket Non-Blockingly
int Socket::send(int socket_descriptor, const std::vector<char> &data) const
{
    // Send data over the socket
    // socket_descriptor: File descriptor of the socket
    // data: Data to be sent
    // Returns the number of bytes sent
    // -1 is returned on error
    return ::send(socket_descriptor, data.data(), data.size(),
                  MSG_DONTWAIT | MSG_NOSIGNAL);
}

// Sends data over the socket blocking until all data is sent
int Socket::sendAll(int socket_descriptor, const std::vector<char> &data) const
{
    // Send data over the socket blocking until all data is sent
    // socket_descriptor: File descriptor of the socket
    // data: Data to be sent
    // Returns the number of bytes sent
    // -1 is returned on error
    return ::send(socket_descriptor, data.data(), data.size(), MSG_NOSIGNAL);
}

// Receives data from the socket Non-Blockingly
ssize_t Socket::recv(int socket_descriptor, char *buffer, size_t len) const
{
    // receive data from the socket
    // socket_descriptor: file descriptor of the socket
    // returns the received data
    // an empty vector is returned on error
    return ::recv(socket_descriptor, buffer, len, MSG_DONTWAIT);
}

// Forcibly bind a socket to a port in use
int Socket::setReuseAddr(int fd) const
{
    int optval = 1;
    return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}

// Path: srcs/network/socket.cpp
