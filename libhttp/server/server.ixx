#include <vector> // TODO remove this?

namespace http::server {

inline server::server(asio::io_context& io_context, asio::ip::tcp::endpoint endpoint, request_handler handler)
  : io_context_{ io_context }, acceptor_{ io_context }, next_client_{ io_context }, handler_{ handler }
{
  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();
}

inline void
server::start()
{
  std::cout << "Binding server start\n";
  init_accept();
}

inline void
server::stop()
{
  acceptor_.close();
}

inline void
server::send_common_error(transaction& tx, common_error const& error)
{
  switch (error) {
    case common_error::bad_request: {
      static std::string const message{ "Bad Request\n" };

      tx.response().set_status(protocol::status_code{ 400 });
      tx.response().set_header("content-type", "text/plain");

      tx.send(asio::buffer(message));
      return;
    }

    case common_error::payment_required: {
      static std::string const message{ "Payment Required\n" };

      tx.response().set_status(protocol::status_code{ 402 });
      tx.response().set_header("content-type", "text/plain");

      tx.send(asio::buffer(message));
      return;
    }

    case common_error::forbidden: {
      static std::string const message{ "Forbidden\n" };

      tx.response().set_status(protocol::status_code{ 403 });
      tx.response().set_header("content-type", "text/plain");

      tx.send(asio::buffer(message));
      return;
    }

    case common_error::not_found: {
      static std::string const message{ "Not Found\n" };

      tx.response().set_status(protocol::status_code{ 404 });
      tx.response().set_header("content-type", "text/plain");

      tx.send(asio::buffer(message));
      return;
    }

    case common_error::method_not_allowed: {
      static std::string const message{ "Method Not Allowed\n" };

      tx.response().set_status(protocol::status_code{ 405 });
      tx.response().set_header("content-type", "text/plain");

      tx.send(asio::buffer(message));
      return;
    }

    case common_error::not_acceptable: {
      static std::string const message{ "Not Acceptable\n" };

      tx.response().set_status(protocol::status_code{ 406 });
      tx.response().set_header("content-type", "text/plain");

      tx.send(asio::buffer(message));
      return;
    }

    case common_error::request_timeout: {
      static std::string const message{ "Request Timeout\n" };

      tx.response().set_status(protocol::status_code{ 408 });
      tx.response().set_header("content-type", "text/plain");

      tx.send(asio::buffer(message));
      return;
    }

    case common_error::conflict:
    case common_error::gone:
    case common_error::length_required:
    case common_error::payload_too_large:
    case common_error::uri_too_long:
    case common_error::unsupported_media_type:
    case common_error::expectation_failed:
    case common_error::upgrade_required: {
      break;
    }

    case common_error::internal_server_error: {
      static std::string const message{ "Internal Server Error\n" };

      tx.response().set_status(protocol::status_code{ 500 });
      tx.response().set_header("content-type", "text/plain");

      tx.send(asio::buffer(message));
      return;
    }

    case common_error::not_implemented: {
      static std::string const message{ "Not Implemented\n" };

      tx.response().set_status(protocol::status_code{ 501 });
      tx.response().set_header("content-type", "text/plain");

      tx.send(asio::buffer(message));
      return;
    }

    case common_error::bad_gateway: {
      static std::string const message{ "Bad Gateway\n" };

      tx.response().set_status(protocol::status_code{ 502 });
      tx.response().set_header("content-type", "text/plain");

      tx.send(asio::buffer(message));
      return;
    }

    case common_error::service_unavailable: {
      static std::string const message{ "Service Unavailable\n" };

      tx.response().set_status(protocol::status_code{ 503 });
      tx.response().set_header("content-type", "text/plain");

      tx.send(asio::buffer(message));
      return;
    }

    case common_error::gateway_timeout: {
      static std::string const message{ "Gateway Timeout\n" };

      tx.response().set_status(protocol::status_code{ 504 });
      tx.response().set_header("content-type", "text/plain");

      tx.send(asio::buffer(message));
      return;
    }

    case common_error::http_version_not_supported: {
      static std::string const message{ "HTTP Version Not Supported\n" };

      tx.response().set_status(protocol::status_code{ 502 });
      tx.response().set_header("content-type", "text/plain");

      tx.send(asio::buffer(message));
      return;
    }
  }

  throw std::invalid_argument{ "invalid common error" };
}

inline void
server::on_exception(transaction& tx)
{
  throw;
}

inline void
server::init_accept()
{
  std::cout << "Initializing accept\n";

  auto on_accept = [this](std::error_code const& ec) { this->on_accept(ec); };

  acceptor_.async_accept(next_client_, std::move(on_accept));
}

inline void
server::on_accept(std::error_code const& ec)
{
  if (!acceptor_.is_open())
    return;

  std::cout << " ** Received connection! **\n";

  auto peer = std::move(next_client_);

  init_accept();

  if (ec)
    return; // TODO log

  auto on_tx_complete = [](std::error_code const& ec) { std::cout << " ** Transaction completed! **\n\n"; };

  common::http_connection conn{ std::move(peer) };

  // TODO make basic_http_transaction std::enable_shared_from_this like ASIO does it.
  auto tx = new http_transaction{ io_context_, *this, std::move(conn), handler_, on_tx_complete };

  tx->start();
}

} // namespace http::server
