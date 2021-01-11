#ifndef libhttp__routing__route_hxx_
#define libhttp__routing__route_hxx_

#include <libhttp/server/middleware.hxx>
#include <libhttp/server/request-handler.hxx>

#include <libhttp/protocol/optional.hxx>
#include <libhttp/routing/endpoint.hxx>

#include <iostream>
#include <list>
#include <stdexcept>
#include <vector>

namespace http::routing {

class route {
public:
  //! Construct a new route.
  route();

  //! Construct a new route.
  explicit route(std::string path);

  //! Construct a new route.
  route(std::string path, server::request_handler handler);

  //! Get the path segment of this route.
  std::string const&
  path() const;

  //! Get any middleware attached to this route.
  std::vector< server::middleware > const&
  middleware() const;

  protocol::optional< server::request_handler > const&
  handler() const;

  std::list< route > const&
  children() const;

  void
  use(server::middleware middleware);

  route&
  add_route();

  route&
  add_route(route r);

  route&
  add_route(std::string path);

  route&
  add_route(std::string path, server::request_handler handler);

private:
  std::string path_;
  std::vector< server::middleware > middleware_;
  protocol::optional< server::request_handler > handler_;

  inline void
  ensure_valid_path(std::string const& path)
  {
    if (!path.empty()) {
      if (path.front() == '/')
        throw std::invalid_argument{ "route path must not start with '/'" };

      if (path.back() == '/')
        throw std::invalid_argument{ "route path must not end with '/'" };
    }
  }

  // Must be std::list to prevent invalidation of references to
  // individual routes.
  std::list< route > children_;
};

std::ostream&
to_stream(std::ostream& o, route const& r, size_t indent);

inline std::ostream&
operator<<(std::ostream& o, route const& r)
{
  return to_stream(o, r, 0);
}

} // namespace http::routing

#include <libhttp/routing/route.ixx>

#endif
