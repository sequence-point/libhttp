#ifndef libhttp__routing__routing_table_hxx_
#define libhttp__routing__routing_table_hxx_

#include <libhttp/protocol/optional.hxx>

#include <libhttp/server/request-handler.hxx>

#include <libhttp/routing/endpoint.hxx>
#include <libhttp/routing/flatten-route.hxx>
#include <libhttp/routing/match-path.hxx>
#include <libhttp/routing/route-parameters.hxx>
#include <libhttp/routing/route.hxx>

#include <list>
#include <vector>

namespace http::routing {

class routing_table {
public:
  class builder;

  struct find_result {
    server::request_handler const& handler;
    route_parameters params;
  };

  explicit routing_table(std::vector< endpoint > endpoint);

  std::vector< endpoint > const&
  endpoints() const;

  protocol::optional< find_result >
  find_route(std::string const& path) const;

private:
  std::vector< endpoint > endpoints_;
};

class routing_table::builder {
public:
  route&
  add_route();

  route&
  add_route(route r);

  route&
  add_route(std::string path);

  route&
  add_route(std::string path, server::request_handler handler);

  routing_table
  build() const;

private:
  // Must be std::list to prevent invalidation of
  // references to individual routes.
  std::list< route > roots_;
};

} // namespace http::routing

#include <libhttp/routing/routing-table.ixx>

#endif
