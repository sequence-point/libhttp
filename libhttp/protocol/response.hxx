#ifndef libhttp__protocol__response_hxx_
#define libhttp__protocol__response_hxx_

#include <libhttp/protocol/message.hxx>
#include <libhttp/protocol/rfc7231-reason.hxx>
#include <libhttp/protocol/status-code.hxx>

#include <asio.hpp>

#include <string>
#include <vector>

namespace http {
namespace protocol {

  class response : public message {
  public:
    response() = default;

    explicit response(status_code status);

    status_code const&
    status() const;

    void
    set_status(status_code status);

    void
    set_status(status_code status, std::string reason);

    std::string const&
    reason() const;

    void
    set_reason(std::string reason);

  private:
    status_code status_;
    std::string reason_;
  };

  void
  to_buffers(std::vector< asio::const_buffer >& buffers, response const& r);

  std::vector< asio::const_buffer >
  to_buffers(response const& r);

} // namespace protocol
} // namespace http

#include <libhttp/protocol/response.ixx>

#endif
