#ifndef libhttp__server__connection_hxx_
#define libhttp__server__connection_hxx_

#include <libhttp/protocol/read-message.hxx>

#include <libhttp/server/exchange.hxx>
#include <libhttp/server/request-handler.hxx>

#include <asio.hpp>

#include <algorithm>
#include <functional>
#include <memory>

namespace http {
namespace server {

  // Does not indicate if the keep-alive or close was requested
  // by the client or the server.
  enum class continuation_policy { keep_alive, close };

  // TODO rename to session
  class connection : public std::enable_shared_from_this< connection > {
  public:
    ~connection()
    {
      std::cout << "connection destroyed!\n";
    }

    void
    start();

    connection(connection const&) = delete;
    connection(connection&&) = delete;

    connection&
    operator=(connection const&) = delete;

    connection&
    operator=(connection&&) = delete;

  public:
    // FIXME Make this private.
    // FIXME Take request handler instead of factory.
    connection(asio::ip::tcp::socket socket, request_handler& handler);

  private:
    // FIXME Rename get_current_exchange.
    exchange&
    get_current_tx();

    void
    init_read_request();

    void
    on_read_request(std::error_code ec,
                    std::size_t bytes_transferred,
                    std::size_t bytes_consumed);

    void
    init_dispatch(std::size_t pending);

    void
    dispatch(std::size_t content_length);

    void
    on_complete();

    void
    close_and_wait();

  private:
    asio::ip::tcp::socket socket_;
    request_handler& handler_;
    protocol::request request_;

    std::unique_ptr< exchange > tx_;
  };

  inline shared_ptr< connection >
  make_connection(asio::ip::tcp::socket socket, request_handler& handler)
  {
    return std::make_shared< connection >(std::move(socket), handler);
  }

} // namespace server
} // namespace http

#include <libhttp/server/connection.ixx>

#endif
