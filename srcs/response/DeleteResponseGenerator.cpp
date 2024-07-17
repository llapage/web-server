#include "../../includes/response/DeleteResponseGenerator.hpp"
#include <cstdio>

// Constructor
DeleteResponseGenerator::DeleteResponseGenerator(ILogger &logger)
    : m_logger(logger)
{
}

// Destructor
DeleteResponseGenerator::~DeleteResponseGenerator() {}

// Generate response
Triplet_t DeleteResponseGenerator::generateResponse(
    const IRoute &route, const IRequest &request, IResponse &response,
    IConfiguration &configuration)
{
    // void the unused parameters
    (void)configuration;

    // Get the file path
    std::string root = route.getRoot();
    std::string uri = request.getUri();

    // remove the location path from the uri
    if (route.getPath() != "/")
        uri = uri.substr(route.getPath().size());

    // if root does not end with a slash and uri does not start with a slash
    if (root[ root.size() - 1 ] != '/' && uri[ 0 ] != '/')
    {
        // add a slash to the root
        root += "/";
    }
    std::string file_path = root + uri;

    // Delete the file
    if (remove(file_path.c_str()) != 0)
    {
        // log the error
        m_logger.log(ERROR, "Could not delete file: " + file_path);

        // set the response
        response.setErrorResponse(FORBIDDEN);
    }
    else
    {
        // log the situation
        m_logger.log(VERBOSE, "File deleted: " + file_path);

        // set the response
        response.setErrorResponse(
            NO_CONTENT); // 204 to indicate that the request has been fulfilled
                         // and the response is empty
    }
    // return -1
    return Triplet_t(-1,
                     std::make_pair(0, 0)); // -1 currently just means no CGI
}

// Path: srcs/response/GetResponseGenerator.cpp
