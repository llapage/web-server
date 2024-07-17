#ifndef HTTPMETHODHELPER_HPP
#define HTTPMETHODHELPER_HPP

/*
 * HttpMethodHelper.hpp
 *
 * The HttpMethodHelper class provides functionality for working with HTTP
 * methods. It defines an enum HttpMethod to represent standard HTTP methods and
 * provides methods to retrieve string representations of methods and vice
 * versa.
 *
 * Example:
 *
 * HttpMethodHelper helper;
 * std::string method = "GET";
 * HttpMethod httpMethod = helper.stringHttpMethodMap(method);
 * std::string method = helper.httpMethodStringMap(httpMethod);
 *
 * std::string method = "PATCH";
 * if (helper.isMethod(method)) {std::cout << "Valid HTTP method." <<
 * std::endl;}
 *
 * Note: Invalid input to Map searches will throw an UnknownMethodError
 * exception.
 *
 * IMPORTANT: Method strings representations in this class are all UPPERCASE.
 */

#include <map>
#include <string>
#include <vector>

enum HttpMethod
{
    GET = 0,    // The GET method requests a representation of the specified
                // resource.
    POST = 1,   // The POST method is used to submit an entity to the specified
                // resource, often causing a change in state or side effects on
                // the server.
    PUT = 2,    // The PUT method replaces all current representations of the
                // target resource with the request payload.
    DELETE = 3, // The DELETE method deletes the specified resource.
    HEAD = 4, // The HEAD method asks for a response identical to that of a GET
              // request, but without the response body.
    OPTIONS = 5, // The OPTIONS method is used to describe the communication
                 // options for the target resource.
    PATCH = 6,  // The PATCH method applies partial modifications to a resource.
    TRACE = 7,  // The TRACE method performs a message loop-back test along the
                // path to the target resource.
    CONNECT = 8 // The CONNECT method establishes a tunnel to the server
                // identified by the target resource.
};

class HttpMethodHelper
{
private:
    // Member variables
    const std::vector<std::string>
        m_method_list; // List of string representations of HTTP methods
    const std::vector<std::string>
        m_supported_methods; // Set of string representations of HTTP methods
    const std::map<std::string, HttpMethod>
        m_string_http_method_map; // Map of string representations to HttpMethod
                                  // enum values
    const std::map<HttpMethod, std::string>
        m_http_method_string_map; // Map of HttpMethod enum values to string
                                  // representations

    // Private member functions for initialization
    static std::vector<std::string> m_setMethodList();
    const std::vector<std::string> m_setSupportedMethods();
    static std::map<std::string, HttpMethod> m_setStringHttpMethodMap();
    static std::map<HttpMethod, std::string> m_setHttpMethodStringMap();

public:
    // Constructor
    HttpMethodHelper();

    // Destructor
    ~HttpMethodHelper();

    // Member functions to access data
    const std::string &httpMethodStringMap(
        HttpMethod method) const; // Get string representation of HttpMethod
    HttpMethod stringHttpMethodMap(const std::string &method)
        const; // Get HttpMethod enum value from string representation

    // Member function to check if a string is a valid HTTP method
    bool isMethod(const std::string &method)
        const; // Check if a string is a valid HTTP method
    bool isSupportedMethod(const std::string &method)
        const; // Check if a string is a supported HTTP method
};

#endif // HTTPMETHODHELPER_HPP
// Path: includes/constants/HttpMethodHelper.hpp
