#ifndef undefined__libhttp__common__basic_connection_hxx_
#define undefined__libhttp__common__basic_connection_hxx_

#include <libhttp/protocol/read-content.hxx>
#include <libhttp/protocol/read-request.hxx>
#include <libhttp/protocol/read-response.hxx>
#include <libhttp/protocol/read-with-content.hxx>
#include <libhttp/protocol/request.hxx>
#include <libhttp/protocol/response.hxx>
#include <libhttp/protocol/write-message.hxx>

#include <asio.hpp>
#include <asio/ssl.hpp>

#include <system_error>
#include <type_traits>

namespace http {
namespace common {

  // TODO rename basic_http_connection
  template< typename StreamType >
  class basic_connection {
  public:
    using stream_type = StreamType;

    explicit basic_connection(stream_type stream);

    basic_connection(basic_connection const&) = delete;

    basic_connection(basic_connection&& other);

    stream_type&
    get_stream();
    stream_type const&
    get_stream() const;

    //
    // read_request
    //

    std::size_t
    read_request(protocol::request& r);

    std::size_t
    read_request(protocol::request& r, std::error_code& ec);

    template< typename CompletionToken >
    auto
    async_read_request(protocol::request& r, CompletionToken token);

    //
    // read_response
    //

    std::size_t
    read_response(protocol::response& r);

    std::size_t
    read_response(protocol::response& r, std::error_code& ec);

    template< typename CompletionToken >
    auto
    async_read_response(protocol::response& r, CompletionToken token);

    //
    // read_content
    //

    template< typename ContentBuffer, typename CompletionToken >
    auto
    async_read_content(protocol::request const& r,
                       ContentBuffer& content,
                       CompletionToken&& token);

    template< typename ContentBuffer, typename CompletionToken >
    auto
    async_read_content(protocol::response const& r,
                       ContentBuffer& content,
                       CompletionToken&& token);

    //
    // write_request
    //

    template< typename ConstBufferSequence >
    std::size_t
    write_request(protocol::request const& r,
                  ConstBufferSequence const& content);

    template< typename ConstBufferSequence >
    std::size_t
    write_request(protocol::request const& r,
                  ConstBufferSequence const& content,
                  std::error_code& ec);

    //
    // write_response
    //

    template< typename ConstBufferSequence >
    std::size_t
    write_response(protocol::response const& r,
                   ConstBufferSequence const& content);

    template< typename ConstBufferSequence >
    std::size_t
    write_response(protocol::response const& r,
                   ConstBufferSequence const& content,
                   std::error_code& ec);

  private:
    stream_type stream_;
    asio::streambuf input_buffer_;
  };

  //! \brief Alias for an HTTP connection.
  using http_connection = basic_connection< asio::ip::tcp::socket >;

  //! \brief Alias for an SSL socket.
  using ssl_socket = asio::ssl::stream< asio::ip::tcp::socket >;

  //! \brief Alias for an HTTPS connection.
  using https_connection = basic_connection< ssl_socket >;

} // namespace common
} // namespace http

#include <libhttp/common/basic-connection.ixx>

#endif
