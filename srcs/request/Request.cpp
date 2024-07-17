#include "../../includes/request/Request.hpp"
#include "../../includes/exception/WebservExceptions.hpp"

/*
 * Request: Represents an HTTP request.
 *
 * This class encapsulates the components of an HTTP request, including the
 * request method, Uri, HTTP version, requestHeaders, and an optional body. It
 * provides getter and setter methods to access and modify these components,
 * following the HTTP protocol specifications.
 *
 * It takes responsability to verifying the validity of its components and
 * throws exceptions if any component is invalid.
 *
 * Instances of this class are typically created by the RequestParser class,
 * which parses raw HTTP request strings and constructs Request objects from
 * them.
 *
 * Usage:
 * - Use getter methods to retrieve the properties of the request.
 *
 * Example:
 * request.getMethod(); // Get the request method
 * request.getUri(); // Get the request URI
 * request.getHttpVersion(); // Get the HTTP version
 * request.getHeaders(); // Get the request headers
 * request.getBody(); // Get the request body
 *
 * NOTE: for string representations use the string getters where available:
 * request.getMethodString(); // Get the request method as a string
 * request.getHttpVersionString(); // Get the HTTP version as a string
 * request.getHeadersStrings(); // Get the request headers as a string-keyed map
 *
 */

// Constructor initializes the Request object with a HttpHelper and a
// IConfiguration object
Request::Request(const IConfiguration &configuration,
                 const HttpHelper &httpHelper)
    : m_configuration(configuration), m_http_helper(httpHelper),
      m_body_file_path("")
{
}

// Copy assignment operator
Request &Request::operator=(const Request &src)
{
    // Assignment operator for copying the contents of another Request object
    if (this == &src)
        return *this; // Check for self-assignment
    // Copy individual member variables from src to this object
    m_method = src.m_method;
    m_uri = src.m_uri;
    m_http_version = src.m_http_version;
    m_headers = src.m_headers;
    m_body = src.m_body;
    return *this;
}

// Copy constructor
Request::Request(const Request &src)
    : m_configuration(src.m_configuration), m_http_helper(src.m_http_helper)
{
    // Copy constructor for copying the contents of another Request object
    *this = src; // Use the assignment operator to copy the contents
}

// Destructor
Request::~Request()
{
    // If a body file path was set, delete the file
    if (!m_body_file_path.empty())
        remove(m_body_file_path.c_str());
}

// Getter function for retrieving the HTTP method of the request
HttpMethod Request::getMethod() const { return m_method; }

// Getter function for retrieving the string representation of the HTTP method
std::string Request::getMethodString() const
{
    return m_http_helper.httpMethodStringMap(m_method);
}

// Getter function for retrieving the URI of the request
std::string Request::getUri() const { return m_uri; }

// Getter function for retrieving the HTTP version of the request
HttpVersion Request::getHttpVersion() const { return m_http_version; }

// Getter function for retrieving the string representation of the HTTP version
std::string Request::getHttpVersionString() const
{
    return m_http_helper.httpVersionStringMap(m_http_version);
}

// Getter function for retrieving the headers of the request
const std::map<HttpHeader, std::string> Request::getHeaders() const
{
    return m_headers;
}

// Getter function for retrieving the headers of the request as string-keyed map
std::map<std::string, std::string> Request::getHeadersStringMap() const
{
    std::map<std::string, std::string> headers_strings;
    for (std::map<HttpHeader, std::string>::const_iterator it =
             m_headers.begin();
         it != m_headers.end(); ++it)
    {
        // Convert enum keys to string keys and copy values
        headers_strings[ m_http_helper.httpHeaderStringMap(it->first) ] =
            it->second;
    }
    return headers_strings;
}

// Getter function for retrieving the query parameters
std::map<std::string, std::string> Request::getQueryParameters() const
{
    return m_query_parameters;
}

// Getter function for retrieving all the cookies
std::map<std::string, std::string> Request::getCookies() const
{
    return m_cookies;
}

// Getter function for retrieving a specific cookie
std::string Request::getCookie(const std::string &key) const
{
    // Check if the cookie exists
    if (m_cookies.find(key) != m_cookies.end())
        return m_cookies.at(key); // Return the value of the cookie
    else
        return ""; // Return an empty string if the cookie does not exist
}

// Getter function for retrieving the value of a specific header
std::string Request::getHeaderValue(HttpHeader header) const
{
    // Check if the header exists in the map
    if (m_headers.find(header) != m_headers.end())
        return m_headers.at(header); // Return the value of the header
    else
        return ""; // Return an empty string if the header does not exist
}

// Getter function for retrieving the body of the request
const std::vector<char> Request::getBody() const { return m_body; }

// Getter function for retrieving the body of the request as a string
std::string Request::getBodyString() const
{
    return std::string(m_body.begin(), m_body.end());
}

