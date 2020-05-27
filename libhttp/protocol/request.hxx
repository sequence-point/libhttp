#ifndef libhttp__protocol__request_hxx_
#define libhttp__protocol__request_hxx_

#include <libhttp/protocol/message.hxx>

#include <liburi/uri.hxx>

#include <asio.hpp>

#include <string>
#include <vector>

namespace http {
namespace protocol {

  class request : public message {
  public:
    request() = default;

    request(std::string method, std::string target, protocol::version version);

    std::string const&
    method() const;

    void
    set_method(std::string method);

    std::string const&
    target() const;

    void
    set_target(std::string target);

    uri::uri const&
    target_uri() const;

    void
    set_target_uri(uri::uri target_uri);

  private:
    std::string method_;
    std::string target_;
    uri::uri target_uri_;
    std::string query_;
  };

  void
  to_buffers(std::vector< asio::const_buffer >& buffers, request const& r);

  std::vector< asio::const_buffer >
  to_buffers(request const& r);

} // namespace protocol
} // namespace http

#include <libhttp/protocol/request.ixx>

#endif
