namespace http::routing {

inline route::route()
{}

inline route::route(std::string path) : path_{ std::move(path) }
{
  ensure_valid_path(path_);
}

inline route::route(std::string path, server::request_handler handler)
  : path_{ std::move(path) }, handler_{ std::move(handler) }
{
  ensure_valid_path(path_);
}

inline std::string const&
route::path() const
{
  return path_;
}

inline std::vector< server::middleware > const&
route::middleware() const
{
  return middleware_;
}

inline protocol::optional< server::request_handler > const&
route::handler() const
{
  return handler_;
}

inline std::list< route > const&
route::children() const
{
  return children_;
}

inline void
route::use(server::middleware middleware)
{
  middleware_.emplace_back(std::move(middleware));
}

inline route&
route::add_route()
{
  children_.emplace_back();
  return children_.back();
}

inline route&
route::add_route(route r)
{
  children_.emplace_back(std::move(r));
  return children_.back();
}

inline route&
route::add_route(std::string path)
{
  return add_route(route{ std::move(path) });
}

inline route&
route::add_route(std::string path, server::request_handler handler)
{
  return add_route(route{ std::move(path), std::move(handler) });
}

inline std::ostream&
to_stream(std::ostream& o, route const& r, std::size_t indent)
{
  o << std::string(indent, ' ');
  o << " -> '" << (r.path().empty() ? "<empty>" : r.path()) << '\'';

  if (!r.middleware().empty())
    o << " (with middleware)";

  if (!r.handler())
    o << " (null handler)";

  o << '\n';

  for (auto const& child : r.children())
    to_stream(o, child, indent + 2);

  return o;
}

} // namespace http::routing
