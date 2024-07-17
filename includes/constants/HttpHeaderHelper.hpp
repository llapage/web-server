#ifndef HTTPHEADERHELPER_HPP
#define HTTPHEADERHELPER_HPP

/*
 * HttpHeaderHelper.hpp
 *
 * The HttpHeaderHelper class provides functionality for working with HTTP
 * headers. It defines an enum HttpHeader to represent standard HTTP headers and
 * provides methods to retrieve string representations of headers and vice
 * versa.
 *
 * Example:
 *
 * HttpHeaderHelper helper;
 * std::string header = "accept";
 * HttpHeader httpHeader = helper.stringHttpHeaderMap(header);
 * std::string header = helper.httpHeaderStringMap(httpHeader);
 *
 * std::string header = "proxy-autohorization";
 * if (helper.isHeaderName(header)) {std::cout << "Valid header name." <<
 * std::endl;}
 *
 * Note: Invalid input to Map searches will throw an UnknownHeaderError
 * exception.
 *
 * IMPORTANT: Header strings representations in this class are all lowercase.
 *
 */

#include <map>
#include <string>
#include <vector>

enum HttpHeader
{
    // General Headers
    CACHE_CONTROL, // Controls caching mechanisms
    CONNECTION,    // Specifies options for the current connection
    DATE,    // Represents the date and time at which the message was originated
    PRAGMA,  // Implementation-specific directives that might apply to any
             // recipient along the request/response chain
    TRAILER, // Contains header fields that apply to the trailer of a message
    TRANSFER_ENCODING, // Specifies the form of encoding used to safely transfer
                       // the entity to the user
    UPGRADE, // Requests that the server upgrade to a different protocol
    VIA, // Informs the recipient about the network path taken by the request
         // message
    WARNING, // Contains additional information about potential problems
             // encountered by the server

    // Request Headers
    ACCEPT, // Indicates the media types that are acceptable for the response
    ACCEPT_CHARSET,  // Indicates the character sets that are acceptable for the
                     // response
    ACCEPT_ENCODING, // Indicates the encoding algorithms that are acceptable
                     // for the response
    ACCEPT_LANGUAGE, // Indicates the natural languages that are preferred for
                     // the response
    AUTHORIZATION,   // Contains the credentials to authenticate the user agent
                     // with the server
    EXPECT,          // Indicates expectations that need to be met by the server
    FROM, // Contains an Internet email address for the human user who controls
          // the requesting user agent
    HOST, // Specifies the host and port number of the target resource
    IF_MATCH, // Makes the request conditional on the current representation of
              // the target resource
    IF_MODIFIED_SINCE, // Makes the request conditional on the last modification
                       // date of the target resource
    IF_NONE_MATCH, // Makes the request conditional on a current representation
                   // of the target resource not matching any of the given ETags
    IF_RANGE, // Makes the request conditional on a range of the target resource
    IF_UNMODIFIED_SINCE, // Makes the request conditional on the last
                         // modification date of the target resource not being
                         // later than the given date/time
    MAX_FORWARDS, // Indicates the remaining number of times that this request
                  // message can be forwarded
    PROXY_AUTHORIZATION, // Contains the credentials to authenticate the user
                         // agent with a proxy server
    RANGE,               // Requests only a portion of the specified resource
    REFERER, // Contains the address of the previous web page from which a link
             // to the currently requested page was followed
    TE, // Specifies the transfer encodings that are acceptable for the response
    USER_AGENT, // Contains information about the user agent originating the
                // request

    // Response Headers
    ACCEPT_RANGES, // Indicates whether the server supports range requests
    AGE,           // Represents the age of the response
    ETAG,          // Provides the current entity tag for the requested variant
    LOCATION,      // Specifies the preferred location for the response
    PROXY_AUTHENTICATE, // Contains the credentials to authenticate the user
                        // agent with a proxy server
    RETRY_AFTER, // Indicates how long the user agent should wait before making
                 // a follow-up request
    SERVER, // Contains information about the software used by the origin server
    VARY,   // Specifies the request header fields that influence the response
            // representation's selection
    WWW_AUTHENTICATE, // Contains the credentials to authenticate the user agent
                      // with the origin server

    // Entity Headers
    ALLOW, // Specifies the set of HTTP methods that are supported for the
           // target resource
    CONTENT_ENCODING, // Specifies the encoding that has been applied to the
                      // representation
    CONTENT_LANGUAGE, // Specifies the natural language(s) of the intended
                      // audience for the representation
    CONTENT_LENGTH,   // Indicates the size of the entity-body
    CONTENT_LOCATION, // Specifies an alternate location for the representation
    CONTENT_MD5,      // Provides an MD5 digest of the entity-body
    CONTENT_RANGE,    // Indicates where in a full body message this partial
                      // message belongs
    CONTENT_TYPE,     // Indicates the media type of the entity-body
    EXPIRES, // Gives the date/time after which the response is considered stale
    LAST_MODIFIED, // Indicates the last modification date of the resource

