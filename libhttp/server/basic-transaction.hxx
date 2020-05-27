#ifndef libhttp__server__basic_transaction_hxx_
#define libhttp__server__basic_transaction_hxx_

#include <libhttp/server/request-handler.hxx>
#include <libhttp/server/transaction.hxx>

#include <libhttp/protocol/rfc7230/host.hxx>

#include <functional>
#include <iostream> // TODO remove

namespace http::server {

template< typename Server, typename ConnectionType >
class basic_transaction : public transaction {
public:
  using server_type = Server;
  using connection_type = ConnectionType;

  using completion_handler = std::function< void(std::error_code) >;

  basic_transaction(asio::io_context& io_context,
                    server_type& server,
                    connection_type connection,
                    request_handler& handler,
                    completion_handler on_complete)
    : io_context_{ io_context },
      server_{ server },
      connection_{ std::move(connection) },
      handler_{ handler },
      on_complete_{ on_complete },
      request_content_stream_{ &request_content_ }
  {}

  server_type&
  get_server()
  {
    return server_;
  }

  connection_type&
  get_connection()
  {
    return connection_;
  }

  connection_type const&
  get_connection() const
  {
    return connection_;
  }

  protocol::request&
  request() override
  {
    return request_;
  }

  protocol::request const&
  request() const override
  {
    return request_;
  }

  std::istream&
  request_content() override
  {
    return request_content_stream_;
  }

  protocol::response&
  response() override
  {
    return response_;
  }

  protocol::response const
  response() const override
  {
    return response_;
  }

  void
  send(asio::const_buffer const& content) override
  {
    init_write_response(content);
  }

  void
  send(stream const& content) override
  {
    init_write_response(content.rdbuf()->data());
  }

  void
  send_error(common_error const& error) override
  {
    get_server().send_common_error(*this, error);
  }

  void
  start()
  {
    std::cout << "Staring transaction\n";
    init_read_request();
  }

protected:
  void
  init_read_request()
  {
    std::cout << "Begin reading TX request...\n";

    auto on_read_request = [this](std::error_code const& ec,
                                  std::size_t bytes_transferred) {
      this->on_read_request(ec, bytes_transferred);
    };

    get_connection().async_read_request(request_, std::move(on_read_request));
  }

  void
  on_read_request(std::error_code const& ec, std::size_t bytes_transferred)
  {

    if (ec) {
      return; // TODO how to handle?
    }

    std::cout << "Received request TX request...\n";

    // TODO Make sure the Host is set correctly.
    // TODO Bail if request_.target_uri().host() and Host disagree.

    // Determine if the request actually has content.
    // TODO read content if request can be expected
    // to contain content.

    init_read_content();
  }

  void
  init_read_content()
  {
    std::cout << "Starting to read content...\n";

    auto on_read_content = [this](std::error_code const& ec,
                                  std::size_t bytes_transferred) {
      this->on_read_content(ec, bytes_transferred);
    };

    get_connection().async_read_content(request_, request_content_,
                                        on_read_content);
  }

  void
  on_read_content(std::error_code const& ec, std::size_t bytes_transferred)
  {
    if (ec) {
      std::cout << "Received error when reading request content\n";
      return; // TODO how to handle?
    }

    request_content_.commit(bytes_transferred);

    std::cout << "Got request content, preparing to dispatch\n";

    prepare_response();
    init_dispatch();
  }

  void
  prepare_response()
  {
    response_ = protocol::response{ 200 };
    response_.set_version(request_.version());
  }

  void
  init_dispatch()
  {
    auto bound = [this]() {
      try {
        handler_.invoke(*this);
      }
      catch (...) {
        server_.on_exception(*this);
      }
    };

    asio::post(io_context_, bound);

    // At this point the transaction is in the control of the handler.
  }

  void
  init_write_response(asio::const_buffer const& content)
  {
    // TODO make this async
    std::error_code ec;

    response_content_ = content;

    get_connection().write_response(response_, response_content_, ec);
    get_connection().get_stream().close();

    auto bound = [this]() { on_complete_({}); };
    asio::post(io_context_, bound);
  }

private:
  asio::io_context& io_context_;
  server_type& server_;
  connection_type connection_;
  request_handler& handler_;
  completion_handler on_complete_;

  protocol::request request_;
  asio::streambuf request_content_;
  std::istream request_content_stream_;

  protocol::response response_;
  asio::const_buffer response_content_;
};

} // namespace http::server

//#include <libhttp/server/basic-transaction.txx>

#endif
