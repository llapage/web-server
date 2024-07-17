#ifndef IREQUESTHANDLER_HPP
#define IREQUESTHANDLER_HPP

#include "../constants/HttpStatusCodeHelper.hpp"

typedef std::pair<int, std::pair<int, int> > Triplet_t;

class IRequestHandler
{
public:
    virtual ~IRequestHandler() {};

    virtual Triplet_t handleRequest(int) = 0;
    virtual int handlePipeException(int) = 0;
    virtual int handlePipeRead(int) = 0;
    virtual void handleErrorResponse(int, int) = 0;
    virtual void handleErrorResponse(int, HttpStatusCode) = 0;
    virtual void handleRedirectResponse(int, std::string) = 0;
    virtual void removeConnection(int) = 0;
    virtual Triplet_t executeCgi(int) = 0;
};

#endif // IREQUESTHANDLER_HPP
// Path: includes/connection/IRequestHandler.hpp
