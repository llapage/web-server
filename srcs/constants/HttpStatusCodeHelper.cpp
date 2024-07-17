#include "../../includes/constants/HttpStatusCodeHelper.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include "../../includes/utils/Converter.hpp"
#include <fstream>
#include <sys/fcntl.h>
#include <unistd.h>
#include <vector>

/*
 * HttpStatusCodeHelper.hpp
 *
 * The HttpStatusCodeHelper class provides functionality for working with HTTP
 * status codes. It defines an enum HttpStatusCode to represent standard HTTP
 * status codes and provides methods to retrieve string representations of
 * status codes and vice versa.
 *
 */

// Constructor initializes member variables using helper functions
HttpStatusCodeHelper::HttpStatusCodeHelper(std::vector<std::string> error_page)
    : m_status_code_list(m_setStatusCodeList()),
      m_string_http_status_code_map(m_setStringHttpStatusCodeMap()),
      m_http_status_code_string_map(m_setHttpStatusCodeStringMap()),
      m_status_code_html_page_map(m_setStatusCodeHtmlPageMap(error_page))
{
}

// Get string representation of HttpStatusCode enum value
const std::string &
HttpStatusCodeHelper::httpStatusCodeStringMap(HttpStatusCode status_code) const
{
    if (m_http_status_code_string_map.find(status_code) !=
        m_http_status_code_string_map.end())
    {
        return m_http_status_code_string_map.at(status_code);
    }
    else
    {
        throw UnknownHttpStatusCodeError();
    }
}

// Get HttpStatusCode enum value from string representation
HttpStatusCode HttpStatusCodeHelper::stringHttpStatusCodeMap(
    const std::string &status_code) const
{
    if (m_string_http_status_code_map.find(status_code) !=
        m_string_http_status_code_map.end())
    {
        return m_string_http_status_code_map.at(status_code);
    }
    else
    {
        throw UnknownHttpStatusCodeError(status_code);
    }
}

// Get HttpStatusCode enum value from string representation
HttpStatusCode
HttpStatusCodeHelper::intHttpStatusCodeMap(const int &status_code) const
{
    if (status_code >= 100 && status_code <= 511)
    {
        return static_cast<HttpStatusCode>(status_code);
    }
    else
    {
        throw UnknownHttpStatusCodeError(Converter::toString(status_code));
    }
}

// Generate a status line string for an HTTP response
std::string
HttpStatusCodeHelper::getStatusLine(HttpStatusCode status_code) const
{
    return "HTTP/1.1 " + Converter::toString(static_cast<size_t>(status_code)) +
           " " + this->httpStatusCodeStringMap(status_code) + "\r\n";
}

// Generate a complete error response for an HTTP status code
std::string
HttpStatusCodeHelper::getErrorResponse(HttpStatusCode status_code) const
{
    std::string body = this->getHtmlPage(status_code);

    return this->getStatusLine(status_code) + "\n" +
           "content-type: text/html\r\n" +
           "content-length: " + Converter::toString(body.length()) + "\r\n" +
           "Connection: close\r\n" + "Server: webserv/1.0\r\n" + "\r\n" + body;
}

// Generate an HTML page with the specified HTTP status code
std::string HttpStatusCodeHelper::getHtmlPage(HttpStatusCode status_code) const
{
    // If a custom HTML page is available for the status code, return it
    if (m_status_code_html_page_map.find(status_code) !=
        m_status_code_html_page_map.end())
    {
        return m_status_code_html_page_map.at(status_code);
    }

    // Otherwise, generate a default HTML page for the status code
    // Create a string stream to build the HTML page.
    std::stringstream html_page;

    // Write the HTML page
    html_page
        << "<!DOCTYPE html>\n"
           "<html lang=\"en\">\n"
           "<head>\n"
           "    <meta charset=\"UTF-8\">\n"
           "    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n"
           "    <meta name=\"viewport\" content=\"width=device-width, "
           "initial-scale=1.0\">\n"
           "    <title>Webserv | "
        << static_cast<size_t>(status_code)
        << "</title>\n"
           "    <style>\n"
           "        body {\n"
           "            font-family: Arial, sans-serif;\n"
           "            background-color: #333;\n"
           "            color: #f4f4f4;\n"
           "            margin: 0;\n"
           "            padding: 0;\n"
           "            display: flex;\n"
           "            align-items: center;\n"
           "            justify-content: center;\n"
           "            height: 100vh;\n"
           "        }\n"
           "        .container {\n"
           "            width: 80%;\n"
           "            text-align: center;\n"
           "            position: relative;\n"
           "        }\n"
           "        .error {\n"
           "            background: #444;\n"
           "            border: #333 solid 1px;\n"
           "            padding: 20px;\n"
           "            position: relative;\n"
           "        }\n"
           "        .error h1 {\n"
           "            color: #f90;\n"
           "            font-size: 33px;\n"
           "            margin-bottom: 5px;\n"
           "        }\n"
           "        .error p {\n"
           "            font-size: 21px;\n"
           "        }\n"
           "        .version {\n"
           "            position: absolute;\n"
           "            bottom: 5px;\n"
           "            right: 5px;\n"
           "            font-size: 14px;\n"
           "            color: #999;\n"
           "        }\n"
           "    </style>\n"
           "</head>\n"
           "<body>\n"
           "    <div class=\"container\">\n"
           "        <div class=\"error\">\n"
           "            <h1>Error <span style=\"color: #f90;\">"
        << static_cast<size_t>(status_code)
        << "</span></h1>\n"
           "            <p> "
        << m_http_status_code_string_map.at(status_code)
        << "</p>\n"
           "            <div class=\"version\">webserv/1.0</div>\n"
           "        </div>\n"
           "    </div>\n"
           "</body>\n"
           "</html>\n";

    // Return the generated HTML page.
    return html_page.str();
}

