namespace http::routing {

inline routing_table::routing_table(std::vector< endpoint > endpoints)
  : endpoints_{ std::move(endpoints) }
{}

inline std::vector< endpoint > const&
routing_table::endpoints() const
{
  return endpoints_;
}

inline protocol::optional< routing_table::find_result >
routing_table::find_route(std::string const& path) const
{
  for (auto const& e : endpoints()) {
    route_parameters params;

    if (match_path(e.path(), path, params))
      return find_result{ e.handler(), std::move(params) };
  }

  return {};
}

inline route&
routing_table::builder::add_route()
{
  roots_.emplace_back();
  return roots_.back();
}

inline route&
routing_table::builder::add_route(route r)
{
  roots_.emplace_back(std::move(r));
  return roots_.back();
}

inline route&
routing_table::builder::add_route(std::string path)
{
  return add_route(route{ std::move(path) });
}

inline route&
routing_table::builder::add_route(std::string path,
                                  server::request_handler handler)
{
  return add_route({ std::move(path), std::move(handler) });
}

inline routing_table
routing_table::builder::build() const
{
  std::vector< endpoint > endpoints;

  for (auto const& r : roots_)
    flatten_route(endpoints, r);

  return routing_table{ std::move(endpoints) };
}

} // namespace http::routing
