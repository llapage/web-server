#ifndef IRESPONSE_HPP
#define IRESPONSE_HPP

/*
 * Response class
 *
 * This class represents an HTTP response
 * It contains the status line, headers, and body of the response.
 *
 */

#include "../constants/HttpHeaderHelper.hpp"
#include "../constants/HttpStatusCodeHelper.hpp"
#include <map>
#include <string>

class IResponse
{
public:
    virtual ~IResponse() {};

    // Getters for status line, headers, body and buffer
    virtual std::string getStatusLine() const = 0;
    virtual std::string getHeaders() const = 0;
    virtual std::string getBodyString() const = 0;
    virtual std::vector<char> getBody() const = 0;
    virtual std::vector<char> &getBuffer() = 0;

    // Setters for status line, headers, and body
    virtual void setStatusLine(std::string status_line) = 0;
    virtual void setStatusLine(HttpStatusCode status_code) = 0;
    virtual void setHeaders(std::vector<std::string> headers) = 0;
    virtual void setHeaders(std::string headers) = 0;
    virtual void addHeader(HttpHeader header, std::string value) = 0;
    virtual void addHeader(std::string header, std::string value) = 0;
    virtual void addHeader(std::string header) = 0;
    virtual void addCookie(std::string key, std::string value) = 0;
    virtual void addCookieHeaders() = 0;
    virtual void setBody(std::string body) = 0;
    virtual void setBody(std::vector<char> body) = 0;

    // Set error response with appropriate status code
    virtual void setErrorResponse(HttpStatusCode status_code) = 0;
    virtual void setErrorResponse(int status_code) = 0;

    // Set a redirect response
    virtual void setRedirectResponse(std::string location) = 0;

    // Set response fields from a complete response vector
    virtual void setCgiResponse(std::vector<char> response) = 0;

    // Getters for specific parts of the response
    virtual std::string getStatusCodeString() const = 0;
    virtual std::string getResponseSizeString() const = 0;
    virtual size_t getResponseSize() const = 0;
    virtual std::map<std::string, std::string> getCookies() const = 0;
    virtual std::string getCookie(const std::string &key) const = 0;

    // Convert headers to map or string
    virtual std::map<std::string, std::string> getHeadersStringMap() const = 0;
    virtual std::vector<char> serialise() = 0;

    // Append data to the buffer
    virtual void appendBuffer(std::vector<char> &data) = 0;
};

#endif // IRESPONSE_HPP
// Path: includes/IResponse.hpp
