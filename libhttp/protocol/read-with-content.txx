namespace http {
namespace protocol {

  template< typename AsyncReadStream,
            typename InputBuffer,
            typename ContentBuffer,
            typename CompletionToken >
  auto
  async_read_request_with_content(AsyncReadStream& stream,
                                  InputBuffer& input_buffer,
                                  request& r,
                                  ContentBuffer& content_buffer,
                                  CompletionToken&& token)
  {
    asio::async_completion< CompletionToken,
                            void(std::error_code, std::size_t) >
      init{ token };

    impl::start_async_read_request_with_content_op(
      stream, input_buffer, r, content_buffer, init.completion_handler);

    return init.result.get();
  }

  template< typename AsyncReadStream,
            typename InputBuffer,
            typename ContentBuffer,
            typename CompletionToken >
  auto
  async_read_response_with_content(AsyncReadStream& stream,
                                   InputBuffer& input_buffer,
                                   response& r,
                                   ContentBuffer& content_buffer,
                                   CompletionToken&& token)
  {
    asio::async_completion< CompletionToken,
                            void(std::error_code, std::size_t) >
      init{ token };

    impl::start_async_read_response_with_content_op(
      stream, input_buffer, r, content_buffer, init.completion_handler);

    return init.result.get();
  }

} // namespace protocol
} // namespace http
