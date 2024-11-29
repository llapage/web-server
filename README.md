This is a HTTP server in c++98 on MacOs from scratch.

The goal of the project is to build a C++98 compatible HTTP web server from scratch. The web server can handle HTTP GET, HEAD, POST, PUT, and DELETE, OPTIONS, PATCH, CONNECT, TRACE Requests, and can serve static files from a specified root directory or dynamic content using CGI. 
It is also able to handle multiple client connections concurrently with the help of poll().

executable runs as follow:

./webserv [configuration file]
