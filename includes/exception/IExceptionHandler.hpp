#ifndef IEXCEPTIONHANDLER_HPP
#define IEXCEPTIONHANDLER_HPP

/*
 * IExceptionHandler.hpp
 * Abstract base class for ExceptionHandler in webserv
 *
 * This file defines the ExceptionHandler interface, which serves as the
 * abstract base class for the ExceptionHandler in webserv. IExceptionHandler
 * allows for polymorphic behavior and dependency injection, thereby enabling us
 * to create a MockExceptionHandler class for isolated unit testing.
 *
 */

#include <exception>
#include <string>

class IExceptionHandler
{
public:
    virtual ~IExceptionHandler() {}

    // Method to handle exceptions
    virtual int handleException(const std::exception &e,
                                const std::string &context = "") const = 0;
};

#endif // IEXCEPTIONHANDLER_HPP
       // Path: includes/exeption/IExceptionHandler.hpp
