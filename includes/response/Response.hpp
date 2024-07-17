#ifndef RESPONSE_HPP
#define RESPONSE_HPP

/*
 * Response class
 *
 * This class represents an HTTP response
 * It contains the status line, headers, and body of the response.
 *
 */

#include "../../includes/constants/HttpHelper.hpp"
#include "IResponse.hpp"

class Response : public IResponse
{
private:
    // Response Status line
    std::string m_status_line;

    // Response headers
    std::map<std::string, std::string> m_headers;

    // Response body
    std::vector<char> m_body;

    // Body size
    size_t m_content_length;

    // Response Cookies
    std::map<std::string, std::string> m_cookies;

    // Helper
    const HttpHelper &m_http_helper;

    // Response buffer - used to store incomplete cgi responses
    std::vector<char> m_buffer;

public:
    Response(const HttpHelper &http_helper);
    ~Response();

    // Getters for status line, headers, body, and buffer
    virtual std::string getStatusLine() const;
    virtual std::string getHeaders() const;
    virtual std::string getBodyString() const;
    virtual std::vector<char> getBody() const;
    virtual std::vector<char> &getBuffer();

    // Setters for status line, headers, and body
    virtual void setStatusLine(std::string status_line);
    virtual void setStatusLine(HttpStatusCode status_code);
    virtual void setHeaders(std::vector<std::string> headers);
    virtual void setHeaders(std::string headers);
    virtual void addHeader(HttpHeader header, std::string value);
    virtual void addHeader(std::string header, std::string value);
    virtual void addHeader(std::string header);
    virtual void addCookie(std::string key, std::string value);
    virtual void addCookieHeaders();
    virtual void setBody(std::string body);
    virtual void setBody(std::vector<char> body);

    // Set error response with appropriate status code
    virtual void setErrorResponse(HttpStatusCode status_code);
    virtual void setErrorResponse(int status_code);

    // Set a redirect response
    virtual void setRedirectResponse(std::string location);

    // Set response fields from a complete response vector
    virtual void setCgiResponse(std::vector<char> response);

    // Getters for specific parts of the response
    virtual std::string getStatusCodeString() const;
    virtual std::string getResponseSizeString() const;
    virtual size_t getResponseSize() const;
    virtual std::map<std::string, std::string> getCookies() const;
    virtual std::string getCookie(const std::string &key) const;

    // Convert headers to map or string
    virtual std::map<std::string, std::string> getHeadersStringMap() const;
    virtual std::vector<char> serialise();

    // Append data to the buffer
    virtual void appendBuffer(std::vector<char> &data);
};

#endif // RESPONSE_HPP
// Path: includes/Response.hpp