std::vector<char> &Request::getBody() { return m_body; }

// Getter function for retrieving the query string
std::string Request::getQueryString() const
{
    // Check if the URI contains a query string
    size_t query_pos = m_uri.find('?');
    if (query_pos != std::string::npos)
    {
        // Return the query string
        return m_uri.substr(query_pos + 1);
    }
    else
    {
        return ""; // Return an empty string if the URI does not contain a query
                   // string
    }
}

// Getter function for retrieving the content length
std::string Request::getContentLength() const
{
    // Check if the 'content-length' header exists in the map
    if (m_headers.find(CONTENT_LENGTH) != m_headers.end())
    {
        // Return the value of the 'content-length' header
        return m_headers.at(CONTENT_LENGTH);
    }
    else
    {
        return ""; // Return an empty string if the 'content-length' header does
                   // not exist
    }
}

// Getter function for retrieving the content type
std::string Request::getContentType() const
{
    // Check if the 'content-type' header exists in the map
    if (m_headers.find(CONTENT_TYPE) != m_headers.end())
    {
        // Return the value of the 'content-type' header
        return m_headers.at(CONTENT_TYPE);
    }
    else
    {
        return ""; // Return an empty string if the 'content-type' header does
                   // not exist
    }
}

// Getter function for retrieving the path info
std::string Request::getPathInfo(const std::string &script_name) const
{
    // Check if the URI contains the script name
    size_t script_name_pos = m_uri.find(script_name);
    if (script_name_pos == std::string::npos)
        return ""; // Return an empty string if the URI does not contain the
                   // script name

    // Calculate the start of the path info
    size_t path_info_start = script_name_pos + script_name.size();

    // Get the start of the query string, if present
    size_t query_pos = m_uri.find('?', path_info_start);

    // Return the path info
    if (query_pos != std::string::npos)
    {
        // Return the path info excluding the query string
        return m_uri.substr(path_info_start, query_pos - path_info_start);
    }
    else
    {
        // Return the remaining part of the URI as path info
        return m_uri.substr(path_info_start);
    }
}

// Getter function for retrieving the client IP address
std::string Request::getClientIp() const
{
    // Check if the 'X-Forwarded-For' header exists in the map
    if (m_headers.find(X_FORWARDED_FOR) != m_headers.end())
    {
        // Return the value of the 'X-Forwarded-For' header
        return m_headers.at(X_FORWARDED_FOR);
    }
    else
    {
        // Return the value of the 'REMOTE_ADDR' header
        return m_remote_address.second;
    }
}

// Getter function for retrieving the host name
std::string Request::getHostName() const { return m_host_name; }

// Getter function for retrieving the host port
std::string Request::getHostPort() const { return m_host_port; }

// Getter function for retrieving the authority of the request
std::string Request::getAuthority() const { return m_authority; }

// Getter function for retrieving the body parameters
const std::vector<BodyParameter> &Request::getBodyParameters() const
{
    return m_body_parameters;
}

// Getter function for retrieving the body file path
std::string Request::getBodyFilePath() const { return m_body_file_path; }

// Getter function for checking if the request is an upload request
bool Request::isUploadRequest() const { return m_upload_request; }

// Setter function for setting the method of the request
void Request::setMethod(const std::string &method)
{
    if (m_http_helper.isMethod(method) == false)
        throw HttpStatusCodeException(
            METHOD_NOT_ALLOWED, // Throw '405' status error
            "unknown method: \"" + method + "\"");
    else if (m_http_helper.isSupportedMethod(method) == false)
        // throw HttpStatusCodeException(
        //    NOT_IMPLEMENTED, // Throw '501' status error
        //    "unsupported method: \"" + method + "\"");
        throw HttpStatusCodeException(
            METHOD_NOT_ALLOWED, // Throw '405' status error
            "unsupported method: \"" + method + "\"");

    // Set the method of the request
    m_method = m_http_helper.stringHttpMethodMap(method);
}

// Getter function for retrieving the state of the request
RequestState &Request::getState(void) { return m_state; }

// Getter function for retrieving the buffer of the request
const std::vector<char> &Request::getBuffer() const { return m_buffer; }

// Setter function for setting the URI of the request
void Request::setUri(const std::string &uri)
{
    // Check if the URI size exceeds the maximum allowed URI size
    if (uri.size() > m_configuration.getSize_t("client_max_uri_size"))
        throw HttpStatusCodeException(URI_TOO_LONG); // Throw '414' status error

    // Check if the URI contains any whitespace characters
    if (uri.find_first_of(" \t") != std::string::npos)
        throw HttpStatusCodeException(BAD_REQUEST, // Throw '400' status error
                                      "whitespace in URI");

    // Set the URI
    m_uri = uri;
}

