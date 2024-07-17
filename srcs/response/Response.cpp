#include "../../includes/response/Response.hpp"
#include "../../includes/utils/Converter.hpp"
#include <cstddef>

/*
 * Response class
 *
 * This class represents an HTTP response.
 * It contains the status line, headers, and body of the response.
 *
 */

// Default constructor
Response::Response(const HttpHelper &httpHelper)
    : m_content_length(0), m_http_helper(httpHelper), m_buffer(0)
{
}

// Destructor
Response::~Response() {}

// Getter for status line
std::string Response::getStatusLine() const { return m_status_line; }

// Getter for headers - returns 1 string with all headers
std::string Response::getHeaders() const
{
    std::string headers;
    for (std::map<std::string, std::string>::const_iterator it =
             m_headers.begin();
         it != m_headers.end(); it++)
    {
        // Construct each header line in the format "HeaderName: Value\r\n"
        headers += it->first + ": " + it->second + "\r\n";
    }
    return headers;
}

// Getter for body string
std::string Response::getBodyString() const { return m_body.data(); }

// Getter for body vector
std::vector<char> Response::getBody() const { return m_body; }

// Getter for buffer vector
std::vector<char> &Response::getBuffer() { return m_buffer; }

// Setter for status line - string input
void Response::setStatusLine(std::string status_line)
{
    m_status_line = status_line;
}

// Setter for status line - based on status code
void Response::setStatusLine(HttpStatusCode status_code)
{
    // Set status line based on the status code
    m_status_line = m_http_helper.getStatusLine(status_code);
}

// Setter for headers - vector of strings input
void Response::setHeaders(std::vector<std::string> headers)
{
    for (std::vector<std::string>::iterator it = headers.begin();
         it != headers.end(); it++)
    {
        std::string header = *it;
        std::string header_name = header.substr(0, header.find(":"));
        std::string header_value = header.substr(header.find(":") + 1);

        // Add header to the map
        m_headers[ header_name ] = header_value;
    }
}

// Setter for headers - single string input
void Response::setHeaders(std::string headers)
{
    // Parse headers in the format "HeaderName: Value\r\n"
    while (headers.find("\r\n") != std::string::npos)
    {
        std::string header = headers.substr(0, headers.find("\r\n"));
        headers = headers.substr(headers.find("\r\n") + 2);
        std::string header_name = header.substr(0, header.find(":"));
        std::string header_value = header.substr(header.find(":") + 1);

        // Add header to the map
        m_headers[ header_name ] = header_value;
    }
}

// Add a header to the map - Enum, string input
void Response::addHeader(HttpHeader header, std::string value)
{
    this->addHeader(m_http_helper.httpHeaderStringMap(header), value);
}

// Add a header to the map - string, string input
void Response::addHeader(std::string header, std::string value)
{
    m_headers[ header ] = value;
}

// Add a header to the map - single string input
void Response::addHeader(std::string header)
{
    // Get the position of the colon in the header
    size_t colon_pos = header.find(":");

    // Extract the header name and value
    std::string header_name = header.substr(0, colon_pos);
    std::string header_value = header.substr(colon_pos + 1);

    // Add the header to the map
    this->addHeader(header_name, header_value);
}

// Add a cookie to the map
void Response::addCookie(std::string key, std::string value)
{
    m_cookies[ key ] = value;
}

// Add Cookie Headers to the response
void Response::addCookieHeaders()
{
    for (std::map<std::string, std::string>::const_iterator it =
             m_cookies.begin();
         it != m_cookies.end(); ++it)
    {
        // Construct the Set-Cookie header for the current cookie
        std::string cookie_header = it->first + "=" + it->second +
                                    "; HttpOnly; Secure; SameSite=Strict;";

        // Add the Set-Cookie header to the response
        this->addHeader(SET_COOKIE, cookie_header);
    }
}

// Setter for body - string input
void Response::setBody(std::string body)
{
    this->setBody(std::vector<char>(body.begin(), body.end()));
}

// Setter for body - vector of chars input
void Response::setBody(std::vector<char> body)
{
    m_body = body;
    m_content_length = body.size();
}

// Set all response fields from a status code
void Response::setErrorResponse(HttpStatusCode status_code)
{
    this->setStatusLine(status_code);
    std::string body = m_http_helper.getHtmlPage(status_code);
    this->setHeaders("content-type: text/html\r\n"
                     "content-length: " +
                     Converter::toString(body.length()) +
                     "\r\n"
                     "connection: close\r\n"
                     "server: webserv/1.0\r\n");
    this->setBody(body);
}

