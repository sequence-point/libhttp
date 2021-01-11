#ifndef libhttp__routing__router_hxx_
#define libhttp__routing__router_hxx_

#include <libhttp/server/transaction.hxx>

#include <libhttp/routing/route.hxx>
#include <libhttp/routing/routing-table.hxx>

namespace http {
namespace routing {

  class router {
  public:
    explicit router(routing::routing_table routing_table);

    routing::routing_table const&
    routing_table() const;

    void
    on_request(server::transaction& tx) const;

    void
    operator()(server::transaction& tx) const
    {
      on_request(tx);
    }

    // TODO virtual void route_not_found(tx)

  private:
    routing::routing_table rt_;
  };

} // namespace routing
} // namespace http

#include <libhttp/routing/router.ixx>

#endif
