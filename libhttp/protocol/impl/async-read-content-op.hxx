#ifndef libhttp__protocol__async_read_content_op_hxx_
#define libhttp__protocol__async_read_content_op_hxx_

namespace http {
namespace protocol {
  namespace impl {

    template< typename AsyncReadStream,
              typename InputBuffer,
              typename ContentBuffer,
              typename CompletionToken >
    struct async_read_content_op {
      AsyncReadStream& stream_;
      std::size_t want_;
      InputBuffer& input_buffer_;
      ContentBuffer& content_;
      CompletionToken token_;

      enum { starting, reading_content } state_{ starting };

      async_read_content_op(AsyncReadStream& stream,
                            std::size_t want,
                            InputBuffer& input_buffer,
                            ContentBuffer& content_,
                            CompletionToken token)
        : stream_{ stream },
          want_{ want },
          input_buffer_{ input_buffer },
          content_{ content_ },
          token_{ token }
      {}

      async_read_content_op(async_read_content_op const& other)
        : stream_{ other.stream_ },
          want_{ other.want_ },
          input_buffer_{ other.input_buffer_ },
          content_{ other.content_ },
          token_{ other.token_ },
          state_{ other.state_ }
      {}

      async_read_content_op(async_read_content_op&& other)
        : stream_{ other.stream_ },
          want_{ other.want_ },
          input_buffer_{ other.input_buffer_ },
          content_{ other.content_ },
          token_{ std::move(other.token_) },
          state_{ other.state_ }
      {}

      void operator()(std::error_code const& ec = {},
                      std::size_t bytes_transferred = 0)
      {
        switch (state_) {
          case starting:
            // copy available bytes from input_buffer_ to content_.
            if (input_buffer_.size() > 0) {
              auto avail = std::min(input_buffer_.size(), want_);

              auto copied = asio::buffer_copy(content_.prepare(avail),
                                              input_buffer_.data(), avail);

              content_.commit(copied);
              input_buffer_.consume(copied);
              want_ -= copied;
            }

            if (want_ > 0) {
              state_ = reading_content;
              asio::async_read(stream_, content_.prepare(want_),
                               asio::transfer_exactly(want_), std::move(*this));
              return;
            }

            asio::post(stream_.get_executor(),
                       std::bind(token_, ec, bytes_transferred));
            break;

          case reading_content:
            if (ec) {
              token_(ec, 0);
              return;
            }

            content_.commit(bytes_transferred);
            token_({}, bytes_transferred);
            // upcall bytes_transferred
            break;
        }
      }
    };

    template< typename AsyncReadStream,
              typename InputBuffer,
              typename ContentBuffer,
              typename CompletionToken >
    void
    start_async_read_content_op(AsyncReadStream& stream,
                                std::size_t want,
                                InputBuffer& input_buffer,
                                ContentBuffer& content,
                                CompletionToken& token)
    {
      async_read_content_op< AsyncReadStream, InputBuffer, ContentBuffer,
                             CompletionToken >{ stream, want, input_buffer,
                                                content, token }();
    }

  } // namespace impl
} // namespace protocol
} // namespace http

#endif
