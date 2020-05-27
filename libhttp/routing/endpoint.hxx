#ifndef libhttp__routing__endpoint_hxx_
#define libhttp__routing__endpoint_hxx_

#include <libhttp/server/request-handler.hxx>

#include <iostream>

namespace http {
namespace routing {

  class endpoint {
  public:
    endpoint(std::string path, server::request_handler handler);

    std::string const&
    path() const;

    server::request_handler const&
    handler() const;

  private:
    std::string path_;
    server::request_handler handler_;
  };

  std::ostream&
  to_stream(std::ostream& o, endpoint const& ep);

  std::ostream&
  operator<<(std::ostream& o, endpoint const& ep);

} // namespace routing
} // namespace http

#include <libhttp/routing/endpoint.ixx>

#endif
