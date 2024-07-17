#ifndef ROUTE_HPP
#define ROUTE_HPP

#include "IResponseGenerator.hpp"
#include "IRoute.hpp"
#include "URIMatcher.hpp"
#include <string>

class Route : public IRoute
{
private:
    const std::string m_path;                // The path of the route (prefix)
    const bool m_is_regex;                   // If the path is a regex
    const std::vector<HttpMethod> m_methods; // The method of the route
    const std::string
        m_root; // The root directory of the route (where the files are stored)
    const std::string
        m_index; // The file to serve if the request uri is a directory
    const std::string
        m_cgi_script; // The script to execute if the request is a CGI
    IResponseGenerator *m_response_generator;
    IURIMatcher *m_matcher;
    const bool m_is_CGI;
    const size_t m_client_max_body_size;
    const std::map<std::string, std::string> m_redirects;
    bool m_autoindex;

public:
    Route(const std::string path, const bool is_regex,
          const std::vector<HttpMethod> methods, const std::string root,
          const std::string index, size_t client_max_body_size,
          const std::map<std::string, std::string> redirects, bool autoindex);
    Route(const std::string path, const bool is_regex,
          const std::vector<HttpMethod> methods, const std::string root,
          const std::string index, const std::string cgi_script,
          IURIMatcher *match, size_t client_max_body_size,
          const std::map<std::string, std::string> redirects, bool autoindex);
    ~Route();
    std::string getPath() const;
    std::string getRoot() const;
    std::string getIndex() const;
    std::string getCgiScript() const;
    size_t getClientMaxBodySize() const;
    bool isAllowedMethod(const HttpMethod method) const;
    bool isRegex() const;
    bool isCGI() const;
    bool isRedirect(const std::string &uri) const;
    bool autoindex() const;
    std::string getRedirect(const std::string &uri) const;
    IResponseGenerator *getResponseGenerator(void) const;
    void setResponseGenerator(IResponseGenerator *generator);
    bool match(const std::string &uri);
};

#endif // ROUTE_HPP
       // Path: includes/response/Route.hpp