// Helper function to initialize m_status_code_list with string representations
// of HTTP status codes
std::vector<std::string> HttpStatusCodeHelper::m_setStatusCodeList()
{
    std::vector<std::string> status_code_list;

    // Add string representations of HTTP status codes to status_code_list
    status_code_list.push_back("100 Continue");
    status_code_list.push_back("101 Switching Protocols");
    status_code_list.push_back("102 Processing");
    status_code_list.push_back("103 Early Hints");
    status_code_list.push_back("200 OK");
    status_code_list.push_back("201 Created");
    status_code_list.push_back("202 Accepted");
    status_code_list.push_back("203 Non-Authoritative Information");
    status_code_list.push_back("204 No Content");
    status_code_list.push_back("205 Reset Content");
    status_code_list.push_back("206 Partial Content");
    status_code_list.push_back("207 Multi-Status");
    status_code_list.push_back("208 Already Reported");
    status_code_list.push_back("226 IM Used");
    status_code_list.push_back("300 Multiple Choices");
    status_code_list.push_back("301 Moved Permanently");
    status_code_list.push_back("302 Found");
    status_code_list.push_back("303 See Other");
    status_code_list.push_back("304 Not Modified");
    status_code_list.push_back("305 Use Proxy");
    status_code_list.push_back("306 Switch Proxy");
    status_code_list.push_back("307 Temporary Redirect");
    status_code_list.push_back("308 Permanent Redirect");
    status_code_list.push_back("400 Bad Request");
    status_code_list.push_back("401 Unauthorized");
    status_code_list.push_back("402 Payment Required");
    status_code_list.push_back("403 Forbidden");
    status_code_list.push_back("404 Not Found");
    status_code_list.push_back("405 Method Not Allowed");
    status_code_list.push_back("406 Not Acceptable");
    status_code_list.push_back("407 Proxy Authentication Required");
    status_code_list.push_back("408 Request Timeout");
    status_code_list.push_back("409 Conflict");
    status_code_list.push_back("410 Gone");
    status_code_list.push_back("411 Length Required");
    status_code_list.push_back("412 Precondition Failed");
    status_code_list.push_back("413 Payload Too Large");
    status_code_list.push_back("414 URI Too Long");
    status_code_list.push_back("415 Unsupported Media Type");
    status_code_list.push_back("416 Range Not Satisfiable");
    status_code_list.push_back("417 Expectation Failed");
    status_code_list.push_back("418 I'm a teapot");
    status_code_list.push_back("421 Misdirected Request");
    status_code_list.push_back("422 Unprocessable Entity");
    status_code_list.push_back("423 Locked");
    status_code_list.push_back("424 Failed Dependency");
    status_code_list.push_back("425 Too Early");
    status_code_list.push_back("426 Upgrade Required");
    status_code_list.push_back("428 Precondition Required");
    status_code_list.push_back("429 Too Many Requests");
    status_code_list.push_back("431 Request Header Fields Too Large");
    status_code_list.push_back("451 Unavailable For Legal Reasons");
    status_code_list.push_back("500 Internal Server Error");
    status_code_list.push_back("501 Not Implemented");
    status_code_list.push_back("502 Bad Gateway");
    status_code_list.push_back("503 Service Unavailable");
    status_code_list.push_back("504 Gateway Timeout");
    status_code_list.push_back("505 HTTP Version Not Supported");
    status_code_list.push_back("506 Variant Also Negotiates");
    status_code_list.push_back("507 Insufficient Storage");
    status_code_list.push_back("508 Loop Detected");
    status_code_list.push_back("510 Not Extended");
    status_code_list.push_back("511 Network Authentication Required");

    return status_code_list;
}

