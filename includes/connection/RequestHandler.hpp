#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

/*
 * The RequestHandler class serves as the orchestrator for handling client
 * requests within webserv.
 *
 * It coordinates the processing of client requests, managing the client
 * handler, request parser, router, and request handler objects.
 *
 * When a request is registered, RequestHandler initializes a ClientHandler with
 * the relevant socket descriptor, reads and processes the request, and sends a
 * response back to the client.
 *
 * In case the entire response cannot be sent at once, RequestHandler handles
 * partial sends by buffering remaining bytes associated with the client's
 * socket descriptor and updating the socket's event status to include POLLOUT
 * for subsequent transmission.
 *
 * In scenarios where exceptions occur, RequestHandler handles various types of
 * exceptions, logs relevant information, and sends appropriate HTTP responses
 * to the client.
 *
 * Throughout its operation, RequestHandler ensures smooth communication between
 * clients and the server while managing exceptions effectively.
 */

#include "../buffer/IBufferManager.hpp"
#include "../configuration/IConfiguration.hpp"
#include "../constants/HttpHelper.hpp"
#include "../exception/IExceptionHandler.hpp"
#include "../logger/ILogger.hpp"
#include "../request/RequestParser.hpp"
#include "../response/IResponseGenerator.hpp"
#include "../response/IRouter.hpp"
#include "IClientHandler.hpp"
#include "IConnectionManager.hpp"
#include "IRequestHandler.hpp"
#include <map>

// Forward declaration
class ISocket;

class RequestHandler : public IRequestHandler
{
private:
    // Private member variables
    IBufferManager &m_buffer_manager;         // the buffer manager
    IConnectionManager &m_connection_manager; // the connection manager
    IClientHandler &m_client_handler;     // Handles communication with clients
    const RequestParser m_request_parser; // Parses incoming requests
    IRouter &m_router; // Routes requests to appropriate handlers

    // AResponseGenerator *m_request_handler;                 // Pointer to the
    // recruited request handler
    HttpHelper m_http_helper; // Helper class for request-related operations
    ILogger &m_logger;        // Reference to the logger
    const IExceptionHandler
        &m_exception_handler;         // Ref to the exception handler
    std::map<int, int> m_pipe_routes; // pipe descriptors to socket descriptors

    // private method
    int m_sendResponse(int socket_descriptor);

public:
    // Constructor
    RequestHandler(IBufferManager &buffer_manager,
                   IConnectionManager &connection_manager,
                   const IConfiguration &configuration, IRouter &router,
                   ILogger &logger, const IExceptionHandler &exception_handler,
                   IClientHandler &client_handler);

    // Destructor
    ~RequestHandler();

    // Handles client requests
    Triplet_t handleRequest(int socket_descriptor);

    // Handles exceptions related to pipe events
    int handlePipeException(int pipe_descriptor);

    // Handles reading response from pipe
    int handlePipeRead(int pipe_descriptor);

    // Handles error responses
    void handleErrorResponse(int socket_descriptor, int status_code);
    void handleErrorResponse(int socket_descriptor, HttpStatusCode status_code);

    // Handle redirect
    void handleRedirectResponse(int socket_descriptor, std::string location);

    // Remove and close the connection
    void removeConnection(int socket_descriptor);

    // Execute CGI
    Triplet_t executeCgi(int body_descriptor);
};

#endif // CONNECTIONS_HPP
// Path: includes/connection/RequestHandler.hpp
