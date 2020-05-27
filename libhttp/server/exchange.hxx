#ifndef libhttp__exchange_hxx_
#define libhttp__exchange_hxx_

#include <libhttp/common/exchange.hxx>

#include <libhttp/protocol/request.hxx>
#include <libhttp/protocol/response.hxx>
#include <libhttp/protocol/status-code.hxx>
#include <libhttp/protocol/write-message.hxx>

#include <libhttp/protocol/rfc7230/connection.hxx>
#include <libhttp/protocol/rfc7230/content-length.hxx>
#include <libhttp/protocol/rfc7231/content-type.hxx>
#include <libhttp/protocol/rfc7231/date.hxx>
#include <libhttp/protocol/rfc7231/server.hxx>

#include <asio.hpp>

#include <sstream>

namespace http {
namespace server {

  class exchange : public common::exchange {
  public:
    exchange(protocol::http_connection connection);

        asio::ip::tcp::socket& s,
               protocol::request request,
               asio::const_buffer content,
               completion_handler on_complete);

        exchange(exchange const&) = delete;
        exchange(exchange&&) = delete;

        // FIXME Add callback support for send, for use when sending
        // 1xx-messages.

        // TODO Document that the buffer argument must be valid until after the
        //      send operation has completed.

        // clang-format off

      //
      // Base forms
      //

      void send(protocol::response r, asio::const_buffer const& content);
      void send(protocol::response r, stream& content);

      //
      // Status + content type + content
      //

      void send(protocol::status_code status, protocol::media_type content_type, asio::const_buffer const& content)
      {
        protocol::response r;
        r.set_status(status);
        r.set< protocol::rfc7231::ContentType >(std::move(content_type));
        send(std::move(r), content);
      }

      void send(protocol::status_code status, protocol::media_type content_type, stream& content)
      {
        protocol::response r;
        r.set_status(status);
        r.set< protocol::rfc7231::ContentType >(std::move(content_type));
        send(std::move(r), content);
      }

      void send(protocol::status_code status, protocol::media_type content_type, std::string content)
      {
        protocol::response r;
        r.set_status(status);
        r.set< protocol::rfc7231::ContentType >(std::move(content_type));

        // Allocate a new string and move content into it.
        auto& allocated_string = alloc< std::string >(std::move(content));

        send(std::move(r), buffer(allocated_string));
      }

      //
      // Status + content
      //

      void send(protocol::status_code status, asio::const_buffer const& content) // sends application/octet-stream
      {
        send(std::move(status), { "application", "octet-stream" }, content);
      }

      void send(protocol::status_code status, stream& content) // sends application/octet-stream
      {
        send(std::move(status), { "application", "octet-stream" }, content);
      }

      void send(protocol::status_code status, string content) // sends text/plain
      {
        send(std::move(status), { "text", "plain" }, std::move(content));
      }

      //
      // Content type + content
      //

      void send(protocol::media_type content_type, asio::const_buffer const& content)
      {
        send(200, std::move(content_type), content);
      }

      void send(protocol::media_type content_type, stream& content)
      {
        send(200, std::move(content_type), content);
      }

      void send(protocol::media_type content_type, string content)
      {
        send(200, std::move(content_type), std::move(content));
      }

      //
      // Content
      //

      void send(asio::const_buffer const& content) // sends 200 + application/octet-stream
      {
        send(200, content);
      }

      void send(stream& content) // sends 200 + application/octet-stream
      {
        send(200, content);
      }

      void send(string content) // sends 200 + text/plain
      {
        send(200, std::move(content));
      }

      //
      // Status
      //

      void send(protocol::status_code status) // sends empty response
      {
        protocol::response r;
        r.set_status(status);
        send(std::move(r), asio::const_buffer{});
      }

      void upgrade(leftover_buffer, std::string protocol, handler)
      {
        // copy leftover
        // set Upgrade header
        // send, invoke handler
      }

      // TODO send_chunk(...)

        // clang-format on

        exchange&
        operator=(exchange const&) = delete;

        exchange&
        operator=(exchange&&) = delete;

      private:
        void
        prepare_response(protocol::response& response,
                         std::size_t content_length);

        void
        complete(error_code const& ec);

        asio::ip::tcp::socket& s_;
  };

} // namespace server
} // namespace http

#include <libhttp/server/exchange.ixx>

#endif