// Setter function for setting the HTTP version of the request
void Request::setHttpVersion(const std::string &httpVersion)
{
    // Set the HTTP version of the request
    // Use the HttpHelper to map the string representation of the HTTP version
    // to an HttpVersion enum value If the HTTP version is not recognized, an
    // UnknownHttpVersionError is thrown
    m_http_version = m_http_helper.stringHttpVersionMap(httpVersion);
}

// Function for adding a header to the request
void Request::addHeader(const std::string &key, const std::string &value)
{
    // Check if the key contains trailing whitespace
    if (!key.empty() &&
        (key[ key.length() - 1 ] == ' ' || key[ key.length() - 1 ] == '\t'))
    {
        throw HttpStatusCodeException(BAD_REQUEST,
                                      "trailing whitespace in header key");
    }

    // Convert the key to lowercase
    std::string lowercase_key = key;
    for (std::string::iterator it = lowercase_key.begin();
         it != lowercase_key.end(); ++it)
    {
        *it = std::tolower(static_cast<unsigned char>(*it));
    }

    // Check if key exists in the map
    HttpHeader name;
    try
    {
        name = m_http_helper.stringHttpHeaderMap(lowercase_key);
        // Add the header to the internal headers map
        m_headers[ name ] = value;
    }
    catch (const UnknownHeaderError &e)
    {
        // Re-throw with original key
        throw UnknownHeaderError(key); // We will skip to the next header
    }
}

// Setter function for setting the body of the request
void Request::setBody(const std::vector<char> &body)
{
    // Check if the body is empty
    if (body.empty())
        return; // If empty, do nothing (no body to set)

    // Check if the body size exceeds the maximum allowed body size
    if (body.size() > m_configuration.getSize_t("client_max_body_size"))
        throw HttpStatusCodeException(
            PAYLOAD_TOO_LARGE); // Throw '413' status error

    // Set the body of the request
    m_body = body;
}

// Setter function for setting the body of the request as a string
void Request::setBody(const std::string &body)
{
    // Convert the body string to a vector of characters
    std::vector<char> bodyVector(body.begin(), body.end());

    // Set the body of the request
    this->setBody(bodyVector);
}

void Request::addBodyChar(char value) { m_body.push_back(value); }

// Function for adding a cookie to the request
void Request::addCookie(const std::string &key, const std::string &value)
{
    // Add the cookie to the internal cookies map
    m_cookies[ key ] = value;
}
#include <iostream>
// Setter function for setting the authority of the request
void Request::setAuthority()
{
    // Check if the 'Host' header exists in the map
    if (m_headers[ HOST ] == "")
        throw HttpStatusCodeException(BAD_REQUEST, // Throw '400' status error
                                      "missing Host header");
    std::istringstream host_stream(m_headers[ HOST ]);
    if (std::getline(host_stream, m_host_name, ':'))
    {
        // If the host header contains a port number, set the host port
        std::getline(host_stream, m_host_port);
    }
    else
    {
        // If the host header does not contain a port number, set the host port
        // to the default port
        m_host_port = m_configuration.getString("default_port");
    }
    if (m_host_port.empty())
        m_host_port = m_configuration.getString("default_port");
    // Set the authority of the request
    m_authority = m_host_name + ":" + m_host_port;
}

// Function for adding a body parameter to the request
void Request::addBodyParameter(const BodyParameter &body_parameter)
{
    // Add the body parameter to the internal body parameters vector
    m_body_parameters.push_back(body_parameter);
}

// Setter function for setting the upload request flag
void Request::setUploadRequest(bool upload_request)
{
    m_upload_request = upload_request;
}

// Method to append new data to the body
void Request::appendBody(std::vector<char>::const_iterator start,
                         std::vector<char>::const_iterator end)
{
    m_body.insert(m_body.end(), start, end);
}

// Method to append new date to the buffer
void Request::appendBuffer(const std::vector<char> &raw_request)
{
    m_buffer.insert(m_buffer.end(), raw_request.begin(), raw_request.end());
}

// Method to clear the buffer
void Request::clearBuffer()
{
    m_buffer.clear();
    m_buffer.shrink_to_fit(); // makes .data() return nullptr
                              // std::vector<char>(m_buffer).swap(m_buffer);
}

// Method to trim the buffer
void Request::trimBuffer(ptrdiff_t new_start)
{
    std::vector<char>::iterator new_start_it = m_buffer.begin() + new_start;
    if (new_start_it == m_buffer.end())
    {
        this->clearBuffer();
    }
    else
    {
        m_buffer.erase(m_buffer.begin(), new_start_it);
    }
}

// Method to set the body file descriptor
void Request::setBodyFilePath(const std::string &body_file_path)
{
    m_body_file_path = body_file_path;
}

// path: srcs/request/Request.cpp
