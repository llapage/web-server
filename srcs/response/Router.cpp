#include "../../includes/response/Router.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include "../../includes/response/DeleteResponseGenerator.hpp"
#include "../../includes/response/RFCCgiResponseGenerator.hpp"
#include "../../includes/response/Route.hpp"
#include "../../includes/response/StaticFileResponseGenerator.hpp"
#include "../../includes/response/UploadResponseGenerator.hpp"
#include "../../includes/utils/Converter.hpp"
#include <algorithm>
#include <cstdlib>
#include <string>

/*Router: Selects the right 'Route' and 'ResponseGenerator' based on URI
(etc.) in the 'IRequest' (each locationblock in the conf file corresponds to a
'Route', ie the Router selects the correct locationblock)*/

// Constructor
Router::Router(IConfiguration &configuration, ILogger &logger)
    : m_configuration(configuration), m_logger(logger),
      m_http_helper(HttpHelper(configuration))
{
    // Log the creation of the Router
    m_logger.log(VERBOSE, "Initializing Router...");

    // Create the response generators
    m_response_generators[ "GET" ] = new StaticFileResponseGenerator(logger);
    m_response_generators[ "POST" ] = new UploadResponseGenerator(logger);
    m_response_generators[ "PUT" ] = new UploadResponseGenerator(logger);
    m_response_generators[ "DELETE" ] = new DeleteResponseGenerator(logger);
    // m_response_generators["CGI"] = NULL;

    const BlockList &servers =
        configuration.getBlocks("http")[ 0 ]->getBlocks("server");
    // first server is the root configuration and is the default one.
    m_servers.push_back(&m_configuration);
    m_routes.push_back(new std::vector<IRoute *>());
    m_createRoutes(*servers[ 0 ], *m_routes[ 0 ]);
    for (size_t i = 1; i < servers.size() + 1; i++)
    {
        m_servers.push_back(servers[ i ]);
        m_routes.push_back(new std::vector<IRoute *>());
        m_createRoutes(*servers[ i ], *m_routes[ i ]);
    }
}

// Destructor
Router::~Router()
{
    // Log the destruction of the Router
    m_logger.log(VERBOSE, "Router destroyed.");

    // Delete the ResponseGenerators
    std::map<std::string, IResponseGenerator *>::iterator it;
    for (it = m_response_generators.begin(); it != m_response_generators.end();
         it++)
    {
        delete it->second;
    }

    // Delete the Routes
    for (size_t i = 0; i < m_routes.size(); i++)
    {
        for (size_t j = 0; j < m_routes[ i ]->size(); j++)
        {
            delete m_routes[ i ]->at(j);
        }
        delete m_routes[ i ];
    }
}

IRoute *Router::getRoute(IRequest *request, IResponse *response)
{
    std::string server_name = request->getHostName();
    std::string server_port = request->getHostPort();
    std::vector<IRoute *> *routes = m_routes[ 0 ];

    // match servers.
    size_t i;
    for (i = 1; i < m_servers.size(); i++)
    {
        if (server_name == m_servers[ i ]->getString("server_name") &&
            server_port == m_servers[ i ]->getString("listen"))
        {
            routes = m_routes[ i ];
        }
    }

    IRoute *route = routes->at(routes->size() - 1); // Default route
    size_t routes_stop = routes->size();
    std::string uri = request->getUri();
    std::string method_str =
        m_http_helper.httpMethodStringMap(request->getMethod());
    IResponseGenerator *response_generator =
        m_response_generators[ method_str ];
    size_t body_size = request->getBody().size();

    // Match the request to a route
    (void)response;

    // Match the request to a route
    HttpMethod method = request->getMethod();

    // search for a route that matches the request.
    for (size_t i = 0; i < routes_stop; i++)
    {
        if ((routes->at(i)->match(uri) &&
             routes->at(i)->isAllowedMethod(method) &&
             body_size <= routes->at(i)->getClientMaxBodySize()) ||
            routes->at(i)->getPath() == request->getUri())
        {
            if (body_size > routes->at(i)->getClientMaxBodySize())
            {
                throw HttpStatusCodeException(PAYLOAD_TOO_LARGE);
            }
            if (routes->at(i)->isAllowedMethod(method) == false)
            {
                throw HttpStatusCodeException(METHOD_NOT_ALLOWED);
            }
            if (routes->at(i)->isRedirect(request->getUri()))
            {
                throw HttpRedirectException(
                    routes->at(i)->getRedirect(request->getUri()));
            }
            // return cgi directly since it already has a response generator
            if (routes->at(i)->isCGI())
            {
                return routes->at(i);
            }
            routes->at(i)->setResponseGenerator(response_generator);
            return routes->at(i);
        }
    }

    route = routes->at(routes->size() - 1); // Default route
    if (route->isAllowedMethod(method) == false)
    {
        throw HttpStatusCodeException(METHOD_NOT_ALLOWED);
    }
    route->setResponseGenerator(response_generator);
    return route;
}

