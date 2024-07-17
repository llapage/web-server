#include "../../includes/constants/HttpHelper.hpp"

/*
 * HttpHelper.hpp
 *
 * The HttpHelper class provides functionality for working with HTTP request
 * components, including methods, versions, and headers. It encapsulates
 * instances of helper classes for HTTP methods, versions, and headers,
 * providing convenient access to their functionality.
 *
 * Path: includes/constants/HttpHelper.hpp
 */

// Constructors to initialize helper classes
HttpHelper::HttpHelper()
    : m_method_helper(), m_version_helper(), m_header_helper(),
      m_status_code_helper()
{
}

HttpHelper::HttpHelper(const IConfiguration &configuration)
    : m_method_helper(), m_version_helper(), m_header_helper(),
      m_status_code_helper(configuration.getStringVector("error_page"))
{
}

// Get string representation of HttpMethod enum value
const std::string &HttpHelper::httpMethodStringMap(HttpMethod method) const
{
    return m_method_helper.httpMethodStringMap(method);
}

// Get HttpMethod enum value from string representation
HttpMethod HttpHelper::stringHttpMethodMap(const std::string &method) const
{
    return m_method_helper.stringHttpMethodMap(method);
}

// Check if a string is a valid HTTP method
bool HttpHelper::isMethod(const std::string &method) const
{
    return m_method_helper.isMethod(method);
}

// Check if a string is a supported HTTP method
bool HttpHelper::isSupportedMethod(const std::string &method) const
{
    return m_method_helper.isSupportedMethod(method);
}

// Get string representation of HttpVersion enum value
const std::string &HttpHelper::httpVersionStringMap(HttpVersion version) const
{
    return m_version_helper.httpVersionStringMap(version);
}

// Get HttpVersion enum value from string representation
HttpVersion HttpHelper::stringHttpVersionMap(const std::string &version) const
{
    return m_version_helper.stringHttpVersionMap(version);
}

// Check if a string represents a valid HTTP version
bool HttpHelper::isHttpVersion(const std::string &version) const
{
    return m_version_helper.isHttpVersion(version);
}

// Get string representation of HttpHeader enum value
const std::string &HttpHelper::httpHeaderStringMap(HttpHeader header) const
{
    return m_header_helper.httpHeaderStringMap(header);
}

// Get HttpHeader enum value from string representation
HttpHeader HttpHelper::stringHttpHeaderMap(const std::string &header) const
{
    return m_header_helper.stringHttpHeaderMap(header);
}

// Check if a string represents a valid HTTP header
bool HttpHelper::isHeaderName(const std::string &header) const
{
    return m_header_helper.isHeaderName(header);
}

// Get string representation of HttpStatusCode enum value
const std::string &
HttpHelper::httpStatusCodeStringMap(HttpStatusCode statusCode) const
{
    return m_status_code_helper.httpStatusCodeStringMap(statusCode);
}

// Get HttpStatusCode enum value from string representation
HttpStatusCode
HttpHelper::stringHttpStatusCodeMap(const std::string &statusCode) const
{
    return m_status_code_helper.stringHttpStatusCodeMap(statusCode);
}

// Get HttpStatusCode enum value from integer representation
HttpStatusCode HttpHelper::intHttpStatusCodeMap(int statusCode) const
{
    return m_status_code_helper.intHttpStatusCodeMap(statusCode);
}

// Generate a status line with the specified HTTP status code
std::string HttpHelper::getStatusLine(HttpStatusCode statusCode) const
{
    return m_status_code_helper.getStatusLine(statusCode);
}

// Generate an HTML page with the specified HTTP status code
std::string HttpHelper::getHtmlPage(HttpStatusCode statusCode) const
{
    return m_status_code_helper.getHtmlPage(statusCode);
}

// Generate an HTML page with the specified HTTP status code
std::string HttpHelper::getErrorResponse(HttpStatusCode statusCode) const
{
    return m_status_code_helper.getErrorResponse(statusCode);
}

// Path: srcs/constants/HttpHeaderHelper.cpp