// Helper function to initialize m_string_http_status_code_map with mappings
// from string representations to HttpStatusCode enum values
std::map<std::string, HttpStatusCode>
HttpStatusCodeHelper::m_setStringHttpStatusCodeMap()
{
    std::map<std::string, HttpStatusCode> string_http_status_code_map;

    // Add mappings from string representations to HttpStatusCode enum values
    string_http_status_code_map[ "100" ] = CONTINUE;
    string_http_status_code_map[ "101" ] = SWITCHING_PROTOCOLS;
    string_http_status_code_map[ "102" ] = PROCESSING;
    string_http_status_code_map[ "103" ] = EARLY_HINTS;
    string_http_status_code_map[ "200" ] = OK;
    string_http_status_code_map[ "201" ] = CREATED;
    string_http_status_code_map[ "202" ] = ACCEPTED;
    string_http_status_code_map[ "203" ] = NON_AUTHORITATIVE_INFORMATION;
    string_http_status_code_map[ "204" ] = NO_CONTENT;
    string_http_status_code_map[ "205" ] = RESET_CONTENT;
    string_http_status_code_map[ "206" ] = PARTIAL_CONTENT;
    string_http_status_code_map[ "207" ] = MULTI_STATUS;
    string_http_status_code_map[ "208" ] = ALREADY_REPORTED;
    string_http_status_code_map[ "226" ] = IM_USED;
    string_http_status_code_map[ "300" ] = MULTIPLE_CHOICES;
    string_http_status_code_map[ "301" ] = MOVED_PERMANENTLY;
    string_http_status_code_map[ "302" ] = FOUND;
    string_http_status_code_map[ "303" ] = SEE_OTHER;
    string_http_status_code_map[ "304" ] = NOT_MODIFIED;
    string_http_status_code_map[ "305" ] = USE_PROXY;
    string_http_status_code_map[ "306" ] = SWITCH_PROXY;
    string_http_status_code_map[ "307" ] = TEMPORARY_REDIRECT;
    string_http_status_code_map[ "308" ] = PERMANENT_REDIRECT;
    string_http_status_code_map[ "400" ] = BAD_REQUEST;
    string_http_status_code_map[ "401" ] = UNAUTHORIZED;
    string_http_status_code_map[ "402" ] = PAYMENT_REQUIRED;
    string_http_status_code_map[ "403" ] = FORBIDDEN;
    string_http_status_code_map[ "404" ] = NOT_FOUND;
    string_http_status_code_map[ "405" ] = METHOD_NOT_ALLOWED;
    string_http_status_code_map[ "406" ] = NOT_ACCEPTABLE;
    string_http_status_code_map[ "407" ] = PROXY_AUTHENTICATION_REQUIRED;
    string_http_status_code_map[ "408" ] = REQUEST_TIMEOUT;
    string_http_status_code_map[ "409" ] = CONFLICT;
    string_http_status_code_map[ "410" ] = GONE;
    string_http_status_code_map[ "411" ] = LENGTH_REQUIRED;
    string_http_status_code_map[ "412" ] = PRECONDITION_FAILED;
    string_http_status_code_map[ "413" ] = PAYLOAD_TOO_LARGE;
    string_http_status_code_map[ "414" ] = URI_TOO_LONG;
    string_http_status_code_map[ "415" ] = UNSUPPORTED_MEDIA_TYPE;
    string_http_status_code_map[ "416" ] = RANGE_NOT_SATISFIABLE;
    string_http_status_code_map[ "417" ] = EXPECTATION_FAILED;
    string_http_status_code_map[ "418" ] = IM_A_TEAPOT;
    string_http_status_code_map[ "421" ] = MISDIRECTED_REQUEST;
    string_http_status_code_map[ "422" ] = UNPROCESSABLE_ENTITY;
    string_http_status_code_map[ "423" ] = LOCKED;
    string_http_status_code_map[ "424" ] = FAILED_DEPENDENCY;
    string_http_status_code_map[ "425" ] = TOO_EARLY;
    string_http_status_code_map[ "426" ] = UPGRADE_REQUIRED;
    string_http_status_code_map[ "428" ] = PRECONDITION_REQUIRED;
    string_http_status_code_map[ "429" ] = TOO_MANY_REQUESTS;
    string_http_status_code_map[ "431" ] = REQUEST_HEADER_FIELDS_TOO_LARGE;
    string_http_status_code_map[ "451" ] = UNAVAILABLE_FOR_LEGAL_REASONS;
    string_http_status_code_map[ "500" ] = INTERNAL_SERVER_ERROR;
    string_http_status_code_map[ "501" ] = NOT_IMPLEMENTED;
    string_http_status_code_map[ "502" ] = BAD_GATEWAY;
    string_http_status_code_map[ "503" ] = SERVICE_UNAVAILABLE;
    string_http_status_code_map[ "504" ] = GATEWAY_TIMEOUT;
    string_http_status_code_map[ "505" ] = HTTP_VERSION_NOT_SUPPORTED;
    string_http_status_code_map[ "506" ] = VARIANT_ALSO_NEGOTIATES;
    string_http_status_code_map[ "507" ] = INSUFFICIENT_STORAGE;
    string_http_status_code_map[ "508" ] = LOOP_DETECTED;
    string_http_status_code_map[ "510" ] = NOT_EXTENDED;
    string_http_status_code_map[ "511" ] = NETWORK_AUTHENTICATION_REQUIRED;

    return string_http_status_code_map;
}

