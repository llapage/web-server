#include "../../includes/constants/HttpHeaderHelper.hpp"
#include "../../includes/exception/WebservExceptions.hpp"

/*
 * HttpVersionHelper.cpp
 *
 * The HttpVersionHelper class provides functionality for working with HTTP
 * versions. It defines an enum HttpVersion to represent standard HTTP versions
 * and provides methods to retrieve string representations of versions and vice
 * versa.
 *
 */

// Constructor initializes member variables using helper functions
HttpHeaderHelper::HttpHeaderHelper()
    : m_header_list(m_setHeaderList()),
      m_string_http_header_map(m_setStringHttpHeaderMap()),
      m_http_header_string_map(m_setHttpHeaderStringMap())
{
}

// Get string representation of HttpHeader enum value
const std::string &
HttpHeaderHelper::httpHeaderStringMap(HttpHeader header) const
{
    if (m_http_header_string_map.find(header) != m_http_header_string_map.end())
    {
        return m_http_header_string_map.at(header);
    }
    else
    {
        throw UnknownHeaderError();
    }
}

// Get HttpHeader enum value from string representation
HttpHeader
HttpHeaderHelper::stringHttpHeaderMap(const std::string &header) const
{
    if (m_string_http_header_map.find(header) != m_string_http_header_map.end())
    {
        return m_string_http_header_map.at(header);
    }
    else
    {
        throw UnknownHeaderError(header);
    }
}

// Check if a string is a valid HTTP header name
bool HttpHeaderHelper::isHeaderName(const std::string &header) const
{
    return m_string_http_header_map.find(header) !=
           m_string_http_header_map.end();
}

// Helper function to initialize m_header_list with string representations of
// HTTP headers
std::vector<std::string> HttpHeaderHelper::m_setHeaderList()
{
    std::vector<std::string> header_list;

    // Add string representations of HTTP headers to header_list
    header_list.push_back("cache-control");
    header_list.push_back("connection");
    header_list.push_back("date");
    header_list.push_back("pragma");
    header_list.push_back("trailer");
    header_list.push_back("transfer-encoding");
    header_list.push_back("upgrade");
    header_list.push_back("via");
    header_list.push_back("warning");
    header_list.push_back("accept");
    header_list.push_back("accept-charset");
    header_list.push_back("accept-encoding");
    header_list.push_back("accept-language");
    header_list.push_back("authorization");
    header_list.push_back("expect");
    header_list.push_back("from");
    header_list.push_back("host");
    header_list.push_back("if-match");
    header_list.push_back("if-modified-since");
    header_list.push_back("if-none-match");
    header_list.push_back("if-range");
    header_list.push_back("if-unmodified-since");
    header_list.push_back("max-forwards");
    header_list.push_back("proxy-authorization");
    header_list.push_back("range");
    header_list.push_back("referer");
    header_list.push_back("te");
    header_list.push_back("user-agent");
    header_list.push_back("accept-ranges");
    header_list.push_back("age");
    header_list.push_back("etag");
    header_list.push_back("location");
    header_list.push_back("proxy-authenticate");
    header_list.push_back("retry-after");
    header_list.push_back("server");
    header_list.push_back("vary");
    header_list.push_back("www-authenticate");
    header_list.push_back("allow");
    header_list.push_back("content-encoding");
    header_list.push_back("content-language");
    header_list.push_back("content-length");
    header_list.push_back("content-location");
    header_list.push_back("content-md5");
    header_list.push_back("content-range");
    header_list.push_back("content-type");
    header_list.push_back("expires");
    header_list.push_back("last-modified");
    header_list.push_back("access-control-allow-origin");
    header_list.push_back("access-control-allow-methods");
    header_list.push_back("access-control-allow-headers");
    header_list.push_back("access-control-max-age");
    header_list.push_back("access-control-allow-credentials");
    header_list.push_back("access-control-expose-headers");
    header_list.push_back("access-control-request-method");
    header_list.push_back("access-control-request-headers");
    header_list.push_back("content-disposition");
    header_list.push_back("content-security-policy");
    header_list.push_back("content-security-policy-report-only");
    header_list.push_back("dnt");
    header_list.push_back("forwarded");
    header_list.push_back("origin");
    header_list.push_back("timing-allow-origin");
    header_list.push_back("x-content-type-options");
    header_list.push_back("x-frame-options");
    header_list.push_back("x-xss-protection");
    header_list.push_back("x-forwarded-for");
    header_list.push_back("cookie");
    header_list.push_back("set-cookie");
    header_list.push_back("upgrade-insecure-requests");
    header_list.push_back("sec-fetch-dest");
    header_list.push_back("sec-fetch-mode");
    header_list.push_back("sec-fetch-site");
    header_list.push_back("sec-fetch-user");
    header_list.push_back("priority");
    header_list.push_back("x-secret-header-for-test");

    return header_list;
}

