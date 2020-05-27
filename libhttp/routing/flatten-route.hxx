#ifndef libhttp__routing__flatten_route_hxx_
#define libhttp__routing__flatten_route_hxx_

#include <libhttp/server/middleware.hxx>

#include <libhttp/routing/endpoint.hxx>
#include <libhttp/routing/route.hxx>

#include <vector>

namespace http::routing {

void
flatten_route(std::vector< endpoint >& endpoints,
              route const& r,
              std::string const& root);

void
flatten_route(std::vector< endpoint >& endpoints, route const& r);

std::vector< endpoint >
flatten_route(route const& r);

} // namespace http::routing

#include <libhttp/routing/flatten-route.ixx>

#endif
