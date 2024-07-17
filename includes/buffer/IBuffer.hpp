#ifndef IBUFFER_HPP
#define IBUFFER_HPP

/*
 * IBuffer.hpp
 * Abstract base class for Buffer in webserv
 *
 * This file defines the Buffer interface, which serves as the abstract base
 * class for the Buffer in webserv.
 * IBuffer allows for polymorphic behavior and dependency injection,
 * thereby enabling us to create a MockBuffer class for isolated unit testing.
 *
 */

#include <unistd.h>
#include <vector>

class IBuffer
{
public:
    virtual ~IBuffer() {};

    // Buffer methods
    virtual ssize_t
    push(const std::vector<char> &) = 0; // Method to append a vector of
                                         // characters to the buffer
    virtual ssize_t flush(int, bool = false) = 0; // Method to flush the buffer
    virtual std::vector<char> peek() const = 0; // Method to peek at the buffer
};

#endif // IBUFFER_HPP
// Path: includes/buffer/IBuffer.hpp