// Set all response fields from a status code - int input
void Response::setErrorResponse(int status_code)
{
    this->setErrorResponse(static_cast<HttpStatusCode>(status_code));
}

// Set all response fields for a permanently moved resource
void Response::setRedirectResponse(std::string location)
{
    this->setStatusLine(MOVED_PERMANENTLY);
    this->setHeaders("location: " + location +
                     "\r\n"
                     "content-length: 0\r\n"
                     "connection: close\r\n"
                     "server: webserv/1.0\r\n");
}

// Set response from a CGI response
void Response::setCgiResponse(std::vector<char> response)
{
    // Replace all LF with CRLF where needed
    for (std::vector<char>::iterator it = response.begin();
         it != response.end(); it++)
        if (*it != '\r' && (it + 1) != response.end() && *(it + 1) == '\n')
            it = response.insert(it + 1, '\r');

    // Parse the response vector
    std::string response_string(response.begin(), response.end());

    // Get the first line of the response
    std::string line = response_string.substr(0, response_string.find("\r\n"));

    // Check if it is a status line or a header
    if (line.find("HTTP") != std::string::npos)
    {
        // Set the status line
        this->setStatusLine(line);
        response_string =
            response_string.substr(response_string.find("\r\n") + 2);
    }
    else
    {
        // Set the default status line
        this->setStatusLine(OK);
    }

    // Set the headers
    size_t crlf_pos = response_string.find("\r\n");
    // starting with CRLF indicates the end of the headers
    while (crlf_pos != 0 && crlf_pos != std::string::npos)
    {
        // Get the header excluding the CRLF
        std::string header = response_string.substr(0, crlf_pos);

        // Add the header to the response
        this->addHeader(header);

        // Move to the next header
        response_string = response_string.substr(crlf_pos + 2);

        // Find the next CRLF
        crlf_pos = response_string.find("\r\n");
    }

    // Set the body
    if (crlf_pos != std::string::npos)
    {
        std::string body = response_string.substr(crlf_pos + 2);
        // Remove trailing CRLF
        if (body.find("\r\n") == body.length() - 2)
            body = body.substr(0, body.length() - 2);
        this->setBody(body);
    }

    // Set missing headers
    if (m_headers.find("content-length") == m_headers.end())
        this->addHeader("content-length: " +
                        Converter::toString(m_body.size()));
    if (m_headers.find("content-type") == m_headers.end())
        this->addHeader("content-type: text/html");
    if (m_headers.find("connection") == m_headers.end())
        this->addHeader("connection: close");
    if (m_headers.find("server") == m_headers.end())
        this->addHeader("server: webserv/1.0");
}

// Extract the status code from the status line
std::string Response::getStatusCodeString() const
{
    return m_status_line.substr(m_status_line.find(" ") + 1);
}

// Calculate the size of the response
std::string Response::getResponseSizeString() const
{
    return Converter::toString(m_status_line.length() +
                               this->getHeaders().length() + m_body.size());
}

// Calculate the size of the response in bytes
size_t Response::getResponseSize() const
{
    return m_status_line.length() + this->getHeaders().length() + m_body.size();
}

// Get the map of cookies
std::map<std::string, std::string> Response::getCookies() const
{
    return m_cookies;
}

// Get a specific cookie from the map
std::string Response::getCookie(const std::string &key) const
{
    std::map<std::string, std::string>::const_iterator it = m_cookies.find(key);
    if (it != m_cookies.end())
    {
        return it->second;
    }
    return "";
}

// Convert headers to a map of strings
std::map<std::string, std::string> Response::getHeadersStringMap() const
{
    return m_headers;
}

// Serialise the response into a vector of chars
std::vector<char> Response::serialise()
{
    std::vector<char> response;

    // Add status line
    response.insert(response.end(), m_status_line.begin(), m_status_line.end());

    // Add headers
    this->addCookieHeaders(); // Add cookies to the headers first
    const std::string &headers = this->getHeaders();
    response.insert(response.end(), headers.begin(), headers.end());

    // Add a blank line
    response.push_back('\r');
    response.push_back('\n');

    // Add body
    response.insert(response.end(), m_body.begin(), m_body.end());

    // Return the serialised response
    return response;
}

// Append data to the buffer
void Response::appendBuffer(std::vector<char> &data)
{
    m_buffer.insert(m_buffer.end(), data.begin(), data.end());
}

// Path: srcs/Response.cpp
