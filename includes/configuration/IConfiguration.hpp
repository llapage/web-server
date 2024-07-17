#ifndef IConfiguration_HPP
#define IConfiguration_HPP

/*
 * IConfiguration.hpp
 * Abstract base class for Configuration in webserv
 *
 * This file defines the Configuration interface, which serves as the abstract
 * base class for the Configuration in webserv. IConfiguration allows for
 * polymorphic behavior and dependency injection, thereby enabling us to create
 * a MockConfigurationBlock class for isolated unit testing.
 *
 */

#include <string>
#include <vector>

class BlockList;

class IConfiguration
{
public:
    virtual ~IConfiguration() {}

    const virtual BlockList &getBlocks(const std::string &key) = 0;
    virtual const std::vector<std::string> &
    getStringVector(const std::string &) const = 0;
    virtual const std::string &getString(const std::string &parameter,
                                         size_t index = 0) const = 0;
    virtual int getInt(const std::string &parameter,
                       size_t index = 0) const = 0;
    virtual size_t getSize_t(const std::string &, size_t index = 0) const = 0;
    virtual bool getBool(const std::string &parameter,
                         size_t index = 0) const = 0;
    virtual void print(size_t depth) const = 0;
    virtual const std::string &getName() const = 0;
    virtual bool isRegex(void) const = 0;
    virtual std::vector<std::string> &getParameters(void) = 0;
};

#endif // IConfiguration_HPP
// Path: includes/configuration/IConfiguration.hpp
