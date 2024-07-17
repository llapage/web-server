#include "../../includes/network/Server.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include "../../includes/utils/Converter.hpp"
#include <cstdlib>
#include <set>

/*
 * The Server class is responsible for managing core operations of webserv,
 * including initialization, connection handling, and termination.
 *
 * It oversees socket creation, binding, and listening for incoming connections,
 * all configured for non-blocking operation. Leveraging the poll() polling
 * mechanism, Server monitors events on the server socket, log file descriptors,
 * and client connections, facilitating the acceptance of incoming connections
 * and their inclusion in the monitoring queue.
 *
 */

/* Constructor - Initializes the Server object and sets up the server socket and
 * polling file descriptors.*/
Server::Server(const ISocket &socket, IPollfdManager &pollfdManager,
               IConnectionManager &connectionManager,
               IConfiguration &configuration, ILogger &logger)
    : m_socket(socket), m_pollfd_manager(pollfdManager),
      m_connection_manager(connectionManager), m_logger(logger)
{
    // Log server initialization
    m_logger.log(VERBOSE, "Initializing Server...");

    // Get the maximum connections value
    int max_connections =
        configuration.getBlocks("events")[ 0 ]->getInt("worker_connections");

    // Create a set to store unique IP:port combinations
    std::set<std::pair<int, int> > processed_endpoints;

    // Get the list of virtual servers
    std::vector<IConfiguration *> servers =
        configuration.getBlocks("http")[ 0 ]->getBlocks("server");

    // For each virtual server
    for (std::vector<IConfiguration *>::iterator server_iterator =
             servers.begin();
         server_iterator != servers.end(); server_iterator++)
    {
        // Get the list of listen directives
        std::vector<std::string> listen_vector =
            (*server_iterator)->getStringVector("listen");

        // For each listen directive
        for (std::vector<std::string>::iterator listen_iterator =
                 listen_vector.begin();
             listen_iterator != listen_vector.end(); listen_iterator++)
        {
            int ip = 0; // Default IP to 0 (all network interfaces)
            int port;

            // Find the position of the colon (if present, ip was specified)
            size_t colon_pos = listen_iterator->find(':');
            if (colon_pos != std::string::npos)
            {
                ip = Converter::toInt(listen_iterator->substr(0, colon_pos));
                port = Converter::toInt(listen_iterator->substr(colon_pos + 1));
            }
            else // ip was not specified
            {
                port = Converter::toInt(*listen_iterator);
            }

            // Check if the current IP:port combination has already been
            // processed
            if (processed_endpoints.find(std::make_pair(ip, port)) ==
                processed_endpoints.end())
            {
                // Add the current IP:port combination to the set of processed
                // endpoints
                processed_endpoints.insert(std::make_pair(ip, port));

                // Initialize a new socket
                m_initializeServerSocket(ip, port, max_connections);
            }
        }
    }
    m_logger.log(VERBOSE, "... finished Server initialization");
}

/* Destructor to close file descriptors*/
Server::~Server()
{
    // Close all socket file descriptors
    m_pollfd_manager.closeAllFileDescriptors();
}

/* Initialize server socket*/
void Server::m_initializeServerSocket(int ip, int port, int max_connections)
{
    // Create server socket
    int server_socket_descriptor = m_socket.socket();
    if (server_socket_descriptor < 0)
        throw SocketCreateError();

    // Set server socket option to reuse address
    if (m_socket.setReuseAddr(server_socket_descriptor) < 0)
        throw SocketSetError();

    // Bind server socket to port
    if (m_socket.bind(server_socket_descriptor, ip, port) < 0)
        throw SocketBindError(server_socket_descriptor, ip, port);

    // Listen for incoming connections
    if (m_socket.listen(server_socket_descriptor, max_connections) < 0)
        throw SocketListenError();

    // Set server socket to non-blocking mode
    if (m_socket.setNonBlocking(server_socket_descriptor) < 0)
        throw SocketSetError();

    // Add server socket to polling list
    pollfd pollfd;
    pollfd.fd = server_socket_descriptor;
    pollfd.events = POLLIN | POLLERR | POLLHUP | POLLNVAL;
    pollfd.revents = 0;
    m_pollfd_manager.addServerSocketPollfd(pollfd);

    // Log server socket initialization
    m_logger.log(INFO, "Server socket initialized. Listening on " +
                           (ip ? Converter::toString(ip) : "ALL") + ":" +
                           Converter::toString(port));
}

/* Terminate server*/
void Server::terminate(int exitCode) { exit(exitCode); }

/* Accept a new client connection*/
void Server::acceptConnection(int server_socket_descriptor)
{
    // Ensure maximum connections limit has not been reached
    if (m_pollfd_manager.hasReachedCapacity())
        throw MaximumConnectionsReachedError();

    // Accept incoming connection
    std::pair<int, std::pair<std::string, std::string> > client_info =
        m_socket.accept(server_socket_descriptor);
    int client_socket_descriptor = client_info.first;
    std::string client_ip = client_info.second.first;
    std::string client_port = client_info.second.second;

    // Create a connection for the client
    m_connection_manager.addConnection(client_info);

    // Add client socket to polling list
    if (client_socket_descriptor < 0)
        throw ConnectionEstablishingError();
    pollfd pollfd;
    pollfd.fd = client_socket_descriptor;
    pollfd.events = POLLIN | POLLERR | POLLHUP | POLLNVAL;
    pollfd.revents = 0;
    m_pollfd_manager.addClientSocketPollfd(pollfd);

    // Set socket to non-blocking mode
    if (m_socket.setNonBlocking(client_socket_descriptor) < 0)
        throw SocketSetError();

    // Log accepted connection
    m_logger.log(VERBOSE, "Accepted new connection from " + client_ip + ":" +
                              client_port + ".");
}

// Path: /srcs/network/Server.cpp
