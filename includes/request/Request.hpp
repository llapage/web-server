#ifndef REQUEST_HPP
#define REQUEST_HPP

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

#include "../configuration/IConfiguration.hpp"
#include "../constants/HttpHelper.hpp"
#include "IRequest.hpp"
#include <cstddef>
#include <map>
#include <string>
#include <vector>

class Request : public IRequest
{
private:
    // Request line components
    HttpMethod m_method;
    std::string m_uri;
    HttpVersion m_http_version;

    // Request headers
    std::map<HttpHeader, std::string> m_headers;

    // Request body
    std::vector<char> m_body;

    // Request buffer - used to store incomplete headers & chunks
    std::vector<char> m_buffer;

    // Included Request parameters
    std::string m_host_name;
    std::string m_host_port;
    std::string m_authority;
    std::map<std::string, std::string> m_query_parameters;
    std::pair<std::string, std::string> m_remote_address;
    std::map<std::string, std::string> m_cookies;
    std::vector<BodyParameter> m_body_parameters;
    bool m_upload_request;
    std::string m_request_id;
    std::string m_raw_request;
    const IConfiguration &m_configuration;

    // Helper
    const HttpHelper &m_http_helper;
    RequestState m_state;

    // Body File Path
    std::string m_body_file_path;

public:
    // Constructor and Destructor
    Request(const IConfiguration &configuration, const HttpHelper &http_helper);
    Request(const Request &src);
    ~Request();

    // Assignment operator
    Request &operator=(const Request &src);

    // Getters
    HttpMethod getMethod() const;
    std::string getMethodString() const;
    std::string getUri() const;
    HttpVersion getHttpVersion() const;
    std::string getHttpVersionString() const;
    const std::map<HttpHeader, std::string> getHeaders() const;
    std::string getHeaderValue(HttpHeader header) const;
    std::map<std::string, std::string> getHeadersStringMap() const;
    std::map<std::string, std::string> getQueryParameters() const;
    std::map<std::string, std::string> getCookies() const;
    std::string getCookie(const std::string &key) const;
    const std::vector<char> getBody() const;
    std::vector<char> &getBody(void);
    std::string getBodyString() const;
    std::string getQueryString() const;
    std::string getContentLength() const;
    std::string getContentType() const;
    std::string getPathInfo(const std::string &script_name) const;
    std::string getClientIp() const;
    std::string getHostName() const;
    std::string getHostPort() const;
    std::string getAuthority() const;
    const std::vector<BodyParameter> &getBodyParameters() const;
    bool isUploadRequest() const;
    RequestState &getState(void);
    const std::vector<char> &getBuffer() const;
    std::string getBodyFilePath() const;

    // Setters
    void setMethod(const std::string &method);
    void setUri(const std::string &uri);
    void setHttpVersion(const std::string &http_version);
    void addHeader(const std::string &key, const std::string &value);
    void setBody(const std::vector<char> &body);
    void setBody(const std::string &body);
    void addBodyChar(char value);
    void addCookie(const std::string &key, const std::string &value);
    void setAuthority();
    void addBodyParameter(const BodyParameter &body_parameter);
    void setUploadRequest(bool upload_request);
    virtual void appendBody(std::vector<char>::const_iterator start,
                            std::vector<char>::const_iterator end);
    void appendBuffer(const std::vector<char> &raw_request);
    void clearBuffer(void);
    void trimBuffer(ptrdiff_t new_start);
    void setBodyFilePath(const std::string &body_file_path);
};

#endif // REQUEST_HPP
       // Path: includes/Response.hpp