    // Additional Headers
    ACCESS_CONTROL_ALLOW_ORIGIN,  // Specifies which origins are allowed to
                                  // access the resource
    ACCESS_CONTROL_ALLOW_METHODS, // Specifies the methods allowed when
                                  // accessing the resource
    ACCESS_CONTROL_ALLOW_HEADERS, // Specifies the headers allowed when
                                  // accessing the resource
    ACCESS_CONTROL_MAX_AGE, // Indicates how long the results of a preflight
                            // request can be cached
    ACCESS_CONTROL_ALLOW_CREDENTIALS, // Indicates whether the response to the
                                      // request can be exposed when the
                                      // credentials flag is true
    ACCESS_CONTROL_EXPOSE_HEADERS, // Indicates which headers can be exposed as
                                   // part of the response by listing their
                                   // names
    ACCESS_CONTROL_REQUEST_METHOD, // Used in response to a preflight request to
                                   // indicate which HTTP methods can be used
                                   // during the actual request
    ACCESS_CONTROL_REQUEST_HEADERS, // Used in response to a preflight request
                                    // to indicate which HTTP headers can be
                                    // used during the actual request
    CONTENT_DISPOSITION, // Indicates the disposition type and optional filename
                         // of the content
    CONTENT_SECURITY_POLICY, // Defines a policy to mitigate cross-site
                             // scripting (XSS) attacks
    CONTENT_SECURITY_POLICY_REPORT_ONLY, // Specifies a policy to mitigate
                                         // cross-site scripting (XSS) attacks,
                                         // but only reports violations
    DNT, // Indicates whether the user agent prefers not to have the user's
         // activity tracked
    FORWARDED, // Contains information from the client-facing side of proxy
               // servers that is altered or lost when a proxy is involved in
               // the path of the request
    ORIGIN, // Specifies the origin server from which the request was initiated
    TIMING_ALLOW_ORIGIN, // Indicates whether the resource is accessible to the
                         // requesting origin
    X_CONTENT_TYPE_OPTIONS, // Mitigates the risk of content sniffing
                            // vulnerabilities
    X_FRAME_OPTIONS,  // Specifies whether a browser should be allowed to render
                      // a page in a frame, iframe, embed, or object
    X_XSS_PROTECTION, // Enables a cross-site scripting (XSS) filter in
                      // supported web browsers
    X_FORWARDED_FOR, // Contains the IP address of the client that initiated the
                     // request
    X_SECRET_HEADER_FOR_TEST,  // A header used by a 42 tester program
    UPGRADE_INSECURE_REQUESTS, // Expresses the client's preference for an
                               // encrypted and authenticated response, and that
                               // it can successfully handle the
                               // upgrade-insecure-requests CSP directive.
    SEC_FETCH_DEST,            // Indicates the destination of the fetch request
    SEC_FETCH_MODE,            // Indicates the mode of the fetch request
    SEC_FETCH_SITE,            // Indicates the site of the fetch request
    SEC_FETCH_USER,            // Indicates the user of the fetch request
    PRIORITY,                  // Indicates the priority of the resource request

    // Cookie Headers
    COOKIE,     // Request header for receiving cookies
    SET_COOKIE, // Response header for setting cookies
};

class HttpHeaderHelper
{
private:
    // Member variables
    const std::vector<std::string>
        m_header_list; // List of string representations of HTTP headers
    const std::map<std::string, HttpHeader>
        m_string_http_header_map; // Map of string representations to HttpHeader
                                  // enum values
    const std::map<HttpHeader, std::string>
        m_http_header_string_map; // Map of HttpHeader enum values to string
                                  // representations

    // Private member functions for initialization
    static std::vector<std::string> m_setHeaderList();
    static std::map<std::string, HttpHeader> m_setStringHttpHeaderMap();
    static std::map<HttpHeader, std::string> m_setHttpHeaderStringMap();

public:
    // Constructor
    HttpHeaderHelper();

    // Member functions to access data
    const std::string &httpHeaderStringMap(
        HttpHeader header) const; // Get string representation of HttpHeader
    HttpHeader stringHttpHeaderMap(const std::string &header)
        const; // Get HttpHeader enum value from string representation

    // Member function to check if a string is a valid HTTP header
    bool isHeaderName(const std::string &header)
        const; // Check if a string is a valid HTTP header
};

#endif // HTTPHEADERHELPER_HPP
// Path: includes/constants/HttpHeaderHelper.hpp