// Sort Routes; regex first, then by path length in descending order
bool Router::m_sortRoutes(const IRoute *a, const IRoute *b)
{
    if (a->isRegex() && !b->isRegex())
        return true;
    if (!a->isRegex() && b->isRegex())
        return false;
    return a->getPath().length() > b->getPath().length();
}

void Router::m_createRoutes(IConfiguration &server,
                            std::vector<IRoute *> &routes)
{
    // Create a route for each location block
    const BlockList &locations_list = server.getBlocks("location");
    for (size_t i = 0; i < locations_list.size(); i++)
    {
        std::string path;
        bool is_regex = false;
        std::vector<HttpMethod> methods;
        std::string root;
        std::string index;
        std::string cgi_script;
        size_t client_max_body_size;
        std::map<std::string, std::string> redirects;
        bool autoindex;

        // Get the path
        std::vector<std::string> &location_params =
            locations_list[ i ]->getParameters();
        path = location_params[ 0 ];

        // set is_regex
        is_regex = locations_list[ i ]->isRegex();

        // remove trailing slash
        if (path.length() > 1 && path[ path.length() - 1 ] == '/')
            path = path.substr(0, path.length() - 1);

        // Get the Methods
        std::vector<std::string> &method_vector =
            locations_list[ i ]
                ->getBlocks("limit_except")[ 0 ]
                ->getParameters();
        std::string methods_string;
        for (size_t j = 0; j < method_vector.size(); j++)
            methods_string += method_vector[ j ] + " ";

        for (size_t j = 0; j < method_vector.size(); j++)
            methods.push_back(
                m_http_helper.stringHttpMethodMap(method_vector[ j ]));

        // Get the root
        const std::vector<std::string> &root_vector =
            locations_list[ i ]->getStringVector("root");
        if (root_vector.size() == 0)
            root = locations_list[ i ]->getString("root");
        else
            root = root_vector[ 0 ];

        // Get the index
        const std::vector<std::string> &index_vector =
            locations_list[ i ]->getStringVector("index");
        if (index_vector.size() == 0)
            index = locations_list[ i ]->getString("index");
        else
            index = index_vector[ 0 ];

        // Get the max body size
        client_max_body_size =
            locations_list[ i ]->getSize_t("client_max_body_size");

        // Get the redirects
        std::vector<std::string> redirects_vector =
            locations_list[ i ]->getStringVector("rewrite");
        for (size_t q = 0; q < redirects_vector.size() / 2; q++)
            redirects[ redirects_vector[ q * 2 ] ] =
                redirects_vector[ q * 2 + 1 ];

        // Get the autoindex
        autoindex = locations_list[ i ]->getBool("autoindex");

        // add cgi's
        const BlockList &cgis = locations_list[ i ]->getBlocks("cgi");
        // if there is any CGI in the file, check if it is active and create it
        // if it does not already exist. note: a cgi response generator is
        // mapped to its path. add cgi routes
        Route *route;
        IResponseGenerator *cgi_rg;
        IURIMatcher *matcher;
        // marks the location as cgi
        bool cgi_route = false;
        for (size_t j = 0; j < cgis.size(); j++)
        {
            const std::string &cgi_path = cgis[ j ]->getString("bin_path");
            const std::vector<std::string> &cgi_target =
                cgis[ j ]->getParameters();
            const std::string &cgi_type = cgis[ j ]->getString("cgi_type");
            if (cgi_path == "none" || cgi_target[ 0 ] == "none" ||
                cgi_type == "none")
            {
                continue;
            }
            cgi_route = true;
            std::map<std::string, IResponseGenerator *>::iterator itr =
                m_response_generators.find(cgi_path);
            // create or retrieve a CGI response generator
            if (itr == m_response_generators.end())
            {
                cgi_rg = m_createCGIResponseGenerator(
                    cgis[ j ]->getString("cgi_type"), cgi_path, m_logger);
                m_response_generators[ cgi_path ] = cgi_rg;
            }
            else
            {
                cgi_rg = itr->second;
            }
            // create and cache if no matcher exists
            if (m_uri_matchers.find(cgi_path) == m_uri_matchers.end())
            {
                if (cgi_target.size() != 0)
                    matcher = new ExtensionMatcher(cgi_target);
                else
                    matcher = new DefaultMatcher(cgi_path);
                m_uri_matchers[ cgi_path ] = matcher;
            }
            else
            {
                matcher = m_uri_matchers[ cgi_path ];
            }
            m_uri_matchers[ cgi_path ] = matcher;
            route =
                new Route(path, is_regex, methods, root, index, cgi_path,
                          matcher, client_max_body_size, redirects, autoindex);
            m_logger.log(VERBOSE, "[Router] New location: '" + path +
                                      "',  methods: '" + methods_string +
                                      "', root: '" + root + "', index: '" +
                                      index + "', cgi script: '" + cgi_script +
                                      "'." + "CGI" +
                                      server.getString("server_name"));
            route->setResponseGenerator(cgi_rg);
            routes.push_back(route);
            m_response_generators[ cgi_path ] = cgi_rg;
        }
        if (!cgi_route)
        {
            m_logger.log(VERBOSE, "[Router] New location: '" + path +
                                      "',  methods: '" + methods_string +
                                      "', root: '" + root + "', index: '" +
                                      index + "', cgi script: '" + cgi_script +
                                      "'.");
            route = new Route(path, is_regex, methods, root, index,
                              client_max_body_size, redirects, autoindex);
            // route->setResponseGenerator(m_response_generators["GET"]);
            routes.push_back(route);
        }
    }

    // Sort the routes
    std::sort(routes.begin(), routes.end(), m_sortRoutes);

    // print all the route paths
    for (size_t i = 0; i < routes.size(); i++)
    {
        m_logger.log(VERBOSE,
                     "[Router] Route path: '" + routes[ i ]->getPath() + "'.");
    }
}

Triplet_t Router::execRoute(IRoute *route, IRequest *request,
                            IResponse *response)
{
    IResponseGenerator *response_generator = route->getResponseGenerator();
    // Generate the response
    Triplet_t return_value = response_generator->generateResponse(
        *route, *request, *response, m_configuration);

    // print return value
    m_logger.log(DEBUG,
                 "Return value: " + Converter::toString(return_value.first) +
                     " " + Converter::toString(return_value.second.first) +
                     " " + Converter::toString(return_value.second.second));

    // return the return value
    return return_value;
}

IResponseGenerator *Router::m_createCGIResponseGenerator(
    const std::string &type, const std::string &cgi_path, ILogger &logger)
{
    if (type == "file")
    {
        return new RFCCgiResponseGenerator(logger, cgi_path, true);
    }
    // default
    return new RFCCgiResponseGenerator(logger, cgi_path);
}
