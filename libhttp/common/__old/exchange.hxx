#ifndef libhttp__common__http_exchange_hxx_
#define libhttp__common__http_exchange_hxx_

#include <libhttp/system/extendable.hxx>

#include <libhttp/protocol/request.hxx>
#include <libhttp/protocol/response.hxx>

namespace http {
namespace common {

  class exchange : public system::extendable {
  public:
    using completion_handler = std::function< void(error_code const&) >;

    class stream;

    protocol::request const&
    request() const;

    const_buffer const&
    get_request_content() const;

    void
    replace_request(protocol::request request);

    void
    replace_request(protocol::request request, const_buffer content);

    void
    replace_request_content(const_buffer content);

    protocol::response const&
    response() const;

    const_buffer const&
    get_response_content() const;

    void
    replace_response(protocol::response response);

    void
    replace_response(protocol::response response, const_buffer content);

    void
    replace_response_content(const_buffer content);

    stream
    alloc_stream();

  protected:
    exchange() = default;

    completion_handler const&
    get_completion_handler() const;

    void
    set_completion_handler(completion_handler handler);

    static asio::streambuf&
    get_asio_streambuf(stream& s)
    {
      return s.get_internal_buffer();
    }

  private:
    protocol::request request_;
    const_buffer request_content_;

    protocol::response response_;
    const_buffer response_content_;

    completion_handler on_complete_;
  };

  class exchange::stream : public std::iostream {
  public:
    std::streambuf
    rdbuf() const
    {
      return buf_;
    }

  private:
    friend class exchange;

    asio::streambuf&
    get_internal_buffer() const
    {
      return buf_;
    }

    explicit stream(asio::streambuf& buf) : buf_{ buf }, std::iostream{ &buf }
    {}

    asio::streambuf* buf_;
  };

} // namespace common
} // namespace http

#include <libhttp/common/exchange.ixx>

#endif
