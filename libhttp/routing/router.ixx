namespace http {
namespace routing {

  inline router::router(routing::routing_table routing_table)
    : rt_{ std::move(routing_table) }
  {}

  inline routing::routing_table const&
  router::routing_table() const
  {
    return rt_;
  }

  inline void
  router::on_request(server::transaction& tx) const
  {
    auto path = tx.request().target_uri().path_str();

    std::cout << "Locating endpoint for " << path << '\n';

    auto result = routing_table().find_route(path);

    if (!result) {
      // TODO send 404
      std::cout << "Could not find route\n";
      tx.send_error(server::common_error::not_found);
      return;
    }

    tx.extensions().extend(&tx.memory().alloc(result->params));

    result->handler.invoke(tx);
  }

} // namespace routing
} // namespace http
