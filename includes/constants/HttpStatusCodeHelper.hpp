#ifndef HTTPSTATUSCODEHELPER_HPP
#define HTTPSTATUSCODEHELPER_HPP

/*
 * HttpStatusCodeHelper.hpp
 *
 * The HttpStatusCodeHelper class provides functionality for working with HTTP
 * status codes. It defines an enum HttpStatusCode to represent standard HTTP
 * status codes and provides methods to retrieve string representations of
 * status codes and vice versa. It may also be used to generate an HTML page
 * with a specified HTTP status code.
 *
 * Example:
 *
 * HttpStatusCodeHelper helper;
 * HttpStatusCode code = 502;
 *
 * std::string meaning = helper.httpStatusCodeStringMap(status_code);
 * std::string htmlPage = helper.getErrorResponse(status_code);
 *
 */

#include <map>
#include <string>
#include <vector>

enum HttpStatusCode
{
    // Informational 1xx
    CONTINUE = 100, // The server has received the request headers and the
                    // client should proceed to send the request body
    SWITCHING_PROTOCOLS = 101, // The requester has asked the server to switch
                               // protocols and the server has agreed to do so
    PROCESSING = 102,  // The server is processing the request, but no response
                       // is available yet
    EARLY_HINTS = 103, // Used to return some response headers before final HTTP
                       // message

    // Success 2xx
    OK = 200,       // The request has succeeded
    CREATED = 201,  // The request has been fulfilled, resulting in the creation
                    // of a new resource
    ACCEPTED = 202, // The request has been accepted for processing, but the
                    // processing has not been completed
    NON_AUTHORITATIVE_INFORMATION =
        203, // The server is a transforming proxy that received a 200 OK
             // response from its origin, but is returning a modified
             // version of the origin's response
    NO_CONTENT = 204,    // The server successfully processed the request and is
                         // not returning any content
    RESET_CONTENT = 205, // The server successfully processed the request and is
                         // not returning any content, but the client should
                         // reset the document view
    PARTIAL_CONTENT = 206, // The server is delivering only part of the resource
                           // due to a range header sent by the client
    MULTI_STATUS = 207,    // The message body that follows is by default an XML
                        // message and can contain a number of separate response
                        // codes, depending on how many sub-requests were made
    ALREADY_REPORTED =
        208, // The members of a DAV binding have already been enumerated in
             // a preceding part of the (multistatus) response, and are not
             // being included again
    IM_USED =
        226, // The server has fulfilled a request for the resource, and the
             // response is a representation of the result of one or more
             // instance-manipulations applied to the current instance

    // Redirection 3xx
    MULTIPLE_CHOICES = 300,  // Indicates multiple options for the resource from
                             // which the client may choose
    MOVED_PERMANENTLY = 301, // This and all future requests should be directed
                             // to the given URI
    FOUND = 302,        // Tells the client to look at (browse to) another URL
    SEE_OTHER = 303,    // Tells the client to look at (browse to) another URL
    NOT_MODIFIED = 304, // Indicates that the resource has not been modified
                        // since the version specified by the request headers
                        // If-Modified-Since or If-None-Match
    USE_PROXY = 305,    // The requested resource is available only through a
                     // proxy, the address for which is provided in the response
    SWITCH_PROXY = 306,       // No longer used
    TEMPORARY_REDIRECT = 307, // Tells the client to go to a different URL
    PERMANENT_REDIRECT = 308, // This and all future requests should be directed
                              // to the given URI

    // Client Error 4xx
    BAD_REQUEST = 400,  // The server cannot or will not process the request due
                        // to an apparent client error
    UNAUTHORIZED = 401, // Similar to 403 Forbidden, but specifically for use
                        // when authentication is required and has failed or has
                        // not yet been provided
    PAYMENT_REQUIRED = 402, // Reserved for future use
    FORBIDDEN = 403, // The request contained valid data and was understood by
                     // the server, but the server is refusing action
    NOT_FOUND = 404, // The requested resource could not be found but may be
                     // available in the future
    METHOD_NOT_ALLOWED =
        405, // A request method is not supported for the requested resource
    NOT_ACCEPTABLE = 406, // The requested resource is capable of generating
                          // only content not acceptable according to the Accept
                          // headers sent in the request
    PROXY_AUTHENTICATION_REQUIRED =
        407, // The client must first authenticate itself with the proxy
    REQUEST_TIMEOUT = 408, // The server timed out waiting for the request
    CONFLICT = 409, // Indicates that the request could not be processed because
                    // of conflict in the request, such as an edit conflict
    GONE = 410, // Indicates that the resource requested is no longer available
                // and will not be available again
    LENGTH_REQUIRED =
        411, // The request did not specify the length of its content, which
             // is required by the requested resource
    PRECONDITION_FAILED =
        412, // The server does not meet one of the preconditions that the
             // requester put on the request header fields
    PAYLOAD_TOO_LARGE = 413, // The request is larger than the server is willing
                             // or able to process
    URI_TOO_LONG =
        414, // The URI provided was too long for the server to process
    UNSUPPORTED_MEDIA_TYPE = 415, // The request entity has a media type which
                                  // the server or resource does not support
    RANGE_NOT_SATISFIABLE =
        416, // The client has asked for a portion of the file, but the
             // server cannot supply that portion
    EXPECTATION_FAILED = 417,  // The server cannot meet the requirements of the
                               // Expect request-header field
    IM_A_TEAPOT = 418,         // Returned by teapots requested to brew coffee
    MISDIRECTED_REQUEST = 421, // The request was directed at a server that is
                               // not able to produce a response
    UNPROCESSABLE_ENTITY = 422, // The request was well-formed but was unable to
                                // be followed due to semantic errors
    LOCKED = 423,               // The resource that is being accessed is locked
    FAILED_DEPENDENCY =
        424,         // The request failed due to failure of a previous request
    TOO_EARLY = 425, // Indicates that the server is unwilling to risk
                     // processing a request that might be replayed
    UPGRADE_REQUIRED = 426, // The client should switch to a different protocol
    PRECONDITION_REQUIRED =
        428, // The origin server requires the request to be conditional
    TOO_MANY_REQUESTS = 429, // The user has sent too many requests in a given
                             // amount of time
    REQUEST_HEADER_FIELDS_TOO_LARGE =
        431, // Server is unwilling to process the request because either an
             // individual header field or all the header fields
             // collectively are too large
    UNAVAILABLE_FOR_LEGAL_REASONS =
        451, // Resource access is denied for legal reasons

