#ifndef REQUESTHELPER_HPP
#define REQUESTHELPER_HPP

/*
 * HttpHelper.hpp
 *
 * The HttpHelper class provides functionality for working with HTTP request
 * components, including methods, versions, headers and status codes. It
 * encapsulates instances of helper classes for HTTP methods, versions, headers
 * and status codes, providing convenient access to their functionality.
 *
 * Path: includes/constants/HttpHelper.hpp
 */

#include "../configuration/IConfiguration.hpp"
#include "HttpHeaderHelper.hpp"
#include "HttpMethodHelper.hpp"
#include "HttpStatusCodeHelper.hpp"
#include "HttpVersionHelper.hpp"
#include <string>

class HttpHelper
{
private:
    // Private member variables for helper classes
    HttpMethodHelper m_method_helper;          // Helper for HTTP methods
    HttpVersionHelper m_version_helper;        // Helper for HTTP versions
    HttpHeaderHelper m_header_helper;          // Helper for HTTP headers
    HttpStatusCodeHelper m_status_code_helper; // Helper for HTTP status codes

public:
    // Constructors to initialize helper classes
    HttpHelper(); // Default constructor
    HttpHelper(const IConfiguration &configuration);

    // Http Method Helper Functions
    const std::string &httpMethodStringMap(
        HttpMethod method) const; // Get string representation of HttpMethod
    HttpMethod stringHttpMethodMap(const std::string &method)
        const; // Get HttpMethod enum value from string representation
    bool isMethod(const std::string &method)
        const; // Check if a string is a valid HTTP method
    bool isSupportedMethod(const std::string &method)
        const; // Check if a string is a supported HTTP method

    // Http Version Helper Functions
    const std::string &httpVersionStringMap(
        HttpVersion version) const; // Get string representation of HttpVersion
    HttpVersion stringHttpVersionMap(const std::string &version)
        const; // Get HttpVersion enum value from string representation
    bool isHttpVersion(const std::string &version)
        const; // Check if a string represents a valid HTTP version

    // Http Header Helper Functions
    const std::string &httpHeaderStringMap(
        HttpHeader header) const; // Get string representation of HttpHeader
    HttpHeader stringHttpHeaderMap(const std::string &header)
        const; // Get HttpHeader enum value from string representation
    bool isHeaderName(const std::string &header)
        const; // Check if a string represents a valid HTTP header

    // Http Status Code Helper Functions
    const std::string &httpStatusCodeStringMap(HttpStatusCode statusCode)
        const; // Get string representation of HttpStatusCode
    HttpStatusCode stringHttpStatusCodeMap(const std::string &statusCode)
        const; // Get HttpStatusCode enum value from string representation
    HttpStatusCode intHttpStatusCodeMap(int statusCode)
        const; // Get HttpStatusCode enum value from integer representation
    std::string getStatusLine(HttpStatusCode statusCode)
        const; // Generate a status line with the specified HTTP status code
    std::string getHtmlPage(HttpStatusCode statusCode)
        const; // Generate an HTML page with the specified HTTP status code
    std::string getErrorResponse(HttpStatusCode statusCode)
        const; // Generate an HTML page with the specified HTTP status code
};

#endif // REQUESTHELPER_HPP
       // Path: includes/constants/HttpHelper.hpp
