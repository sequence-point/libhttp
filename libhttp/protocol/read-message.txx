namespace http {
namespace protocol {

  template< typename Parser,
            typename SyncReadStream,
            typename DynamicBuffer,
            typename Message >
  std::size_t
  read_message(SyncReadStream& s, DynamicBuffer& buffer, Message& m)
  {
    std::error_code ec;
    auto bytes_transferred = read_message< Parser >(s, buffer, m, ec);

    if (ec)
      throw std::system_error{ ec };

    return bytes_transferred;
  }

  template< typename Parser,
            typename SyncReadStream,
            typename DynamicBuffer,
            typename Message >
  std::size_t
  read_message(SyncReadStream& s,
               DynamicBuffer& buffer,
               Message& m,
               std::error_code& ec)
  {
    std::error_code parse_ec;

    auto bytes_transferred = asio::read_until(
      s, buffer, match_message< Parser, Message >{ m, parse_ec }, ec);

    // Override the error code if the matcher set one.
    if (ec) {
      ec = parse_ec;
      return 0;
    }

    buffer.consume(bytes_transferred);
    return bytes_transferred;
  }

  template< typename Parser,
            typename AsyncReadStream,
            typename DynamicBuffer,
            typename Message,
            typename CompletionToken >
  struct async_read_message_op {
    AsyncReadStream& stream_;
    DynamicBuffer& buffer_;
    Message& message_;
    CompletionToken token_;
    std::size_t total_transferred_{ 0 };

    enum { starting, reading } state_{ starting };

    async_read_message_op(AsyncReadStream& stream,
                          DynamicBuffer& buffer,
                          Message& message,
                          CompletionToken token)
      : stream_{ stream },
        buffer_{ buffer },
        message_{ message },
        token_{ std::move(token) }
    {}

    async_read_message_op(async_read_message_op const& other) = delete;

    async_read_message_op(async_read_message_op const&& other)
      : stream_{ other.stream_ },
        buffer_{ other.buffer_ },
        message_{ other.message_ },
        token_{ std::move(other.token_) },
        total_transferred_{ other.total_transferred_ },
        state_{ other.state_ }
    {}

    void operator()(std::error_code const& ec = {},
                    std::size_t bytes_transferred = 0)
    {
      total_transferred_ += bytes_transferred;

      switch (state_) {
        case reading:
          std::cout << "Read " << bytes_transferred << " bytes!\n";

          if (ec || (!ec && bytes_transferred == 0)) {
            std::cout << "Got error or zero read success\n";
            complete(ec, 0);
            return;
          }

          buffer_.commit(bytes_transferred);

          // fall through

        case starting:
          if (buffer_.size() > 0) {
            std::cout << "trying to parse\n";
            std::error_code parse_ec;

            Parser parser;
            auto begin = asio::buffers_begin(buffer_.data());
            auto end =
              parser.parse(begin, asio::buffers_end(buffer_.data()), parse_ec);

            if (!parse_ec) {
              std::cout << "Complete! No parser-error.";
              extract_message(parser, message_);
              buffer_.consume(std::distance(begin, end));
              complete({}, total_transferred_);
              return;
            }
            else if (parse_ec != parse_error::incomplete_message) {
              std::cout << "Received parser error\n";
              complete(parse_ec, total_transferred_);
              return;
            }

            std::cout << "Received incomplete message, reading more.";
          }
          else {
            std::cout << "no data in buffer, reading\n";
          }

          state_ = reading;
          std::size_t bytes_to_read = 512;

          stream_.async_read_some(buffer_.prepare(bytes_to_read),
                                  std::move(*this));
      }
    }

    void
    complete(std::error_code const& ec, std::size_t bytes_transferred)
    {
      if (state_ == starting) {
        auto bound = [=, this]() { token_(ec, bytes_transferred); };

        asio::post(stream_.get_executor(), bound);
        return;
      }

      token_(ec, bytes_transferred);
    }
  };

  template< typename Parser,
            typename AsyncReadStream,
            typename DynamicBuffer,
            typename Message,
            typename CompletionToken >

  void
  start_async_read_message_op(AsyncReadStream& stream,
                              DynamicBuffer& buffer,
                              Message& m,
                              CompletionToken& token)
  {
    std::cout << "starting async_read_message_op\n";
    async_read_message_op< Parser, AsyncReadStream, DynamicBuffer, Message,
                           CompletionToken >{ stream, buffer, m, token }();
  }

  template< typename Parser,
            typename AsyncReadStream,
            typename DynamicBuffer,
            typename Message,
            typename CompletionToken >
  auto
  async_read_message(AsyncReadStream& stream,
                     DynamicBuffer& buffer,
                     Message& message,
                     CompletionToken&& token)
  {
    asio::async_completion< CompletionToken,
                            void(std::error_code, std::size_t) >
      init{ token };

    start_async_read_message_op< Parser >(stream, buffer, message,
                                          init.completion_handler);

    return init.result.get();
  }

} // namespace protocol
} // namespace http