// Helper function to initialize m_string_http_header_map with string
// representations of HTTP headers
std::map<std::string, HttpHeader> HttpHeaderHelper::m_setStringHttpHeaderMap()
{
    std::map<std::string, HttpHeader> string_http_header_map;

    // Add mappings from string representations to HttpHeader enum values
    string_http_header_map[ "cache-control" ] = CACHE_CONTROL;
    string_http_header_map[ "connection" ] = CONNECTION;
    string_http_header_map[ "date" ] = DATE;
    string_http_header_map[ "pragma" ] = PRAGMA;
    string_http_header_map[ "trailer" ] = TRAILER;
    string_http_header_map[ "transfer-encoding" ] = TRANSFER_ENCODING;
    string_http_header_map[ "upgrade" ] = UPGRADE;
    string_http_header_map[ "via" ] = VIA;
    string_http_header_map[ "warning" ] = WARNING;
    string_http_header_map[ "accept" ] = ACCEPT;
    string_http_header_map[ "accept-charset" ] = ACCEPT_CHARSET;
    string_http_header_map[ "accept-encoding" ] = ACCEPT_ENCODING;
    string_http_header_map[ "accept-language" ] = ACCEPT_LANGUAGE;
    string_http_header_map[ "authorization" ] = AUTHORIZATION;
    string_http_header_map[ "expect" ] = EXPECT;
    string_http_header_map[ "from" ] = FROM;
    string_http_header_map[ "host" ] = HOST;
    string_http_header_map[ "if-match" ] = IF_MATCH;
    string_http_header_map[ "if-modified-since" ] = IF_MODIFIED_SINCE;
    string_http_header_map[ "if-none-match" ] = IF_NONE_MATCH;
    string_http_header_map[ "if-range" ] = IF_RANGE;
    string_http_header_map[ "if-unmodified-since" ] = IF_UNMODIFIED_SINCE;
    string_http_header_map[ "max-forwards" ] = MAX_FORWARDS;
    string_http_header_map[ "proxy-authorization" ] = PROXY_AUTHORIZATION;
    string_http_header_map[ "range" ] = RANGE;
    string_http_header_map[ "referer" ] = REFERER;
    string_http_header_map[ "te" ] = TE;
    string_http_header_map[ "user-agent" ] = USER_AGENT;
    string_http_header_map[ "accept-ranges" ] = ACCEPT_RANGES;
    string_http_header_map[ "age" ] = AGE;
    string_http_header_map[ "etag" ] = ETAG;
    string_http_header_map[ "location" ] = LOCATION;
    string_http_header_map[ "proxy-authenticate" ] = PROXY_AUTHENTICATE;
    string_http_header_map[ "retry-after" ] = RETRY_AFTER;
    string_http_header_map[ "server" ] = SERVER;
    string_http_header_map[ "vary" ] = VARY;
    string_http_header_map[ "www-authenticate" ] = WWW_AUTHENTICATE;
    string_http_header_map[ "allow" ] = ALLOW;
    string_http_header_map[ "content-encoding" ] = CONTENT_ENCODING;
    string_http_header_map[ "content-language" ] = CONTENT_LANGUAGE;
    string_http_header_map[ "content-length" ] = CONTENT_LENGTH;
    string_http_header_map[ "content-location" ] = CONTENT_LOCATION;
    string_http_header_map[ "content-md5" ] = CONTENT_MD5;
    string_http_header_map[ "content-range" ] = CONTENT_RANGE;
    string_http_header_map[ "content-type" ] = CONTENT_TYPE;
    string_http_header_map[ "expires" ] = EXPIRES;
    string_http_header_map[ "last-modified" ] = LAST_MODIFIED;
    string_http_header_map[ "access-control-allow-origin" ] =
        ACCESS_CONTROL_ALLOW_ORIGIN;
    string_http_header_map[ "access-control-allow-methods" ] =
        ACCESS_CONTROL_ALLOW_METHODS;
    string_http_header_map[ "access-control-allow-headers" ] =
        ACCESS_CONTROL_ALLOW_HEADERS;
    string_http_header_map[ "access-control-max-age" ] = ACCESS_CONTROL_MAX_AGE;
    string_http_header_map[ "access-control-allow-credentials" ] =
        ACCESS_CONTROL_ALLOW_CREDENTIALS;
    string_http_header_map[ "access-control-expose-headers" ] =
        ACCESS_CONTROL_EXPOSE_HEADERS;
    string_http_header_map[ "access-control-request-method" ] =
        ACCESS_CONTROL_REQUEST_METHOD;
    string_http_header_map[ "access-control-request-headers" ] =
        ACCESS_CONTROL_REQUEST_HEADERS;
    string_http_header_map[ "content-disposition" ] = CONTENT_DISPOSITION;
    string_http_header_map[ "content-security-policy" ] =
        CONTENT_SECURITY_POLICY;
    string_http_header_map[ "content-security-policy-report-only" ] =
        CONTENT_SECURITY_POLICY_REPORT_ONLY;
    string_http_header_map[ "dnt" ] = DNT;
    string_http_header_map[ "forwarded" ] = FORWARDED;
    string_http_header_map[ "origin" ] = ORIGIN;
    string_http_header_map[ "timing-allow-origin" ] = TIMING_ALLOW_ORIGIN;
    string_http_header_map[ "x-content-type-options" ] = X_CONTENT_TYPE_OPTIONS;
    string_http_header_map[ "x-frame-options" ] = X_FRAME_OPTIONS;
    string_http_header_map[ "x-xss-protection" ] = X_XSS_PROTECTION;
    string_http_header_map[ "x-forwarded-for" ] = X_FORWARDED_FOR;
    string_http_header_map[ "x-secret-header-for-test" ] =
        X_SECRET_HEADER_FOR_TEST;
    string_http_header_map[ "cookie" ] = COOKIE;
    string_http_header_map[ "set-cookie" ] = SET_COOKIE;
    string_http_header_map[ "upgrade-insecure-requests" ] =
        UPGRADE_INSECURE_REQUESTS;
    string_http_header_map[ "sec-fetch-dest" ] = SEC_FETCH_DEST;
    string_http_header_map[ "sec-fetch-mode" ] = SEC_FETCH_MODE;
    string_http_header_map[ "sec-fetch-site" ] = SEC_FETCH_SITE;
    string_http_header_map[ "sec-fetch-user" ] = SEC_FETCH_USER;
    string_http_header_map[ "priority" ] = PRIORITY;

    return string_http_header_map;
}

