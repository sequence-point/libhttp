#ifndef libhttp__server__server_hxx_
#define libhttp__server__server_hxx_

#include <libhttp/common/basic-connection.hxx>

#include <libhttp/server/basic-transaction.hxx>
#include <libhttp/server/common-error.hxx>
#include <libhttp/server/request-handler.hxx>

#include <iostream>

#include <asio.hpp>

namespace http::server {

class server {
public:
  server(asio::io_context& io_context, asio::ip::tcp::endpoint endpoint, request_handler handler);

  void
  start();

  void
  stop();

protected:
  virtual void
  send_common_error(transaction& tx, common_error const& error);

  virtual void
  on_exception(transaction& tx);

private:
  using http_transaction = basic_transaction< server, common::http_connection >;
  using https_transaction = basic_transaction< server, common::https_connection >;

  friend http_transaction;
  friend https_transaction;

  void
  init_accept();

  void
  on_accept(std::error_code const& ec);

  asio::io_context& io_context_;
  asio::ip::tcp::acceptor acceptor_;
  asio::ip::tcp::socket next_client_;

  request_handler handler_;
};

} // namespace http::server

#include "server.ixx"

#endif
