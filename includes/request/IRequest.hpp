#ifndef IREQUEST_HPP
#define IREQUEST_HPP

/*
 * IRequest.hpp
 * Abstract base class for Request in webserv
 *
 * This file defines the Request interface, which serves as the abstract base
 * class for the Request in webserv.
 * IRequest allows for polymorphic behavior and dependency injection,
 * thereby enabling us to create a MockRequest class for isolated unit testing.
 *
 */

#include "../constants/HttpHeaderHelper.hpp"
#include "../constants/HttpMethodHelper.hpp"
#include "../constants/HttpVersionHelper.hpp"
#include "../response/IRoute.hpp"
#include <cstddef>
#include <map>
#include <string>
#include <vector>

// BodyParameter represents a single part of a multipart/form-data request.
// This can be a file (for upload) or a form field (for regular form data).
struct BodyParameter
{
    std::string filename;     // Name of the file; applicable for file uploads
    std::string content_type; // MIME type of the content; applicable for file
                              // uploads
    std::vector<char> data;   // Data of the file or field; contains the content
                              // of the part
    std::string disposition_type; // Disposition type (e.g., "form-data",
                                  // "attachment", "inline")
    std::string field_name;       // Name of the form field; applicable for form
                                  // fields
    std::map<std::string, std::string>
        headers; // All headers of the part; useful
                 // for both files and form fields
};

class RequestState
{
private:
    int m_content_red;
    int m_content_length;
    bool m_initial;
    bool m_headers;
    bool m_finished;
    IRoute *m_route;

public:
    RequestState();
    bool finished(void) const;
    bool headers(void) const;
    bool initial(void) const;
    int getContentRed(void) const;
    int getContentLength(void) const;
    IRoute *getRoute(void) const;

    void finished(bool value);
    void headers(bool value);
    void initial(bool value);
    void incrementContentRed();
    void setContentRed(int value);
    void setContentLength(int value);
    void reset(void);
    void setRoute(IRoute *route);
};

class IRequest
{
public:
    virtual ~IRequest() {};

    // Getters
    virtual HttpMethod getMethod() const = 0;
    virtual std::string getMethodString() const = 0;
    virtual std::string getUri() const = 0;
    virtual HttpVersion getHttpVersion() const = 0;
    virtual std::string getHttpVersionString() const = 0;
    virtual const std::map<HttpHeader, std::string> getHeaders() const = 0;
    virtual std::string getHeaderValue(HttpHeader header) const = 0;
    virtual std::map<std::string, std::string> getHeadersStringMap() const = 0;
    virtual std::map<std::string, std::string> getQueryParameters() const = 0;
    virtual std::map<std::string, std::string> getCookies() const = 0;
    virtual std::string getCookie(const std::string &) const = 0;
    virtual const std::vector<char> getBody() const = 0;
    virtual std::string getBodyString() const = 0;
    virtual std::string getQueryString() const = 0;
    virtual std::string getContentLength() const = 0;
    virtual std::string getContentType() const = 0;
    virtual std::string getPathInfo(const std::string &script_name) const = 0;
    virtual std::string getClientIp() const = 0;
    virtual std::string getHostName() const = 0;
    virtual std::string getHostPort() const = 0;
    virtual std::string getAuthority() const = 0;
    virtual const std::vector<BodyParameter> &getBodyParameters() const = 0;
    virtual bool isUploadRequest() const = 0;
    virtual RequestState &getState(void) = 0;
    virtual std::vector<char> &getBody(void) = 0;
    virtual std::string getBodyFilePath() const = 0;
    virtual const std::vector<char> &getBuffer() const = 0;

    // Setters
    virtual void setMethod(const std::string &method) = 0;
    virtual void setUri(const std::string &uri) = 0;
    virtual void setHttpVersion(const std::string &http_version) = 0;
    virtual void addHeader(const std::string &key,
                           const std::string &value) = 0;
    virtual void setBody(const std::vector<char> &body) = 0;
    virtual void setBody(const std::string &body) = 0;
    virtual void addCookie(const std::string &key,
                           const std::string &value) = 0;
    virtual void setAuthority() = 0;
    virtual void addBodyParameter(const BodyParameter &body_parameter) = 0;
    virtual void setUploadRequest(bool upload_request) = 0;
    virtual void appendBody(std::vector<char>::const_iterator begin,
                            std::vector<char>::const_iterator end) = 0;
    virtual void appendBuffer(const std::vector<char> &raw_request) = 0;
    virtual void clearBuffer() = 0;
    virtual void trimBuffer(ptrdiff_t) = 0;
    virtual void setBodyFilePath(const std::string &body_file_path) = 0;
};

#endif // IREQUEST_HPP
       // Path: includes/IRequest.hpp
