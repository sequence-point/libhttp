#ifndef libhttp__protocol__impl__async_read_request_with_content_op_hxx_
#define libhttp__protocol__impl__async_read_request_with_content_op_hxx_

namespace http {
namespace protocol {
  namespace impl {

    template< typename AsyncReadStream,
              typename InputBuffer,
              typename ContentBuffer,
              typename CompletionToken >
    struct async_read_request_with_content_op {
      AsyncReadStream& stream_;
      InputBuffer& input_buffer_;
      request& request_;
      ContentBuffer& content_buffer_;
      CompletionToken token_;
      std::size_t total_transferred_{ 0 };

      enum { starting, reading_request, reading_content } state_{ starting };

      async_read_request_with_content_op(AsyncReadStream& stream,
                                         InputBuffer& input_buffer,
                                         request& r,
                                         ContentBuffer& content_buffer,
                                         CompletionToken token)
        : stream_{ stream },
          input_buffer_{ input_buffer },
          request_{ r },
          content_buffer_{ content_buffer },
          token_{ std::move(token) }
      {}

      async_read_request_with_content_op(
        async_read_request_with_content_op const& other)
        : stream_{ other.stream_ },
          input_buffer_{ other.input_buffer_ },
          request_{ other.request_ },
          content_buffer_{ other.content_buffer_ },
          token_{ other.token_ },
          total_transferred_{ other.total_transferred_ },
          state_{ other.state_ }
      {}

      async_read_request_with_content_op(
        async_read_request_with_content_op&& other)
        : stream_{ other.stream_ },
          input_buffer_{ other.input_buffer_ },
          request_{ other.request_ },
          content_buffer_{ other.content_buffer_ },
          token_(std::move(other.token_)),
          total_transferred_{ other.total_transferred_ },
          state_{ other.state_ }
      {}

      void operator()(std::error_code const& ec = {},
                      std::size_t bytes_transferred = 0)
      {
        total_transferred_ += bytes_transferred;

        switch (state_) {
          case starting:
            state_ = reading_request;
            async_read_request(stream_, input_buffer_, request_,
                               std::move(*this));
            break;

          case reading_request:
            if (ec) {
              token_(ec, 0);
              break;
            }

            state_ = reading_content;
            async_read_content(stream_, input_buffer_, content_buffer_,
                               request_, std::move(*this));
            break;

          case reading_content:
            // We're already in a continuation so asio::post()
            // shouldn't be required.
            token_(ec, total_transferred_);
        }
      }
    };

    template< typename AsyncReadStream,
              typename InputBuffer,
              typename ContentBuffer,
              typename CompletionToken >
    void
    start_async_read_request_with_content_op(AsyncReadStream& stream,
                                             InputBuffer& input_buffer,
                                             request& r,
                                             ContentBuffer& content_buffer,
                                             CompletionToken& token)
    {
      async_read_request_with_content_op< AsyncReadStream, InputBuffer,
                                          ContentBuffer, CompletionToken >{
        stream, input_buffer, r, content_buffer, token
      }();
    }

    template< typename AsyncReadStream,
              typename InputBuffer,
              typename ContentBuffer,
              typename CompletionToken >
    struct async_read_response_with_content_op {
      AsyncReadStream& stream_;
      InputBuffer& input_buffer_;
      response& response_;
      ContentBuffer& content_buffer_;
      CompletionToken token_;
      std::size_t total_transferred_{ 0 };

      enum { starting, reading_response, reading_content } state_{ starting };

      async_read_response_with_content_op(AsyncReadStream& stream,
                                          InputBuffer& input_buffer,
                                          response& r,
                                          ContentBuffer& content_buffer,
                                          CompletionToken token)
        : stream_{ stream },
          input_buffer_{ input_buffer },
          response_{ r },
          content_buffer_{ content_buffer },
          token_{ std::move(token) }
      {}

      async_read_response_with_content_op(
        async_read_response_with_content_op const& other)
        : stream_{ other.stream_ },
          input_buffer_{ other.input_buffer_ },
          response_{ other.response_ },
          content_buffer_{ other.content_buffer_ },
          token_{ other.token_ },
          total_transferred_{ other.total_transferred_ },
          state_{ other.state_ }
      {}

      async_read_response_with_content_op(
        async_read_response_with_content_op&& other)
        : stream_{ other.stream_ },
          input_buffer_{ other.input_buffer_ },
          response_{ other.response_ },
          content_buffer_{ other.content_buffer_ },
          token_(std::move(other.token_)),
          total_transferred_{ other.total_transferred_ },
          state_{ other.state_ }
      {}

      void operator()(std::error_code const& ec = {},
                      std::size_t bytes_transferred = 0)
      {
        total_transferred_ += bytes_transferred;

        switch (state_) {
          case starting:
            state_ = reading_response;
            async_read_response(stream_, input_buffer_, response_,
                                std::move(*this));
            break;

          case reading_response:
            state_ = reading_content;
            async_read_content(stream_, input_buffer_, content_buffer_,
                               response_, std::move(*this));
            break;

          case reading_content:
            // We're already in a continuation so asio::post()
            // shouldn't be required.
            token_(ec, total_transferred_);
        }
      }
    };

    template< typename AsyncReadStream,
              typename InputBuffer,
              typename ContentBuffer,
              typename CompletionToken >
    void
    start_async_read_response_with_content_op(AsyncReadStream& stream,
                                              InputBuffer& input_buffer,
                                              response& r,
                                              ContentBuffer& content_buffer,
                                              CompletionToken& token)
    {
      async_read_response_with_content_op< AsyncReadStream, InputBuffer,
                                           ContentBuffer, CompletionToken >{
        stream, input_buffer, r, content_buffer, token
      }();
    }

  } // namespace impl
} // namespace protocol
} // namespace http

#endif
