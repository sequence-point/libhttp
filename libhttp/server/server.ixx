#include <vector> // TODO remove this?

namespace http::server {

inline server::server(asio::io_context& io_context,
                      asio::ip::tcp::endpoint endpoint,
                      request_handler handler)
  : io_context_{ io_context },
    acceptor_{ io_context },
    next_client_{ io_context },
    handler_{ handler }
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
    case common_error::not_found: {
      static std::string message{ "Resource not found\n" };

      tx.response().set_status(protocol::status_code{ 404 });
      tx.send(asio::buffer(message));
      return;
    }
  }

  throw std::invalid_argument{ "unknown common error" };
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

  std::cout << "received connection\n";

  auto peer = std::move(next_client_);

  init_accept();

  if (ec)
    return; // TODO log

  std::cout << "received connection\n";

  auto on_tx_complete = [](std::error_code const& ec) {
    std::cout << "transaction complete\n";
  };

  common::http_connection conn{ std::move(peer) };
  auto tx = new http_transaction{ io_context_, *this, std::move(conn), handler_,
                                  on_tx_complete };
  tx->start();

  //      std::make_shared< http_session >(std::move(conn), handler_);

  // std::make_shared< http_session >(std::move(conn), handler_)->start();
  //(new http_session{ std::move(conn), handler_ })->start();
}

} // namespace http::server
