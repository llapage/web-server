#ifndef DELETERESPONSEGENERATOR_HPP
#define DELETERESPONSEGENERATOR_HPP

#include "../logger/ILogger.hpp"
#include "IResponseGenerator.hpp"

class DeleteResponseGenerator : public IResponseGenerator
{
private:
    ILogger &m_logger;

public:
    DeleteResponseGenerator(ILogger &logger);
    ~DeleteResponseGenerator();
    Triplet_t generateResponse(const IRoute &route, const IRequest &request,
                               IResponse &response,
                               IConfiguration &configuration);
};

#endif // DELETE_RESPONSE_GENERATOR_HPP
// Path: includes/response/DeleteResponseGenerator.hpp