// Helper function to initialize m_http_status_code_string_map with mappings
// from HttpStatusCode enum values to string representations
std::map<HttpStatusCode, std::string>
HttpStatusCodeHelper::m_setHttpStatusCodeStringMap()
{
    std::map<HttpStatusCode, std::string> http_status_code_string_map;

    // Add mappings from HttpStatusCode enum values to string representations
    http_status_code_string_map[ CONTINUE ] = "Continue";
    http_status_code_string_map[ SWITCHING_PROTOCOLS ] = "Switching Protocols";
    http_status_code_string_map[ PROCESSING ] = "Processing";
    http_status_code_string_map[ EARLY_HINTS ] = "Early Hints";
    http_status_code_string_map[ OK ] = "OK";
    http_status_code_string_map[ CREATED ] = "Created";
    http_status_code_string_map[ ACCEPTED ] = "Accepted";
    http_status_code_string_map[ NON_AUTHORITATIVE_INFORMATION ] =
        "Non-Authoritative Information";
    http_status_code_string_map[ NO_CONTENT ] = "No Content";
    http_status_code_string_map[ RESET_CONTENT ] = "Reset Content";
    http_status_code_string_map[ PARTIAL_CONTENT ] = "Partial Content";
    http_status_code_string_map[ MULTI_STATUS ] = "Multi-Status";
    http_status_code_string_map[ ALREADY_REPORTED ] = "Already Reported";
    http_status_code_string_map[ IM_USED ] = "IM Used";
    http_status_code_string_map[ MULTIPLE_CHOICES ] = "Multiple Choices";
    http_status_code_string_map[ MOVED_PERMANENTLY ] = "Moved Permanently";
    http_status_code_string_map[ FOUND ] = "Found";
    http_status_code_string_map[ SEE_OTHER ] = "See Other";
    http_status_code_string_map[ NOT_MODIFIED ] = "Not Modified";
    http_status_code_string_map[ USE_PROXY ] = "Use Proxy";
    http_status_code_string_map[ SWITCH_PROXY ] = "Switch Proxy";
    http_status_code_string_map[ TEMPORARY_REDIRECT ] = "Temporary Redirect";
    http_status_code_string_map[ PERMANENT_REDIRECT ] = "Permanent Redirect";
    http_status_code_string_map[ BAD_REQUEST ] = "Bad Request";
    http_status_code_string_map[ UNAUTHORIZED ] = "Unauthorized";
    http_status_code_string_map[ PAYMENT_REQUIRED ] = "Payment Required";
    http_status_code_string_map[ FORBIDDEN ] = "Forbidden";
    http_status_code_string_map[ NOT_FOUND ] = "Not Found";
    http_status_code_string_map[ METHOD_NOT_ALLOWED ] = "Method Not Allowed";
    http_status_code_string_map[ NOT_ACCEPTABLE ] = "Not Acceptable";
    http_status_code_string_map[ PROXY_AUTHENTICATION_REQUIRED ] =
        "Proxy Authentication Required";
    http_status_code_string_map[ REQUEST_TIMEOUT ] = "Request Timeout";
    http_status_code_string_map[ CONFLICT ] = "Conflict";
    http_status_code_string_map[ GONE ] = "Gone";
    http_status_code_string_map[ LENGTH_REQUIRED ] = "Length Required";
    http_status_code_string_map[ PRECONDITION_FAILED ] = "Precondition Failed";
    http_status_code_string_map[ PAYLOAD_TOO_LARGE ] = "Payload Too Large";
    http_status_code_string_map[ URI_TOO_LONG ] = "URI Too Long";
    http_status_code_string_map[ UNSUPPORTED_MEDIA_TYPE ] =
        "Unsupported Media Type";
    http_status_code_string_map[ RANGE_NOT_SATISFIABLE ] =
        "Range Not Satisfiable";
    http_status_code_string_map[ EXPECTATION_FAILED ] = "Expectation Failed";
    http_status_code_string_map[ IM_A_TEAPOT ] = "I'm a teapot";
    http_status_code_string_map[ MISDIRECTED_REQUEST ] = "Misdirected Request";
    http_status_code_string_map[ UNPROCESSABLE_ENTITY ] =
        "Unprocessable Entity";
    http_status_code_string_map[ LOCKED ] = "Locked";
    http_status_code_string_map[ FAILED_DEPENDENCY ] = "Failed Dependency";
    http_status_code_string_map[ TOO_EARLY ] = "Too Early";
    http_status_code_string_map[ UPGRADE_REQUIRED ] = "Upgrade Required";
    http_status_code_string_map[ PRECONDITION_REQUIRED ] =
        "Precondition Required";
    http_status_code_string_map[ TOO_MANY_REQUESTS ] = "Too Many Requests";
    http_status_code_string_map[ REQUEST_HEADER_FIELDS_TOO_LARGE ] =
        "Request Header Fields Too Large";
    http_status_code_string_map[ UNAVAILABLE_FOR_LEGAL_REASONS ] =
        "Unavailable For Legal Reasons";
    http_status_code_string_map[ INTERNAL_SERVER_ERROR ] =
        "Internal Server Error";
    http_status_code_string_map[ NOT_IMPLEMENTED ] = "Not Implemented";
    http_status_code_string_map[ BAD_GATEWAY ] = "Bad Gateway";
    http_status_code_string_map[ SERVICE_UNAVAILABLE ] = "Service Unavailable";
    http_status_code_string_map[ GATEWAY_TIMEOUT ] = "Gateway Timeout";
    http_status_code_string_map[ HTTP_VERSION_NOT_SUPPORTED ] =
        "HTTP Version Not Supported";
    http_status_code_string_map[ VARIANT_ALSO_NEGOTIATES ] =
        "Variant Also Negotiates";
    http_status_code_string_map[ INSUFFICIENT_STORAGE ] =
        "Insufficient Storage";
    http_status_code_string_map[ LOOP_DETECTED ] = "Loop Detected";
    http_status_code_string_map[ NOT_EXTENDED ] = "Not Extended";
    http_status_code_string_map[ NETWORK_AUTHENTICATION_REQUIRED ] =
        "Network Authentication Required";

    return http_status_code_string_map;
}

