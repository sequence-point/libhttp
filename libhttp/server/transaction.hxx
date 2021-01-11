#ifndef libhttp__server__transaction_hxx_
#define libhttp__server__transaction_hxx_

#include <libhttp/system/allocator.hxx>
#include <libhttp/system/extension-context.hxx>

#include <libhttp/protocol/request.hxx>
#include <libhttp/protocol/response.hxx>

#include <libhttp/server/common-error.hxx>

#include <istream>

namespace http::server {

//! Polymorphic base class for transactions.
class transaction {
public:
  class stream;

  //! Access the request message.
  virtual protocol::request&
  request() = 0;

  //! Access the request message.
  virtual protocol::request const&
  request() const = 0;

  //! Access the request content.
  virtual std::istream&
  request_content() = 0;

  //! Access the response message.
  virtual protocol::response&
  response() = 0;

  //! Access the response message.
  virtual protocol::response const
  response() const = 0;

  //! Send a response.
  virtual void
  send(asio::const_buffer const& content) = 0;

  //! Send a response.
  virtual void
  send(stream const& content) = 0;

  //! Send error.
  virtual void
  send_error(common_error const& error) = 0;

  //! Allocate a new stream owned by this transaction.
  stream
  alloc_stream();

  //! Access the transaction allocator.
  system::allocator&
  memory();

  //! Access the extensions manager.
  system::extension_context&
  extensions();

protected:
  //! Construct a new transaction.
  transaction() = default;

  //! Destroy this transaction.
  ~transaction() noexcept = default;

private:
  system::allocator allocator_;
  system::extension_context extensions_;
};

class transaction::stream : public std::iostream {
public:
  asio::streambuf*
  rdbuf() const
  {
    return buf_;
  }

  std::size_t
  size() const
  {
    return buf_->size();
  }

private:
  friend transaction;

  explicit stream(asio::streambuf* buf) : buf_{ buf }, std::iostream{ buf }
  {}

  asio::streambuf* buf_;
};

} // namespace http::server

#include <libhttp/server/transaction.ixx>

#endif