    // Server Error 5xx
    INTERNAL_SERVER_ERROR =
        500, // A generic error message, given when an unexpected condition
             // was encountered and no more specific message is suitable
    NOT_IMPLEMENTED =
        501, // The server either does not recognize the request method, or
             // it lacks the ability to fulfill the request
    BAD_GATEWAY = 502, // The server was acting as a gateway or proxy and
                       // received an invalid response from the upstream server
    SERVICE_UNAVAILABLE =
        503, // The server cannot handle the request due to temporary
             // overload or maintenance of the server
    GATEWAY_TIMEOUT =
        504, // The server was acting as a gateway or proxy and did not
             // receive a timely response from the upstream server
    HTTP_VERSION_NOT_SUPPORTED = 505, // The server does not support the HTTP
                                      // protocol version used in the request
    VARIANT_ALSO_NEGOTIATES = 506,    // Transparent content negotiation for the
                                      // request results in a circular reference
    INSUFFICIENT_STORAGE = 507,       // The server is unable to store the
                                // representation needed to complete the request
    LOOP_DETECTED = 508, // The server detected an infinite loop while
                         // processing the request
    NOT_EXTENDED = 510,  // Further extensions to the request are required for
                         // the server to fulfill it
    NETWORK_AUTHENTICATION_REQUIRED =
        511 // The client needs to authenticate to gain network access
};

class HttpStatusCodeHelper
{
private:
    // Member variables
    const std::vector<std::string>
        m_status_code_list; // List of string representations of HTTP versions
    const std::map<std::string, HttpStatusCode>
        m_string_http_status_code_map; // Map of string representations to
                                       // HttpStatusCode enum values
    const std::map<HttpStatusCode, std::string>
        m_http_status_code_string_map; // Map of HttpStatusCode enum values to
                                       // string representations
    const std::map<std::string, std::string>
        m_status_code_description; // Map of status code to description
    const std::map<HttpStatusCode, std::string>
        m_status_code_html_page_map; // Map of status code to html page

    // Private member functions for initialization
    static std::vector<std::string> m_setStatusCodeList();
    static std::map<std::string, HttpStatusCode> m_setStringHttpStatusCodeMap();
    static std::map<HttpStatusCode, std::string> m_setHttpStatusCodeStringMap();
    std::map<HttpStatusCode, std::string>
    m_setStatusCodeHtmlPageMap(std::vector<std::string> error_page);

public:
    // Constructor
    HttpStatusCodeHelper(
        std::vector<std::string> error_page = std::vector<std::string>());

    // Member functions to access data
    const std::string &httpStatusCodeStringMap(HttpStatusCode status_code)
        const; // Get string representation of HttpStatusCode
    HttpStatusCode stringHttpStatusCodeMap(const std::string &status_code)
        const; // Get HttpStatusCode enum value from string representation
    HttpStatusCode intHttpStatusCodeMap(const int &status_code)
        const; // Get HttpStatusCode enum value from integer representation

    // Member function to generate a status line
    std::string getStatusLine(HttpStatusCode status_code)
        const; // Generate a status line with the specified HTTP status code

    // Member function to generate an HTML page
    std::string getHtmlPage(HttpStatusCode status_code)
        const; // Generate an HTML page with the specified HTTP status code

    // Member function to generate a complete response
    std::string getErrorResponse(HttpStatusCode status_code)
        const; // Generate an HTML page with the specified HTTP status code
};

#endif // HTTPSTATUSCODEHELPER_HPP
// Path: includes/constants/HttpStatusCodeHelper.hpp
