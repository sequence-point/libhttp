namespace http {
namespace protocol {

  template< typename SyncReadStream,
            typename DynamicBuffer,
            typename MutableBufferSequence >
  size_t
  read_content(SyncReadStream& s,
               DynamicBuffer& input_buffer,
               MutableBufferSequence const& content,
               size_t content_length)
  {
    std::error_code ec;

    auto bytes_transferred =
      read_content(s, input_buffer, content, content_length, ec);

    if (ec)
      throw std::system_error{ ec };

    return bytes_transferred;
  }

  template< typename SyncReadStream,
            typename DynamicBuffer,
            typename MutableBufferSequence >
  size_t
  read_content(SyncReadStream& s,
               DynamicBuffer& input_buffer,
               MutableBufferSequence const& content,
               size_t content_length,
               std::error_code& ec)
  {
    if (content_length > asio::buffer_size(content)) {
      ec = protocol_error::request_too_large;
      return 0;
    }

    auto want = content_length;
    auto buffer = content;

    if (input_buffer.size() > 0) {
      auto copied = asio::buffer_copy(buffer, input_buffer.data(),
                                      std::min(input_buffer.size(), want));
      input_buffer.consume(copied);
      want -= copied;
      buffer += copied;
    }

    if (want < 1)
      return 0;

    return asio::read(s, buffer, asio::transfer_exactly(want), ec);
  }

  template< typename SyncReadStream,
            typename DynamicBuffer,
            typename DynamicBuffer2 >
  size_t
  read_content(SyncReadStream& s,
               DynamicBuffer& input_buffer,
               DynamicBuffer2&& content,
               protocol::response const& response)
  {
    std::error_code ec;

    auto bytes_transferred =
      read_content(s, input_buffer, response, content, ec);

    if (ec)
      throw std::system_error{ ec };

    return bytes_transferred;
  }

  template< typename SyncReadStream,
            typename DynamicBuffer,
            typename DynamicBuffer2 >
  size_t
  read_content(SyncReadStream& s,
               DynamicBuffer& input_buffer,
               DynamicBuffer2&& content,
               request const& r,
               std::error_code& ec)
  {
    using rfc7230::ContentLength;

    // if (auto opt_transer_encoding = get< TransferEncoding >(r)) {
    //   TODO chunked transfer encoding
    // }

    if (auto opt_content_length = get< ContentLength >(r)) {
      auto content_length = *opt_content_length;

      auto b = content.prepare(content_length);
      auto bytes_transferred =
        read_content(s, input_buffer, b, content_length, ec);

      if (ec)
        return 0;

      content.commit(content_length);

      return bytes_transferred;
    }

    auto pending = input_buffer.size();

    if (pending > 0) {
      auto b = content.prepare(pending);
      auto copied = asio::buffer_copy(b, input_buffer.data());

      input_buffer.consume(copied);
      content.commit(copied);
    }

    return asio::read(s, content, ec);
  }

  template< typename SyncReadStream,
            typename DynamicBuffer,
            typename DynamicBuffer2 >
  size_t
  read_content(SyncReadStream& s,
               DynamicBuffer& input_buffer,
               DynamicBuffer2&& content,
               response const& r,
               std::error_code& ec)
  {
    using rfc7230::ContentLength;

    // if (auto opt_transer_encoding = get< TransferEncoding >(r)) {
    //   TODO chunked transfer encoding
    // }

    if (auto opt_content_length = get< ContentLength >(r)) {
      auto content_length = *opt_content_length;

      auto b = content.prepare(content_length);
      read_content(s, input_buffer, b, content_length, ec);

      if (ec)
        return 0;

      content.commit(content_length);

      return content_length;
    }

    if (input_buffer.size() > 0) {
      auto b = content.prepare(input_buffer.size());
      auto copied = asio::buffer_copy(b, input_buffer.data());

      input_buffer.consume(copied);
      content.commit(copied);
    }

    return asio::read(s, content, ec);
  }

  template< typename AsyncReadStream,
            typename DynamicBuffer,
            typename MutableBufferSequence >
  struct async_read_content_implementation {
    AsyncReadStream& stream_;
    DynamicBuffer& buffer_;
    MutableBufferSequence target_;

    enum { starting, reading } state_{ starting };

    template< typename Self >
    void operator()(Self& self,
                    std::error_code const& ec = {},
                    std::size_t bytes_transferred = 0)
    {
      switch (state_) {
        case starting:
          std::cout << "input buffer size " << buffer_.size() << '\n';
          std::cout << "target buffer size " << asio::buffer_size(target_)
                    << '\n';

          if (buffer_.size() > 0) {
            auto copied = asio::buffer_copy(target_, buffer_.data());
            target_ = target_ + copied;
            buffer_.consume(copied);
          }

          if (asio::buffer_size(target_) < 1) {
            self.complete({}, 0);
            return;
          }

          std::cout << "not enough data, reading more\n";

          state_ = reading;
          asio::async_read(stream_, target_, std::move(self));
          return;

        case reading:
          std::cout << "read " << bytes_transferred << " data bytes\n";
          self.complete(ec, bytes_transferred);
      }
    }
  };

  template< typename AsyncReadStream,
            typename DynamicBuffer,
            typename MutableBufferSequence,
            typename CompletionToken >
  auto
  async_read_content(AsyncReadStream& s,
                     DynamicBuffer& buffer,
                     MutableBufferSequence const& target,
                     CompletionToken&& token)
  {
    return asio::async_compose< CompletionToken,
                                void(std::error_code, std::size_t) >(
      async_read_content_implementation< AsyncReadStream, DynamicBuffer,
                                         MutableBufferSequence >{ s, buffer,
                                                                  target },
      token, s);
  }

  template< typename AsyncReadStream,
            typename InputBuffer,
            typename ContentBuffer,
            typename CompletionToken >

  auto
  async_read_content2(AsyncReadStream& stream,
                      InputBuffer& input_buffer,
                      ContentBuffer& content,
                      std::size_t content_length,
                      CompletionToken&& token)
  {
    asio::async_completion< CompletionToken,
                            void(std::error_code, std::size_t) >
      init{ token };

    impl::start_async_read_content_op(stream, content_length, input_buffer,
                                      content, init.completion_handler);

    return init.result.get();
  }

  template< typename AsyncReadStream,
            typename DynamicBuffer,
            typename DynamicBuffer2,
            typename ReadHandler >
  auto
  async_read_content(AsyncReadStream& s,
                     DynamicBuffer& buffer,
                     DynamicBuffer2&& content,
                     request const& r,
                     ReadHandler&& handler)
  {
    using rfc7230::ContentLength;

    // if (auto opt_transer_encoding = get< TransferEncoding >(r)) {
    //   TODO chunked transfer encoding
    // }

    if (auto opt_content_length = get< ContentLength >(r)) {
      auto content_length = *opt_content_length;

      std::cout << "Reading " << content_length << " bytes of content data\n";

      return async_read_content2(s, buffer, content, content_length,
                                 std::move(handler));
    }

    // TODO handler Transfer-Encoding
    auto copy = [h = handler]() { h({}, 0); };
    asio::post(s.get_executor(), copy);
  }

  template< typename AsyncReadStream,
            typename DynamicBuffer,
            typename DynamicBuffer2,
            typename ReadHandler >
  void
  async_read_content(AsyncReadStream& s,
                     DynamicBuffer& buffer,
                     DynamicBuffer2&& content,
                     response const& r,
                     ReadHandler&& handler)
  {
    // TODO
    using rfc7230::ContentLength;

    // if (auto opt_transer_encoding = get< TransferEncoding >(r)) {
    //   TODO chunked transfer encoding
    // }

    if (auto opt_content_length = get< ContentLength >(r)) {
      auto content_length = *opt_content_length;
      auto b = content.prepare(content_length);
      return async_read_content(s, buffer, b, std::move(handler));
    }

    auto pending = buffer.size();

    if (pending > 0) {
      auto b = content.prepare(pending);
      auto copied = asio::buffer_copy(b, buffer.data());

      buffer.consume(copied);
      content.commit(copied);
    }

    return asio::async_read(s, content, handler);
  }

} // namespace protocol
} // namespace http
