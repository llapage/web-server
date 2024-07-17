#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

/*
 * RequestParser.hpp
 *
 * The RequestParser class provides functionality for parsing raw HTTP requests
 * and converting them into IRequest objects.
 *
 */

#include "../configuration/IConfiguration.hpp"
#include "../logger/ILogger.hpp"
#include "../request/IRequest.hpp"
#include <string>
#include <vector>

class RequestParser
{
private:
    ILogger &m_logger; // Reference to the error logger
    const IConfiguration
        &m_configuration; // Reference to the server IConfiguration

    // Function to parse the request line of an HTTP request
    void m_parseRequestLine(std::vector<char>::const_iterator &it,
                            const std::vector<char> &raw_request,
                            IRequest &parsed_request) const;

    // Functions to parse the components of the request line
    std::string m_parseMethod(std::vector<char>::const_iterator &it,
                              const std::vector<char> &raw_request) const;
    std::string m_parseUri(std::vector<char>::const_iterator &it,
                           const std::vector<char> &raw_request) const;
    std::string m_parseHttpVersion(std::vector<char>::const_iterator &it,
                                   const std::vector<char> &raw_request) const;

    // Function to parse the headers of an HTTP request
    void m_parseHeaders(std::vector<char>::const_iterator &it,
                        const std::vector<char> &raw_request,
                        IRequest &parsed_request) const;

    // Function to parse an individual header
    void m_parseHeader(std::vector<char>::const_iterator &it,
                       const std::vector<char> &raw_request,
                       IRequest &parsed_request) const;

    // Function to unchunk the body of an HTTP request
    void m_unchunkBody(const std::vector<char> &raw_request,
                       IRequest &request) const;

    // Function to parse a Cookie header
    void m_parseCookie(std::string &cookie_header_value,
                       IRequest &parsed_request) const;

    // Function to check if a character is whitespace
    bool m_isWhitespace(char c) const;

    // Function to check if an iterator points to CRLF (carriage return + line
    // feed)
    bool m_isCRLF(std::vector<char>::const_iterator it) const;

    // Function to check if an iterator points to a character that is in a given
    // set
    bool m_isCharInSet(std::vector<char>::const_iterator it,
                       const std::string &set) const;

    // Function to trim leading and trailing whitespace from a string
    std::string m_trimWhitespace(const std::string &string) const;

    // Function to remove quotes from a string
    void m_removeQuotes(std::string &string) const;

    // Custom getline function to remove carriage returns
    std::istream &m_getlineNoCr(std::istream &is, std::string &line) const;

    // Function to read a stringstream into a char vector until the boundary is
    // found
    void m_assignUntilBoundary(std::istringstream &iss,
                               const std::string &boundary,
                               std::vector<char> &dest) const;

public:
    // Constructor to initialize the RequestParser with required references
    RequestParser(const IConfiguration &configuration, ILogger &logger);

    // Function to parse a raw HTTP request and convert it into a IRequest
    // object
    void parseRequest(IRequest &request) const;
    // Function to parse the body of an HTTP request
    void parseBody(IRequest &parsed_request) const;
    // Function to parse the Upload Chunked Body
    void parseBodyParameters(IRequest &parsed_request) const;
};

#endif // REQUESTPARSER_HPP
// Path: srcs/RequestParser.cpp
