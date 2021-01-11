namespace http::routing {

inline void
flatten_route(std::vector< endpoint >& endpoints,
              std::vector< server::middleware > middlewares,
              route const& r,
              std::string const& root)
{
  //      auto merge = [](string const& l, string const& r) {
  //        return r.empty() ? l : (l + '/' + r);
  //      };
  auto const path = r.path().empty() ? root : (root + '/' + r.path());

  // Append any middlewares if we have any.
  for (auto const& m : r.middleware())
    middlewares.emplace_back(m);

  // Generate an endpoint for this route if we have a handler.
  if (auto const& h = r.handler()) {
    endpoints.emplace_back(path,
                           server::make_middleware_chain(middlewares, *h));
  }

  // Flatten any child routes.
  for (auto const& child_route : r.children())
    flatten_route(endpoints, middlewares, child_route, path);
}

inline void
flatten_route(std::vector< endpoint >& endpoints, route const& r)
{
  flatten_route(endpoints, {}, r, "");
}

inline std::vector< endpoint >
flatten_route(route const& r)
{
  std::vector< endpoint > endpoints;
  flatten_route(endpoints, r);
  return endpoints;
}

} // namespace http::routing
