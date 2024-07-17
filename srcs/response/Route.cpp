#include "../../includes/response/Route.hpp"

// Constructor
Route::Route(const std::string path, const bool is_regex,
             const std::vector<HttpMethod> methods, const std::string root,
             const std::string index, const std::string cgi_script,
             IURIMatcher *matcher, size_t client_max_body_size,
             const std::map<std::string, std::string> redirects, bool autoindex)
    : m_path(path), m_is_regex(is_regex), m_methods(methods), m_root(root),
      m_index(index), m_cgi_script(cgi_script), m_matcher(matcher),
      m_is_CGI(true), m_client_max_body_size(client_max_body_size),
      m_redirects(redirects), m_autoindex(autoindex)
{
}

Route::Route(const std::string path, const bool is_regex,
             const std::vector<HttpMethod> methods, const std::string root,
             const std::string index, size_t client_max_body_size,
             const std::map<std::string, std::string> redirects, bool autoindex)
    : m_path(path), m_is_regex(is_regex), m_methods(methods), m_root(root),
      m_index(index), m_cgi_script(""), m_matcher(NULL), m_is_CGI(false),
      m_client_max_body_size(client_max_body_size), m_redirects(redirects),
      m_autoindex(autoindex)
{
}

// Destructor
Route::~Route() {}

// Get the path
std::string Route::getPath() const { return m_path; }

// Get the root
std::string Route::getRoot() const { return m_root; }

// Get the index
std::string Route::getIndex() const { return m_index; }

// Get the CGI script
std::string Route::getCgiScript() const { return m_cgi_script; }

size_t Route::getClientMaxBodySize() const { return m_client_max_body_size; }

// Check if the method is allowed
bool Route::isAllowedMethod(const HttpMethod method) const
{
    for (std::vector<HttpMethod>::const_iterator it = m_methods.begin();
         it != m_methods.end(); ++it)
    {
        if (*it == method)
            return true;
    }
    return false;
}

// Check if the path is a regex
bool Route::isRegex() const { return m_is_regex; }
bool Route::isCGI() const { return m_is_CGI; }

// Check if the uri is a redirect
bool Route::isRedirect(const std::string &uri) const
{
    for (std::map<std::string, std::string>::const_iterator it =
             m_redirects.begin();
         it != m_redirects.end(); ++it)
    {
        if (uri.find(it->first) != std::string::npos)
            return true;
    }
    return false;
}

bool Route::autoindex() const { return m_autoindex; }

// Get the redirect
std::string Route::getRedirect(const std::string &uri) const
{
    for (std::map<std::string, std::string>::const_iterator it =
             m_redirects.begin();
         it != m_redirects.end(); ++it)
    {
        if (uri.find(it->first) != std::string::npos)
            return it->second;
    }
    return "";
}

// Get the response generator
IResponseGenerator *Route::getResponseGenerator() const
{
    return m_response_generator;
}

void Route::setResponseGenerator(IResponseGenerator *generator)
{
    m_response_generator = generator;
}

#include <iostream>
bool Route::match(const std::string &uri)
{
    //(void)m_matcher;
    // return uri.find(m_path) != std::string::npos;

    // std::cout << "Matching uri " << uri << " with path " << m_path <<
    // std::endl;
    if (!m_matcher)
    {
        return uri.find(m_path) != std::string::npos;
    }
    // std::cout << "Using m_matcher" << std::endl;
    return m_matcher->match(uri);
}
