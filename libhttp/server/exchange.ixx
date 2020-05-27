namespace http {
namespace server {

  inline exchange::exchange(socket& s,
                            protocol::request request,
                            asio::const_buffer content,
                            completion_handler on_complete)
    : s_{ s },
      request_{ request },
      content_{ content },
      on_complete_{ on_complete }
  {}

  inline void
  exchange::before_send(before_send_callback cb)
  {
    before_send_callbacks_.emplace_back(std::move(cb));
  }

  inline void
  exchange::send(protocol::response r, asio::const_buffer const& content)
  {
    prepare_response(r, asio::buffer_size(content));
    call_before_send_callbacks(r);

    response_ = std::move(r);

    auto on_write_response = [this, content](std::error_code const& ec) {
      if (ec) {
        complete(ec);
        return;
      };

      auto on_write = [this](std::error_code const& ec, size_t) {
        complete(ec);
      };

      asio::async_write(s_, content, on_write);
    };

    protocol::async_write_response(s_, *response_, on_write_response);
  }

  inline void
  exchange::send(protocol::response r, stream& s)
  {
    prepare_response(r, s.rdbuf()->size());
    call_before_send_callbacks(r);

    response_ = std::move(r);

    auto on_write_response = [this,
                              content = s.rdbuf()](std::error_code const& ec) {
      if (ec) {
        complete(ec);
        return;
      };

      auto on_write = [this](std::error_code const& ec, size_t) {
        complete(ec);
      };

      asio::async_write(s_, *content, on_write);
    };

    protocol::async_write_response(s_, *response_, on_write_response);
  }

  inline void
  exchange::prepare_response(protocol::response& r, std::size_t content_length)
  {
    // TODO Validate response status? >= 100 && <= 999?

    // FIXME Leave version as-is if not equal to 0.0?
    r.set_version(request().version());

    namespace rfc7230 = protocol::rfc7230;
    namespace rfc7231 = protocol::rfc7231;

    r.set_if_not_set< rfc7231::Server >("libhttp");
    r.set_if_not_set< rfc7231::Date >(system_clock::now());

    // FIXME Set Last-Modified header if already set and in the future
    //       according to Date header just set.

    if (request().method() != "HEAD") {
      // We cannot blindly overwrite Content-Length here
      // since it may already have been set for a reason, e.g. as
      // the result of a HEAD request. For this reason we also
      // don't set it if the request method is HEAD.
      r.set_if_not_set< rfc7230::ContentLength >(content_length);

      // Ensure Content-Type is set to application/octet-stream
      // if not set explicitly by the sender.
      r.set_if_not_set< rfc7231::ContentType >("application", "octet-stream");
    }

    // FIXME Connection close handling is currently very naive.
    r.set< rfc7230::Connection >("close");
  }

  inline void
  exchange::call_before_send_callbacks(protocol::response& response)
  {
    for (auto& cb : before_send_callbacks_)
      cb(*this, response);
  }

  inline void
  exchange::complete(error_code const& ec)
  {
    // TODO use dispatch

    // FIXME remove this copy when using dispatch
    auto on_complete = on_complete_;

    if (on_complete)
      on_complete(ec);
  }

} // namespace server
} // namespace http

exchange server side : start : have request end : have response to request

                                                    process
  : take request,
    hand off to some handler,
    complete or error out

                  exchange client side : start have request end : have response

                                                                    process
  : send over wire,
    hand off to some handler,
    complete or error out

                    handlers
  : prepare_request_for_send prepare_response_for_send
      prepare_request_for_receive prepare_response_for_receive

        client::filter <
    -type erasing class filter_request_to_server filter_response_from_server

      server::filter <
    -type erasing class filter_request_from_client filter_response_to_client

       cookie_filter::process_request_coming_from_client exchange
         .extend< cookie_jar >()
         .fill(ex.request());

process_response_going_to_client exchange.get< cookie_jar >.pour(ex.response());
