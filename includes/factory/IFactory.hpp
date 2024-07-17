#ifndef IFACTORY_HPP
#define IFACTORY_HPP

#include "../connection/IConnection.hpp"
#include "../connection/ISession.hpp"
#include "../request/IRequest.hpp"
#include "../response/IResponse.hpp"

class IFactory
{
public:
    virtual ~IFactory() {};

    virtual IConnection *createConnection(
        std::pair<int, std::pair<std::string, std::string> >) = 0;
    virtual IRequest *createRequest() = 0;
    virtual IResponse *createResponse() = 0;
    virtual ISession *createSession(SessionId_t id) = 0;
};

#endif // IFACTORY_HPP
// Path: includes/IFactory.hpp
