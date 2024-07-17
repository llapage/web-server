#include "../../includes/response/RFCCgiResponseGenerator.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include "../../includes/utils/Converter.hpp"
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <string>

#define NO_THROW 0x1 // Do not throw an exception
#define KEEP_CGI_OUTPUT_PIPE_READ_END                                          \
    0x2 // Do no close the read end of the CGI Output pipe
#define KEEP_CGI_INPUT_PIPE_WRITE_END                                          \
    0x4             // Do not close the write end of the CGI Input pipe
#define READ_END 0  // Read end of a pipe
#define WRITE_END 1 // Write end of a pipe

RFCCgiResponseGenerator::RFCCgiResponseGenerator(ILogger &logger,
                                                 const std::string &bin_path)
    : m_logger(logger), m_http_status_code_helper(HttpStatusCodeHelper()),
      m_bin_path(bin_path), m_from_file(false)
{
}

RFCCgiResponseGenerator::RFCCgiResponseGenerator(ILogger &logger,
                                                 const std::string &bin_path,
                                                 bool from_file)
    : m_logger(logger), m_http_status_code_helper(HttpStatusCodeHelper()),
      m_bin_path(bin_path), m_from_file(from_file)
{
}

RFCCgiResponseGenerator::~RFCCgiResponseGenerator() {}

// calls execve to execute the CGI script
// returns the cgi process Info (pid, read end of the CGI Output pipe, write end
// of the CGI Input pipe) Throws an exception if an error occurs
Triplet_t RFCCgiResponseGenerator::generateResponse(
    const IRoute &route, const IRequest &request, IResponse &response,
    IConfiguration &configuration)
{
    // void the unused parameters
    (void)response;
    (void)configuration;

    // Get the CGI script path
    // const std::string cgi_script = route.getCgiScript();

    // Set the Script path
    std::string uri = request.getUri();
    size_t last_slash = uri.find_last_of('/');
    size_t question_mark = uri.find('?');
    std::string script =
        uri.substr(last_slash + 1, question_mark - last_slash - 1);

    m_logger.log(DEBUG, "CGI SCRIPT " + script);
    (void)m_from_file;

    // Set cgi arguments
    std::vector<char *> cgi_args;
    m_setCgiArguments(m_bin_path, script, route, cgi_args);

    // Set cgi environment variables
    std::vector<char *> cgi_env;
    m_setCgiEnvironment(script, route, request, cgi_env);

    // Create a pipe to send the response from the CGI script to the server
    int cgi_output_pipe_fd[ 2 ];
    if (pipe(cgi_output_pipe_fd) == -1)
        // pipe failed
        m_cleanUp(cgi_args.data(),
                  cgi_env.data()); // Free memory and throw exception 500

    // Fork a child process
    pid_t pid = fork();
    if (pid == -1)
        // fork failed
        m_cleanUp(cgi_args.data(), cgi_env.data(),
                  cgi_output_pipe_fd); // Free memory and throw exception 500

    else if (pid == 0) // child process
    {
        m_logger.log(DEBUG,
                     "Forked a child process to execute the CGI script PID: " +
                         Converter::toString(getpid()) +
                         " Parent PID: " + Converter::toString(getppid()));

        // open the body file for reading
        std::string body_file_path = request.getBodyFilePath();
        int file_fd = open(body_file_path.c_str(), O_RDONLY);
        if (file_fd == -1)
        {
            m_cleanUp(cgi_args.data(), cgi_env.data(), cgi_output_pipe_fd,
                      NO_THROW);
            exit(1);
        }

        // set CLOSE_ON_EXEC flag
        fcntl(file_fd, F_SETFD, FD_CLOEXEC);

        // stdin should read from the request body file
        dup2(file_fd,
             STDIN_FILENO); // redirect stdin to file descriptor

        // stdout should write to CGI Output pipe
        close(cgi_output_pipe_fd[ READ_END ]); // close read end
        dup2(cgi_output_pipe_fd[ WRITE_END ],
             STDOUT_FILENO);                    // redirect stdout to pipe
        close(cgi_output_pipe_fd[ WRITE_END ]); // close write end

        // Call execve
        execve(cgi_args[ 0 ], cgi_args.data(), cgi_env.data());
        m_logger.log(ERROR, "Execve failed: " + std::string(strerror(errno)));
        // If execve returns, an error occurred; free memory and exit
        m_cleanUp(cgi_args.data(), cgi_env.data(), cgi_output_pipe_fd,
                  NO_THROW);
        close(file_fd);
        exit(EXIT_FAILURE);
    }

    else // parent process
    {
        // Log the new CGI process ID
        m_logger.log(DEBUG, "New CGI process ID: " + Converter::toString(pid));

        // Set the read end of the Cgi Output Pipe to non-blocking
        fcntl(cgi_output_pipe_fd[ 0 ], F_SETFL, O_NONBLOCK);

        // Free memory and keep the write end of the CGI Input pipe open and the
        // read end of the CGI Output pipe open
        m_cleanUp(cgi_args.data(), cgi_env.data(), cgi_output_pipe_fd,
                  NO_THROW | KEEP_CGI_OUTPUT_PIPE_READ_END);

        // Log the Cgi info
        m_logger.log(VERBOSE, "Returning CGI info tuple; PID: " +
                                  Converter::toString(pid) +
                                  " CGI output pipe Read end: " +
                                  Converter::toString(cgi_output_pipe_fd[ 0 ]));

        // Return the read end of the pipe to read the response later without
        // blocking
        return std::make_pair(
            pid, std::make_pair(cgi_output_pipe_fd[ READ_END ],
                                -1)); // change to a simple pair later
    }

    return std::make_pair(-1, std::make_pair(-1, -1)); // unreachable code
}

