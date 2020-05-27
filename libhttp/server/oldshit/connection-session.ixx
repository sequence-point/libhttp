#include <cstring>
#include <iostream>

namespace http {
namespace server {

  inline connection::connection(asio::ip::tcp::socket socket,
                                request_handler& handler)
    : socket_{ std::move(socket) }, handler_{ handler }
  {}

  inline void
  connection::start()
  {
    init_read_request();
  }

  inline exchange&
  connection::get_current_tx()
  {
    if (!tx_)
      throw std::invalid_argument{ "not in transaction" };

    return *tx_;
  }

  inline void
  connection::init_read_request()
  {
    auto on_read = [this, self = shared_from_this()](
                     std::error_code ec, std::size_t bytes_transferred,
                     std::size_t bytes_consumed) {
      on_read_request(ec, bytes_transferred, bytes_consumed);
    };

    protocol::async_read_request(socket_, asio::buffer(buffer_), 0, request_,
                                 on_read);
  }

  inline void
  connection::on_read_request(std::error_code ec,
                              std::size_t bytes_transferred,
                              std::size_t bytes_consumed)
  {
    if (ec == asio::error::operation_aborted)
      return;

    if (ec) {
      // TODO handle error.
      std::cerr << "error: " << ec << '\n';
      return;
    }

    // consume bytes_consumed bytes from the input buffer
    buf_consume(bytes_consumed);

    init_dispatch(bytes_transferred - bytes_consumed);
  }

  inline void
  connection::init_dispatch(std::size_t pending)
  {
    // TODO handle Transfer-Encoding messages!

    // FIXME Throws if Content-Length is unset or invalid.
    auto const maybe_content_length =
      request_.get< protocol::rfc7230::ContentLength >();

    size_t const content_length =
      maybe_content_length ? *maybe_content_length : 0;

    size_t const avail{ std::min(content_length, pending) };

    buf_commit(avail);
    buf_consume(avail);

    auto const want_more = content_length - avail;

    if (want_more < 1) {
      dispatch(content_length);
      return;
    }

    auto const read_length = want_more;
    auto const read_offset = avail;
    auto const read_cap = (2 * 1024 * 1024) - read_offset;

    auto on_read = [this, self = shared_from_this(),
                    content_length](std::error_code const& ec, std::size_t) {
      if (ec) {
        // TODO handle EC
        return;
      }

      dispatch(content_length);
    };

    asio::async_read(socket_, asio::buffer(content_ + read_offset, read_cap),
                     on_read);
  }

  inline void
  connection::dispatch(std::size_t content_length)
  {
    auto on_complete = [this, self = shared_from_this()](
                         std::error_code const&) { this->on_complete(); };

    asio::const_buffer content;

    if (content_length > 0)
      content = { content_, content_length };

    tx_.reset(
      new exchange{ socket_, std::move(request_), content, on_complete });

    handler_.invoke(*tx_);

    // Opa! We're now in a transaction.
  }

  inline void
  connection::on_complete()
  {
    tx_.reset();
    close_and_wait();
  }

  inline void
  connection::close_and_wait()
  {
    auto self = shared_from_this();

    std::error_code ignored_ec;
    socket_.shutdown(asio::ip::tcp::socket::shutdown_send, ignored_ec);

    auto on_read = [this, self](std::error_code ec, std::size_t) {
      socket_.close();
    };

    socket_.async_read_some(asio::null_buffers(), on_read);
  }

} // namespace server
} // namespace http
