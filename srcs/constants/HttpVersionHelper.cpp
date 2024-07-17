#include "../../includes/constants/HttpVersionHelper.hpp"
#include "../../includes/exception/WebservExceptions.hpp"

/*
 * HttpVersionHelper.hpp
 *
 * The HttpVersionHelper class provides functionality for working with HTTP
 * versions. It defines an enum HttpVersion to represent standard HTTP versions
 * and provides methods to retrieve string representations of versions and vice
 * versa.
 *
 */

// Constructor initializes member variables using helper functions
HttpVersionHelper::HttpVersionHelper()
    : m_version_list(m_setVersionList()),
      m_string_http_version_map(m_setStringHttpVersionMap()),
      m_http_version_string_map(m_setHttpVersionStringMap())
{
}

// Get string representation of HttpVersion enum value
const std::string &
HttpVersionHelper::httpVersionStringMap(HttpVersion version) const
{
    if (m_http_version_string_map.find(version) !=
        m_http_version_string_map.end())
    {
        return m_http_version_string_map.at(version);
    }
    else
    {
        throw UnknownHttpVersionError();
    }
}

// Get HttpVersion enum value from string representation
HttpVersion
HttpVersionHelper::stringHttpVersionMap(const std::string &version) const
{
    if (m_string_http_version_map.find(version) !=
        m_string_http_version_map.end())
    {
        return m_string_http_version_map.at(version);
    }
    else
    {
        throw UnknownHttpVersionError(version);
    }
}

// Member function to check if a string is a valid HTTP version
bool HttpVersionHelper::isHttpVersion(const std::string &version) const
{
    return m_string_http_version_map.find(version) !=
           m_string_http_version_map.end();
}

// Helper function to initialize m_version_list with string representations of
// HTTP versions
std::vector<std::string> HttpVersionHelper::m_setVersionList()
{
    std::vector<std::string> version_list;

    // Add string representations of HTTP versions to version_list
    version_list.push_back("HTTP/0.9");
    version_list.push_back("HTTP/1.0");
    version_list.push_back("HTTP/1.1");
    version_list.push_back("HTTP/2.0");
    version_list.push_back("HTTP/3.0");

    return version_list;
}

// Helper function to initialize m_string_http_version_map with mappings from
// string representations to HttpVersion enum values
std::map<std::string, HttpVersion>
HttpVersionHelper::m_setStringHttpVersionMap()
{
    std::map<std::string, HttpVersion> string_http_version_map;

    // Add mappings from string representations to HttpVersion enum values
    string_http_version_map[ "HTTP/0.9" ] = HTTP_0_9;
    string_http_version_map[ "HTTP/1.0" ] = HTTP_1_0;
    string_http_version_map[ "HTTP/1.1" ] = HTTP_1_1;
    string_http_version_map[ "HTTP/2.0" ] = HTTP_2_0;
    string_http_version_map[ "HTTP/3.0" ] = HTTP_3_0;

    return string_http_version_map;
}

// Helper function to initialize m_http_version_string_map with mappings from
// HttpVersion enum values to string representations
std::map<HttpVersion, std::string>
HttpVersionHelper::m_setHttpVersionStringMap()
{
    std::map<HttpVersion, std::string> http_version_string_map;

    // Add mappings from HttpVersion enum values to string representations
    http_version_string_map[ HTTP_0_9 ] = "HTTP/0.9";
    http_version_string_map[ HTTP_1_0 ] = "HTTP/1.0";
    http_version_string_map[ HTTP_1_1 ] = "HTTP/1.1";
    http_version_string_map[ HTTP_2_0 ] = "HTTP/2.0";
    http_version_string_map[ HTTP_3_0 ] = "HTTP/3.0";

    return http_version_string_map;
}

// Path: srcs/constants/HttpMethodHelper.cpp