void RFCCgiResponseGenerator::m_setCgiArguments(const std::string &cgi_script,
                                                const std::string &script,
                                                const IRoute &route,
                                                std::vector<char *> &cgi_args)
{
    cgi_args.push_back(strdup(cgi_script.c_str())); // Interpreter absolute path
    cgi_args.push_back(
        m_getScriptPath(script, route)); // script path: location block root
                                         // path + URI(excl. query string)
    cgi_args.push_back(NULL);

    // Check for strdup failures
    for (size_t i = 0; i < cgi_args.size() - 1; ++i)
    {
        if (cgi_args[ i ] == NULL)
        {
            m_cleanUp(cgi_args.data()); // Free memory and throw exception 500
        }
    }

    m_logger.log(DEBUG, "CGI interpreter: " + std::string(cgi_args[ 0 ]));
    m_logger.log(DEBUG, "CGI script: " + std::string(cgi_args[ 1 ]));
}

void RFCCgiResponseGenerator::m_setCgiEnvironment(const std::string &script,
                                                  const IRoute &route,
                                                  const IRequest &request,
                                                  std::vector<char *> &cgi_env)
{
    cgi_env.push_back(
        strdup(("REQUEST_METHOD=" + request.getMethodString()).c_str()));
    cgi_env.push_back(
        strdup(("QUERY_STRING=" + request.getQueryString()).c_str()));
    cgi_env.push_back(
        strdup(("CONTENT_LENGTH=" + request.getContentLength()).c_str()));
    cgi_env.push_back(
        strdup(("CONTENT_TYPE=" + request.getContentType()).c_str()));
    std::string script_filename = route.getRoot() + route.getPath();
    if (script_filename[ script_filename.size() - 1 ] != '/')
        script_filename += "/";
    script_filename += script;
    cgi_env.push_back(strdup(("SCRIPT_FILENAME=" + script_filename).c_str()));
    cgi_env.push_back(strdup(("SCRIPT_NAME=" + script).c_str()));
    // path info to satisfy 42 tester
    std::string path_info = request.getUri();
    cgi_env.push_back(strdup(("PATH_INFO=" + path_info).c_str()));
    cgi_env.push_back(strdup(
        ("PATH_TRANSLATED=" + m_getPathTranslated(path_info, route)).c_str()));
    cgi_env.push_back(strdup(("REQUEST_URI=" + request.getUri()).c_str()));
    cgi_env.push_back(
        strdup(("SERVER_PROTOCOL=" + request.getHttpVersionString()).c_str()));
    std::map<HttpHeader, std::string> headers = request.getHeaders();
    cgi_env.push_back(strdup(
        ("HTTP_X_SECRET_HEADER_FOR_TEST=" + headers[ X_SECRET_HEADER_FOR_TEST ])
            .c_str()));
    cgi_env.push_back(NULL);

    // Check for strdup failures
    for (size_t i = 0; i < cgi_env.size() - 1; ++i)
        if (cgi_env[ i ] == NULL)
            m_cleanUp(NULL,
                      cgi_env.data()); // Free memory and throw exception 500

    // Log the environment variables
    for (size_t i = 0; i < cgi_env.size() - 1; ++i)
        m_logger.log(VERBOSE, "CGI Environment: " + std::string(cgi_env[ i ]));
}

char *RFCCgiResponseGenerator::m_getScriptPath(const std::string &script_name,
                                               const IRoute &route) const
{
    // Get the root path
    std::string root_path = route.getRoot();

    // Get the prefix
    std::string prefix = route.getPath();

    // Append a slash if the prefix does not end with a slash
    if (prefix[ prefix.size() - 1 ] != '/')
        prefix += '/';

    // Return the script path without the query string
    return strdup((root_path + prefix + script_name)
                      .c_str()); //  e.g. /path/to/script.php
}

std::string
RFCCgiResponseGenerator::m_getPathTranslated(std::string &path_info,
                                             const IRoute &route) const
{
    // PATH_TRANSLATED = location root + location prefix + PATH_INFO

    //  Get the location root path
    std::string root_path = route.getRoot();

    // Get the location prefix
    std::string prefix = route.getPath();

    // Return the path translated
    return root_path + prefix + path_info;
}

void RFCCgiResponseGenerator::m_cleanUp(char *cgi_args[], char *cgi_env[],
                                        int cgi_output_pipe_fd[ 2 ],
                                        short option) const
{
    // Free args
    if (cgi_args != NULL)
    {
        for (int i = 0; cgi_args[ i ] != NULL; ++i)
        {
            free(cgi_args[ i ]);
        }
    }

    // Free env
    if (cgi_env != NULL)
    {
        for (int i = 0; cgi_env[ i ] != NULL; ++i)
        {
            free(cgi_env[ i ]);
        }
    }

    // Close CGI Output pipe
    if (cgi_output_pipe_fd != NULL)
    {
        if ((option & KEEP_CGI_OUTPUT_PIPE_READ_END) == 0)
            close(cgi_output_pipe_fd[ 0 ]);
        close(cgi_output_pipe_fd[ 1 ]);
    }

    // Throw an exception
    if ((option & NO_THROW) == 0)
        throw HttpStatusCodeException(INTERNAL_SERVER_ERROR); // 500
}

// Path: srcs/CgiResponseGenerator.cpp
