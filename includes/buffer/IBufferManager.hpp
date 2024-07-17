#ifndef IBUFFERMANAGER_HPP
#define IBUFFERMANAGER_HPP

#include <sys/types.h>
#include <vector>

class IBufferManager
{
public:
    virtual ~IBufferManager() {};

    // methods for managing buffers
    virtual ssize_t pushFileBuffer(int, const std::vector<char> &,
                                   size_t = 32500) = 0;
    virtual ssize_t pushSocketBuffer(int, const std::vector<char> &) = 0;
    virtual ssize_t flushBuffer(int, bool = false) = 0;
    virtual void flushBuffers() = 0;
    virtual void destroyBuffer(int) = 0;
    virtual std::vector<char> peekBuffer(int) const = 0;
    virtual void setFlushThreshold(size_t) = 0;
    virtual void transferBuffer(int, int) = 0;
};

#endif // IBUFFERMANAGER_HPP
// Path: includes/buffer/IBufferManager.hpp
