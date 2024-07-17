#include "includes/buffer/BufferManager.hpp"
#include "includes/configuration/ConfigurationLoader.hpp"
#include "includes/connection/ClientHandler.hpp"
#include "includes/connection/ConnectionManager.hpp"
#include "includes/connection/RequestHandler.hpp"
#include "includes/core/EventManager.hpp"
#include "includes/core/PollingService.hpp"
#include "includes/exception/ExceptionHandler.hpp"
#include "includes/factory/Factory.hpp"
#include "includes/logger/Logger.hpp"
#include "includes/logger/LoggerConfiguration.hpp"
#include "includes/network/Server.hpp"
#include "includes/network/Socket.hpp"
#include "includes/pollfd/PollfdManager.hpp"
#include "includes/response/Router.hpp"
#include "includes/utils/SignalHandler.hpp"

/*
 * webserv Workflow:
 *
 * Upon launch, the configuration class reads and parses the configuration file,
 * providing settings for other components.
 * The Server class creates the required server sockets and binds them to the
 * specified ports. Once initialisation is complete, the server enters the main
 * loop. The PollingService class polls the server sockets for events. When an
 * event is detected or the timeout is reached, the EventManager class handles
 * the event. Requests are managed by the RequestHandler class, which utilizes
 * the ClientHandler to communicate with the client, the RequestParser to parse
 * the request, the Router to select the appropriate route and to generate a
 * Response. The generated Response is then pushed to the BufferManager, who
 * will send it to the client in the next cycle if polling indicates the socket
 * is ready. All the while, the Logger class registers errors and access log
 * entries with the BufferManager, who writes them non-blockingly to the log
 * file. This process continues in a loop.
 */

int main(int argc, char **argv)
{
    // Instantiate the SignalHandler.
    SignalHandler signalHandler;

    // Catch SIGINT signal.
    signalHandler.sigint();

    // Get the configuration file path.
    std::string config_path;
    if (argc == 1)
        config_path = "config/default.conf";
    else
        config_path = argv[ 1 ];

    // Instantiate the Socket instance.
    Socket socket;

    // Instantiate the buffer_manager.
    BufferManager buffer_manager(socket);

    // Instantiate the logger.
    Logger logger(buffer_manager);

    // Instantiate the Client Handler.
    ClientHandler client_handler(socket, logger);

    // Instantiate the exception_handler.
    ExceptionHandler exception_handler(logger);

    ConfigurationLoader conf_loader(logger);

    try
    {
        // load configuration from file and create the configuration object.
        IConfiguration &configuration =
            conf_loader.loadConfiguration(config_path);

        // parse the configuration file

        // Instantiate the PollfdManager.
        PollfdManager pollfd_manager(configuration, logger);

        // Configure the logger
        LoggerConfiguration *logger_configuration = new LoggerConfiguration(
            buffer_manager, configuration, pollfd_manager);
        logger.configure(*logger_configuration);

        // Instantiate the Factory.
        Factory factory(configuration, logger);

        // Instantiate the ConnectionManager.
        ConnectionManager connection_manager(logger, factory);

        // Instantiate the Server.
        Server server(socket, pollfd_manager, connection_manager, configuration,
                      logger);

        // Instantiate the Router.
        // Router router(configuration, logger, HttpHelper());
        Router router(configuration, logger);

        // Instantiate the RequestHandler.
        RequestHandler request_handler(buffer_manager, connection_manager,
                                       configuration, router, logger,
                                       exception_handler, client_handler);

        // Instantiate the PollingService.
        PollingService polling_service(pollfd_manager, logger);

        // Instantiate the EventManager.
        EventManager event_manager(pollfd_manager, buffer_manager,
                                   connection_manager, server, request_handler,
                                   logger);

        // Start the webserv core cycle.
        while (true)
        {
            try
            {
                // Poll events.
                polling_service.pollEvents();

                // Handle events.
                event_manager.handleEvents();

                // Collect garbage.
                connection_manager.collectGarbage();

                // Check for signals.
                signalHandler.checkState();
            }
            catch (WebservException &e)
            {
                // Handle exceptions.
                exception_handler.handleException(e, "webserv core cycle: ");
                break;
            }
        }
    }
    catch (WebservException &e)
    {
        // Configure the logger with a null configuration.
        LoggerConfiguration *null = NULL;
        logger.configure(*null);

        // Handle setup exceptions.
        exception_handler.handleException(e, "webserv setup: ");
    }

    // Configure the logger with a null configuration.
    // LoggerConfiguration *null = NULL;
    // logger.configure(*null);
}

// Path: main.cpp
