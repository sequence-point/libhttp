#ifndef libhttp__server__http_acceptor_hxx_
#define libhttp__server__http_acceptor_hxx_

#include <asio.hpp>

namespace http {
namespace server {
  namespace impl {

    struct http_acceptor_async_accept_op {
      http_connection& connection_;
      asio::ip::tcp::socket peer_;

      enum { starting, accepting } state_;

      void operator()(std::error_code const& ec = {})
      {
        switch (state_) {
          case starting: break;

          case accepting:
            if (ec) {
              token_(ec);
              break;
            }

            connection_ = http_connection{ std::move(peer_) };

            break;
        }
      }
    };

  } // namespace impl
} // namespace server
} // namespace http

namespace http {
namespace server {

  class http_acceptor {
  public:
    template< typename CompletionHandler >
    void
    async_accept(http_connection& connection, CompletionHandler&& token)
    {}

  private:
    asio::ip::tcp::acceptor acceptor_;
  };

} // namespace server
} // namespace http

#endif
