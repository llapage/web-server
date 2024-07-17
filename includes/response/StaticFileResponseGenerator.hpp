#ifndef STATICFILERESPONSEGENERATOR_HPP
#define STATICFILERESPONSEGENERATOR_HPP

#include "../logger/ILogger.hpp"
#include "IResponseGenerator.hpp"

class StaticFileResponseGenerator : public IResponseGenerator
{
private:
    const std::map<std::string, std::string> m_mime_types;
    ILogger &m_logger;

    std::map<std::string, std::string> m_initialiseMimeTypes() const;
    std::string m_getMimeType(const std::string &file_path) const;
    bool m_isDirectory(const std::string &path) const;
    int m_serveFile(const std::string &file_path, IResponse &response);
    void m_serveDirectoryListing(const std::string &directory_path,
                                 IResponse &response);

public:
    StaticFileResponseGenerator(ILogger &logger);
    ~StaticFileResponseGenerator();
    Triplet_t generateResponse(const IRoute &route, const IRequest &request,
                               IResponse &response,
                               IConfiguration &configuration);
};

#endif // STATICFILERESPONSEGENERATOR_HPP
// Path: includes/response/StaticFileResponseGenerator.hpp