// Helper function to initialize m_http_header_string_map with mappings from
// HttpHeader enum values to string representations
std::map<HttpHeader, std::string> HttpHeaderHelper::m_setHttpHeaderStringMap()
{
    std::map<HttpHeader, std::string> http_header_string_map;

    // Add mappings from HttpHeader enum values to string representations
    http_header_string_map[ CACHE_CONTROL ] = "cache-control";
    http_header_string_map[ CONNECTION ] = "connection";
    http_header_string_map[ DATE ] = "date";
    http_header_string_map[ PRAGMA ] = "pragma";
    http_header_string_map[ TRAILER ] = "trailer";
    http_header_string_map[ TRANSFER_ENCODING ] = "transfer-encoding";
    http_header_string_map[ UPGRADE ] = "upgrade";
    http_header_string_map[ VIA ] = "via";
    http_header_string_map[ WARNING ] = "warning";
    http_header_string_map[ ACCEPT ] = "accept";
    http_header_string_map[ ACCEPT_CHARSET ] = "accept-charset";
    http_header_string_map[ ACCEPT_ENCODING ] = "accept-encoding";
    http_header_string_map[ ACCEPT_LANGUAGE ] = "accept-language";
    http_header_string_map[ AUTHORIZATION ] = "authorization";
    http_header_string_map[ EXPECT ] = "expect";
    http_header_string_map[ FROM ] = "from";
    http_header_string_map[ HOST ] = "host";
    http_header_string_map[ IF_MATCH ] = "if-match";
    http_header_string_map[ IF_MODIFIED_SINCE ] = "if-modified-since";
    http_header_string_map[ IF_NONE_MATCH ] = "if-none-match";
    http_header_string_map[ IF_RANGE ] = "if-range";
    http_header_string_map[ IF_UNMODIFIED_SINCE ] = "if-unmodified-since";
    http_header_string_map[ MAX_FORWARDS ] = "max-forwards";
    http_header_string_map[ PROXY_AUTHORIZATION ] = "proxy-authorization";
    http_header_string_map[ RANGE ] = "range";
    http_header_string_map[ REFERER ] = "referer";
    http_header_string_map[ TE ] = "te";
    http_header_string_map[ USER_AGENT ] = "user-agent";
    http_header_string_map[ ACCEPT_RANGES ] = "accept-ranges";
    http_header_string_map[ AGE ] = "age";
    http_header_string_map[ ETAG ] = "etag";
    http_header_string_map[ LOCATION ] = "location";
    http_header_string_map[ PROXY_AUTHENTICATE ] = "proxy-authenticate";
    http_header_string_map[ RETRY_AFTER ] = "retry-after";
    http_header_string_map[ SERVER ] = "server";
    http_header_string_map[ VARY ] = "vary";
    http_header_string_map[ WWW_AUTHENTICATE ] = "www-authenticate";
    http_header_string_map[ ALLOW ] = "allow";
    http_header_string_map[ CONTENT_ENCODING ] = "content-encoding";
    http_header_string_map[ CONTENT_LANGUAGE ] = "content-language";
    http_header_string_map[ CONTENT_LENGTH ] = "content-length";
    http_header_string_map[ CONTENT_LOCATION ] = "content-location";
    http_header_string_map[ CONTENT_MD5 ] = "content-md5";
    http_header_string_map[ CONTENT_RANGE ] = "content-range";
    http_header_string_map[ CONTENT_TYPE ] = "content-type";
    http_header_string_map[ EXPIRES ] = "expires";
    http_header_string_map[ LAST_MODIFIED ] = "last-modified";
    http_header_string_map[ ACCESS_CONTROL_ALLOW_ORIGIN ] =
        "access-control-allow-origin";
    http_header_string_map[ ACCESS_CONTROL_ALLOW_METHODS ] =
        "access-control-allow-methods";
    http_header_string_map[ ACCESS_CONTROL_ALLOW_HEADERS ] =
        "access-control-allow-headers";
    http_header_string_map[ ACCESS_CONTROL_MAX_AGE ] = "access-control-max-age";
    http_header_string_map[ ACCESS_CONTROL_ALLOW_CREDENTIALS ] =
        "access-control-allow-credentials";
    http_header_string_map[ ACCESS_CONTROL_EXPOSE_HEADERS ] =
        "access-control-expose-headers";
    http_header_string_map[ ACCESS_CONTROL_REQUEST_METHOD ] =
        "access-control-request-method";
    http_header_string_map[ ACCESS_CONTROL_REQUEST_HEADERS ] =
        "access-control-request-headers";
    http_header_string_map[ CONTENT_DISPOSITION ] = "content-disposition";
    http_header_string_map[ CONTENT_SECURITY_POLICY ] =
        "content-security-policy";
    http_header_string_map[ CONTENT_SECURITY_POLICY_REPORT_ONLY ] =
        "content-security-policy-report-only";
    http_header_string_map[ DNT ] = "dnt";
    http_header_string_map[ FORWARDED ] = "forwarded";
    http_header_string_map[ ORIGIN ] = "origin";
    http_header_string_map[ TIMING_ALLOW_ORIGIN ] = "timing-allow-origin";
    http_header_string_map[ X_CONTENT_TYPE_OPTIONS ] = "x-content-type-options";
    http_header_string_map[ X_FRAME_OPTIONS ] = "x-frame-options";
    http_header_string_map[ X_XSS_PROTECTION ] = "x-xss-protection";
    http_header_string_map[ X_FORWARDED_FOR ] = "x-forwarded-for";
    http_header_string_map[ X_SECRET_HEADER_FOR_TEST ] =
        "x-secret-header-for-test";
    http_header_string_map[ COOKIE ] = "cookie";
    http_header_string_map[ SET_COOKIE ] = "set-cookie";
    http_header_string_map[ UPGRADE_INSECURE_REQUESTS ] =
        "upgrade-insecure-requests";
    http_header_string_map[ SEC_FETCH_DEST ] = "sec-fetch-dest";
    http_header_string_map[ SEC_FETCH_MODE ] = "sec-fetch-mode";
    http_header_string_map[ SEC_FETCH_SITE ] = "sec-fetch-site";
    http_header_string_map[ SEC_FETCH_USER ] = "sec-fetch-user";
    http_header_string_map[ PRIORITY ] = "priority";

    return http_header_string_map;
}

// Path: srcs/constants/HttpHeaderHelper.cpp
