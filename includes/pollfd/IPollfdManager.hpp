#ifndef IPOLLFDMANAGER_HPP
#define IPOLLFDMANAGER_HPP

/*
 * IPollfdManager.hpp
 * Abstract base class for PollfdManager in webserv
 *
 * This file defines the PollfdManager interface, which serves as the abstract
 * base class for the PollfdManager in webserv. IPollfdManager allows for
 * polymorphic behavior and dependency injection, thereby enabling us to create
 * a MockPollfdManager class for isolated unit testing.
 *
 */

#include <cstddef>
#include <poll.h>

enum DescriptorType
{
    REGULAR_FILE = 0x0,
    SERVER_SOCKET = 0x40,
    CLIENT_SOCKET = 0x80,
    PIPE = 0xC0,
};

class IPollfdManager
{
public:
    virtual ~IPollfdManager() {}

    // Method to add a regular file pollfd to the pollfdQueue
    virtual void addRegularFilePollfd(pollfd pollfd) = 0;
    virtual void addBodyFilePollfd(pollfd pollfd) = 0;
    virtual bool isBodyFile(int position) = 0;

    // Method to add a server socket pollfd to the pollfdQueue
    virtual void addServerSocketPollfd(pollfd pollfd) = 0;

    // Method to add a client socket pollfd to the pollfdQueue
    virtual void addClientSocketPollfd(pollfd pollfd) = 0;

    // Method to add a pipe pollfd to the pollfdQueue
    virtual void addPipePollfd(pollfd pollfd) = 0;

    // Method to remove a file descriptor from the pollfdQueue
    virtual void removePollfd(int position) = 0;

    // Method to add the POLLOUT event for a specific position in the
    // pollfdQueue
    virtual void addPollOut(int position) = 0;

    // Method to close all file descriptors in the pollfdQueue
    virtual void closeAllFileDescriptors() = 0;

    // Method to get the size of the pollfdQueue
    virtual size_t getPollfdQueueSize() const = 0;

    // Method to get the events at a specific position in the pollfdQueue
    virtual short getEvents(int position) = 0;

    // Method to get the file descriptor at a specific position in the
    // pollfdQueue
    virtual int getDescriptor(int position) = 0;

    // Method to check if the pollfdQueue has reached its capacity
    virtual bool hasReachedCapacity() const = 0;

    // Method to get a pointer to the pollfd array
    virtual pollfd *getPollfdArray() = 0;

    // Method to get the current pollfd queue index given a file descriptor
    virtual int getPollfdQueueIndex(int fd) = 0;
};

#endif // IPOLLFDMANAGER_HPP
// Path: includes/IPollfdManager.hpp
