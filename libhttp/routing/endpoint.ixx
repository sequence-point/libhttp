namespace http::routing {

inline endpoint::endpoint(std::string path, server::request_handler handler)
  : path_{ std::move(path) }, handler_{ std::move(handler) }
{}

inline std::string const&
endpoint::path() const
{
  return path_;
}

inline server::request_handler const&
endpoint::handler() const
{
  return handler_;
}

inline std::ostream&
to_stream(std::ostream& os, endpoint const& ep)
{
  return os << ep.path();
}

inline std::ostream&
operator<<(std::ostream& os, endpoint const& ep)
{
  return to_stream(os, ep);
}

} // namespace http::routing
