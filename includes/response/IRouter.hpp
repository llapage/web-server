#ifndef IRouter_HPP
#define IRouter_HPP

/*
 * IRouter.hpp
 * Abstract class for the Router
 */

#include "../request/IRequest.hpp"
#include "../response/IResponse.hpp"

typedef std::pair<int, std::pair<int, int> > Triplet_t;

class IRouter
{
public:
    virtual ~IRouter() {};

    virtual Triplet_t execRoute(IRoute *route, IRequest *req,
                                IResponse *res) = 0;
    virtual IRoute *getRoute(IRequest *req, IResponse *res) = 0;
};

#endif // IROUTER_HPP
// Path: includes/IRouter.hpp
