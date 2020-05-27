#ifndef libhttp__server__basic_session_hxx_
#define libhttp__server__basic_session_hxx_

#include <libhttp/common/basic-connection.hxx>

#include <libhttp/server/basic-transaction.hxx>
#include <libhttp/server/request-handler.hxx>
#include <libhttp/server/session.hxx>

#include <iostream>
#include <memory>

#include <asio.hpp>

namespace http {
namespace server {

  class transaction;

  template< typename ConnectionType >
  class basic_session {
  public:
    using connection_type = ConnectionType;
    using transaction_type = basic_transaction< connection_type >;

    // TODO make private, add make_session helper.
    basic_session(connection_type connection, request_handler& handler)
      : connection_{ std::move(connection) }, handler_{ handler }
    {}

    ~basic_session() noexcept
    {
      std::cout << "destroyed session\n";
    }

    void
    start()
    {
      std::cout << "starting session\n";
      begin_transaction();
    }

  private:
    void
    begin_transaction()
    {
      if (current_tx_)
        throw std::invalid_argument{ "already in transaction" };

      auto on_complete = [this, self](std::error_code const& ec) {};

      current_tx_.reset(new transaction_type{
        io_context_, connection_, handler_, std::move(on_complete) });
      current_tx_->start();
    }

    std::unique_ptr< transaction_type > current_tx_;

    connection_type connection_;
    request_handler& handler_;
  };

  using http_session = basic_session< common::http_connection >;
  using https_session = basic_session< common::https_connection >;

} // namespace server
} // namespace http

//#include <libhttp/server/session.ixx>

#endif
