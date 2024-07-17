#ifndef FACTORY_HPP
#define FACTORY_HPP

#include "../configuration/IConfiguration.hpp"
#include "../constants/HttpHelper.hpp"
#include "../logger/ILogger.hpp"
#include "IFactory.hpp"

class Factory : public IFactory
{
private:
    const IConfiguration &m_configuration;
    ILogger &m_logger;
    const HttpHelper m_http_helper;

public:
    Factory(const IConfiguration &configuration, ILogger &m_logger);
    virtual ~Factory();

    virtual IConnection *
        createConnection(std::pair<int, std::pair<std::string, std::string> >);
    virtual IRequest *createRequest();
    virtual IResponse *createResponse();
    virtual ISession *createSession(SessionId_t id);
};

#endif // FACTORY_HPP
// Path: includes/Factory.hpp
