namespace http {
namespace protocol {
  namespace impl {

    template< typename Parser,
              typename AsyncReadStream,
              typename DynamicBuffer,
              typename Message >
    struct async_read_message_op {
      AsyncReadStream& stream_;
      DynamicBffer& buffer_;
      Message& message_;
      std::size_t total_transferred_;

      enum { starting, reading } state_{ starting };

      template< typename Self >
      void operator()(Self& self,
                      std::error_code const& ec = {},
                      std::size_t bytes_transferred = 0)
      {
        total_transferred_ += bytes_transferred;

        switch (state_) {
          case reading:
            if (ec) {
              token_(ec, 0);
              break;
            }

            buffer_.commit(bytes_transferred);

            [[fallthrough]];

          case starting:
            // attempt to parse message already in buffer if we have data.
            if (buffer_.size() > 0) {
              std::error_code parse_ec;
              Parser parser;
              parser.parse(asio::buffers_begin(buffer_.data()),
                           asio::buffers_end(buffer_.data()), parse_ec);

              if (!parse_ec) {
                extract_message(parser, message_);
                self.complete({}, total_transferred_);
                break;
              }

              if (parse_ec != parse_error::incomplete_message) {
                self.complete(parse_ec, 0);
                break;
              }

              // we received an incomplete message, read more.
            }

            state_ = reading;
            std::size_t bytes_to_read = 512;
            stream_.async_read_some(buffer_.prepare(bytes_to_read),
                                    std::move(self));
            break;
        }
      }
    };

  } // namespace impl
} // namespace protocol
} // namespace http
