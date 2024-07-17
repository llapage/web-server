#ifndef ISOCKET_HPP
#define ISOCKET_HPP

/*
 * ISocket - Interface for socket operations.
 *
 * This interface defines methods for socket-related operations such as
 * creating a socket, binding to an address, listening for connections,
 * accepting connections, sending and receiving data, polling for events,
 * controlling socket file descriptor flags, and closing the socket.
 */

#include <string>
#include <sys/socket.h>
#include <vector>

// Interface for socket operations
class ISocket
{
public:
    virtual ~ISocket() {};

    // Creates a socket
    virtual int socket() const = 0;

    // Binds the socket to an IP address and port
    virtual int bind(int socket_descriptor, int ip, int port) const = 0;

    // Listens for incoming connections
    virtual int listen(int fd, int max_connections) const = 0;

    // Performs file control operations on a socket
    virtual int fcntl(int fd, int cmd, int arg) const = 0;

    // Sets the socket to non-blocking mode
    virtual int setNonBlocking(int fd) const = 0;

    // Accepts an incoming connection
    virtual std::pair<int, std::pair<std::string, std::string> >
    accept(int fd) const = 0;

    // Sends data over the socket
    virtual int send(int recipient_socket_fd,
                     const std::vector<char> &data) const = 0;

    // Sends data over the socket blocking until all data is sent
    virtual int sendAll(int recipient_socket_fd,
                        const std::vector<char> &data) const = 0;

    // Receives data from the socket
    virtual ssize_t recv(int socket_descriptor, char *buffer,
                         size_t len) const = 0;

    // Forcibly bind a socket to a port in use
    virtual int setReuseAddr(int fd) const = 0;
};

#endif // ISOCKET_HPP
// Path: includes/Server.hpp
