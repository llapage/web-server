#ifndef HTTPVERSIONHELPER_HPP
#define HTTPVERSIONHELPER_HPP

/*
 * HttpVersionHelper.hpp
 *
 * The HttpVersionHelper class provides functionality for working with HTTP
 * versions. It defines an enum HttpVersion to represent standard HTTP versions
 * and provides methods to retrieve string representations of versions and vice
 * versa.
 *
 * Example:
 *
 * HttpVersionHelper helper;
 * std::string version = "HTTP/1.1";
 * HttpVersion httpVersion = helper.stringHttpVersionMap(version);
 * std::string version = helper.httpVersionStringMap(httpVersion);
 *
 * std::string version = "HTTP/3.0";
 * if (helper.isHttpVersion(version)) {std::cout << "Valid HTTP version." <<
 * std::endl;}
 *
 * Note: Invalid input to Map searches will throw an UnknownHttpVersionError
 * exception.
 *
 * IMPORTANT: Version strings representations in this class are all UPPERCASE.
 */

#include <map>
#include <string>
#include <vector>

enum HttpVersion
{
    HTTP_0_9, // HTTP/0.9: The initial version of HTTP, introduced in 1991.
              // Simple protocol with support only for GET requests and no
              // headers.
    HTTP_1_0, // HTTP/1.0: Introduced in 1996, added support for additional
              // request methods (POST, HEAD), headers, status codes, and more.
    HTTP_1_1, // HTTP/1.1: Released in 1997, the most widely used version of
              // HTTP. Introduced persistent connections, chunked transfer
              // encoding, and more.
    HTTP_2_0, // HTTP/2: Introduced in 2015, a major overhaul of the protocol
              // focused on performance and efficiency. Introduced features like
              // multiplexing and header compression.
    HTTP_3_0  // HTTP/3: Still in development, based on the QUIC protocol. Aims
              // to improve performance and security, especially for web
              // applications.
};

class HttpVersionHelper
{
private:
    // Member variables
    const std::vector<std::string>
        m_version_list; // List of string representations of HTTP versions
    const std::map<std::string, HttpVersion>
        m_string_http_version_map; // Map of string representations to
                                   // HttpVersion enum values
    const std::map<HttpVersion, std::string>
        m_http_version_string_map; // Map of HttpVersion enum values to string
                                   // representations

    // Private member functions for initialization
    static std::vector<std::string> m_setVersionList();
    static std::map<std::string, HttpVersion> m_setStringHttpVersionMap();
    static std::map<HttpVersion, std::string> m_setHttpVersionStringMap();

public:
    // Constructor
    HttpVersionHelper();

    // Member functions to access data
    const std::string &httpVersionStringMap(
        HttpVersion version) const; // Get string representation of HttpVersion
    HttpVersion stringHttpVersionMap(const std::string &version)
        const; // Get HttpVersion enum value from string representation

    // Member function to check if a string represents a valid HTTP version
    bool isHttpVersion(const std::string &version) const;
};

#endif // HTTPVERSIONHELPER_HPP
// Path: includes/constants/HttpVersionHelper.hpp