// Sets the status code to html page mapping
std::map<HttpStatusCode, std::string>
HttpStatusCodeHelper::m_setStatusCodeHtmlPageMap(
    std::vector<std::string> error_page)
{
    // e.g. error_page = {"500", "502", "503", "504", "/50x.html", "404",
    // "/404.html"};
    std::vector<std::string>::iterator it = error_page.begin();
    std::map<HttpStatusCode, std::string> status_code_html_page_map;

    // Iterate through the error_page restarting the loop after each html page
    // path
    while (it != error_page.end())
    {
        // Stack the error codes until we reach the html page path
        std::vector<std::string> stack;
        while (it->data()[ 0 ] != '/')
        {
            stack.push_back(it->data());
            it++;
        }

        // open the file
        std::string path = it->data();
        path.insert(0, ".");
        std::ifstream file_stream(path.c_str());

        // check if the file is open
        if (!file_stream.is_open())
            throw ErrorPageOpenError();

        // read the file contents
        std::string line;
        std::string html_page;
        while (std::getline(file_stream, line))
            html_page += line;

        // close the file
        file_stream.close();

        // pop the error codes from the stack and map them to the html page
        while (!stack.empty())
        {
            std::string status_code_string = stack.back();
            HttpStatusCode http_status_code =
                stringHttpStatusCodeMap(stack.back());
            status_code_html_page_map[ http_status_code ] = html_page;
            stack.pop_back();
        }
        it++;
    }

    return status_code_html_page_map;
}

// Path: includes/constants/HttpStatusCodeHelper.hpp
