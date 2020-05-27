#ifndef libhttp__server__transaction_hxx_
#define libhttp__server__transaction_hxx_

#include <libhttp/system/allocator.hxx>
#include <libhttp/system/extension-context.hxx>

#include <libhttp/protocol/request.hxx>
#include <libhttp/protocol/response.hxx>

#include <libhttp/server/common-error.hxx>

#include <istream>

namespace http {
namespace server {

  class transaction {
  public:
    class stream;

    virtual protocol::request&
    request() = 0;
    virtual protocol::request const&
    request() const = 0;

    virtual std::istream&
    request_content() = 0;

    virtual protocol::response&
    response() = 0;
    virtual protocol::response const
    response() const = 0;

    virtual void
    send(asio::const_buffer const& content) = 0;
    virtual void
    send(stream const& content) = 0;
    virtual void
    send_error(common_error const& error) = 0;

    stream
    alloc_stream();

    system::allocator&
    memory();
    system::extension_context&
    extensions();

  protected:
    transaction() = default;
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

  private:
    friend class transaction;

    explicit stream(asio::streambuf* buf) : buf_{ buf }, std::iostream{ buf }
    {}

    asio::streambuf* buf_;
  };

} // namespace server
} // namespace http

#include <libhttp/server/transaction.ixx>

#endif
