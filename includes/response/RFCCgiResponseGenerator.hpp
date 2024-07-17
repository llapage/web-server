#ifndef RFCCGIRESPONSEGENERATOR_HPP
#define RFCCGIRESPONSEGENERATOR_HPP

#include "../configuration/IConfiguration.hpp"
#include "../constants/HttpStatusCodeHelper.hpp"
#include "../logger/ILogger.hpp"
#include "../request/IRequest.hpp"
#include "IResponseGenerator.hpp"
#include "IRoute.hpp"
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

class RFCCgiResponseGenerator : public IResponseGenerator
{
private:
    ILogger &m_logger;
    HttpStatusCodeHelper m_http_status_code_helper;
    const std::string &m_bin_path;
    bool m_from_file;

    void m_setCgiArguments(const std::string &cgi_script,
                           const std::string &script, const IRoute &route,
                           std::vector<char *> &cgi_args);
    void m_setCgiEnvironment(const std::string &cgi_script, const IRoute &route,
                             const IRequest &request,
                             std::vector<char *> &cgi_env);
    char *m_getScriptPath(const std::string &script_name,
                          const IRoute &route) const;
    std::string m_getPathTranslated(std::string &script_name,
                                    const IRoute &route) const;
    void m_cleanUp(char *cgi_args[], char *cgi_env[] = NULL,
                   int response_pipe_fd[ 2 ] = NULL, short option = 0x0) const;

public:
    RFCCgiResponseGenerator(ILogger &logger, const std::string &bin_path);
    RFCCgiResponseGenerator(ILogger &logger, const std::string &bin_path,
                            bool from_file);
    ~RFCCgiResponseGenerator();

    virtual Triplet_t generateResponse(const IRoute &route,
                                       const IRequest &request,
                                       IResponse &response,
                                       IConfiguration &configuration);
};

#endif // RFCCGIRESPONSEGENERATOR_HPP
// Path: includes/CgiResponseGenerator.hpp
