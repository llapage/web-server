#include "../../includes/constants/HttpMethodHelper.hpp"
#include "../../includes/exception/WebservExceptions.hpp"

/*
 * HttpMethodHelper.hpp
 *
 * The HttpMethodHelper class provides functionality for working with HTTP
 * methods. It defines an enum HttpMethod to represent standard HTTP methods and
 * provides methods to retrieve string representations of methods and vice
 * versa.
 *
 */

// Constructor initializes member variables using helper functions
HttpMethodHelper::HttpMethodHelper()
    : m_method_list(m_setMethodList()),
      m_supported_methods(m_setSupportedMethods()),
      m_string_http_method_map(m_setStringHttpMethodMap()),
      m_http_method_string_map(m_setHttpMethodStringMap())
{
}

// Destructor
HttpMethodHelper::~HttpMethodHelper() {}

// Get string representation of HttpMethod enum value
const std::string &
HttpMethodHelper::httpMethodStringMap(HttpMethod method) const
{
    if (m_http_method_string_map.find(method) != m_http_method_string_map.end())
    {
        return m_http_method_string_map.at(method);
    }
    else
    {
        throw UnknownMethodError();
    }
}

// Get HttpMethod enum value from string representation
HttpMethod
HttpMethodHelper::stringHttpMethodMap(const std::string &method) const
{
    if (m_string_http_method_map.find(method) != m_string_http_method_map.end())
    {
        return m_string_http_method_map.at(method);
    }
    else
    {
        throw UnknownMethodError(method);
    }
}

// Member function to check if a string is a valid HTTP method
bool HttpMethodHelper::isMethod(const std::string &method) const
{
    return m_string_http_method_map.find(method) !=
           m_string_http_method_map.end();
}

// Member function to check if a string is a supported HTTP method
bool HttpMethodHelper::isSupportedMethod(const std::string &method) const
{
    // Iterate through the vector to find the method
    for (std::vector<std::string>::const_iterator it =
             m_supported_methods.begin();
         it != m_supported_methods.end(); ++it)
    {
        // Compare the current method with the target method
        if (*it == method)
        {
            // Method found, return true
            return true;
        }
    }
    // Method not found, return false
    return false;
}

// Helper function to initialize m_method_list with string representations of
// HTTP methods
std::vector<std::string> HttpMethodHelper::m_setMethodList()
{
    std::vector<std::string> method_list;

    // Add string representations of HTTP methods to method_list
    method_list.push_back("GET");
    method_list.push_back("POST");
    method_list.push_back("PUT");
    method_list.push_back("DELETE");
    method_list.push_back("HEAD");
    method_list.push_back("OPTIONS");
    method_list.push_back("PATCH");
    method_list.push_back("TRACE");
    method_list.push_back("CONNECT");

    return method_list;
}

// Helper function to initialize m_supported_methods with string representations
// of supported HTTP methods
const std::vector<std::string> HttpMethodHelper::m_setSupportedMethods()
{
    std::vector<std::string> supported_methods;

    supported_methods.push_back("GET");
    supported_methods.push_back("POST");
    supported_methods.push_back("DELETE");

    return supported_methods;
}

// Helper function to initialize m_string_http_method_map with mappings from
// string representations to HttpMethod enum values
std::map<std::string, HttpMethod> HttpMethodHelper::m_setStringHttpMethodMap()
{
    std::map<std::string, HttpMethod> string_http_method_map;

    // Add mappings from string representations to HttpMethod enum values
    string_http_method_map[ "GET" ] = GET;
    string_http_method_map[ "POST" ] = POST;
    string_http_method_map[ "PUT" ] = PUT;
    string_http_method_map[ "DELETE" ] = DELETE;
    string_http_method_map[ "HEAD" ] = HEAD;
    string_http_method_map[ "OPTIONS" ] = OPTIONS;
    string_http_method_map[ "PATCH" ] = PATCH;
    string_http_method_map[ "TRACE" ] = TRACE;
    string_http_method_map[ "CONNECT" ] = CONNECT;

    return string_http_method_map;
}

// Helper function to initialize m_http_method_string_map with mappings from
// HttpMethod enum values to string representations
std::map<HttpMethod, std::string> HttpMethodHelper::m_setHttpMethodStringMap()
{
    std::map<HttpMethod, std::string> http_method_string_map;

    // Add mappings from HttpMethod enum values to string representations
    http_method_string_map[ GET ] = "GET";
    http_method_string_map[ POST ] = "POST";
    http_method_string_map[ PUT ] = "PUT";
    http_method_string_map[ DELETE ] = "DELETE";
    http_method_string_map[ HEAD ] = "HEAD";
    http_method_string_map[ OPTIONS ] = "OPTIONS";
    http_method_string_map[ PATCH ] = "PATCH";
    http_method_string_map[ TRACE ] = "TRACE";
    http_method_string_map[ CONNECT ] = "CONNECT";

    return http_method_string_map;
}

// Path: includes/constants/HttpMethodHelper.hpp
