#ifndef ISERVER_HPP
#define ISERVER_HPP

class IServer
{
public:
    virtual ~IServer() {};

    virtual void acceptConnection(int) = 0;
    virtual void terminate(int) = 0;
};

#endif // ISERVER_HPP
       // Path: includes/ISocket.hpp
