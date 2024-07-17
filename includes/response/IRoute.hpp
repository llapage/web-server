#ifndef IROUTE_HPP
#define IROUTE_HPP

#include "../constants/HttpMethodHelper.hpp"
#include <string>

class IResponseGenerator;

class IRoute
{
public:
    virtual ~IRoute() {};

    /* ROUTER

    virtual std::string	getUri() const = 0;
    virtual void setUri(std::string new_uri) = 0;
    virtual void appendUri(const std::string &new_string) = 0;

        // HostName
    virtual std::string getHostname() const = 0;
        virtual void setHostname(std::string new_hostname) = 0;

        // Port
        virtual std::string getPort() const = 0;
        virtual void setPort(std::string new_port) = 0;

        // Method
    virtual std::vector<HttpMethod> getMethod() const = 0;
    virtual void setMethod(const std::vector<std::string> new_method, HttpHelper
    &http_helper) = 0;

        // Root
        virtual std::string getRoot() const = 0;
        virtual void setRoot(std::string new_root) = 0;

    // index
    virtual std::string getIndex() const = 0;
    virtual void setIndex(std::string new_index) = 0;

        // Prefix
        virtual std::string getPath() const = 0;
        virtual void setPath(std::string new_path) = 0;

        // Regex flag
    virtual bool getRegex() const = 0;
    virtual void setRegex(bool new_regex) = 0;

        // getResponseGenerator
        virtual IResponseGenerator *getResponseGenerator(void) const = 0;

    END ROUTER*/

    virtual std::string getRoot() const = 0;
    virtual std::string getPath() const = 0;
    virtual std::string getIndex() const = 0;
    virtual std::string getCgiScript() const = 0;
    virtual size_t getClientMaxBodySize() const = 0;
    virtual bool isAllowedMethod(const HttpMethod method) const = 0;
    virtual bool isRegex() const = 0;
    virtual bool isCGI() const = 0;
    virtual bool isRedirect(const std::string &uri) const = 0;
    virtual bool autoindex() const = 0;
    virtual std::string getRedirect(const std::string &uri) const = 0;
    virtual IResponseGenerator *getResponseGenerator() const = 0;
    virtual void setResponseGenerator(IResponseGenerator *generator) = 0;
    virtual bool match(const std::string &uri) = 0;
};

#endif // IROUTE_HPP
       // Path: includes/response/IRoute.hpp
