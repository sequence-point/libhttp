namespace http {
namespace client {

  template< typename S >
  template< typename RequestContent, typename ResponseContent >
  exchange< S >::exchange(stream_type stream,
                          protocol::request request,
                          RequestContent request_content,
                          ResponseContent response_content)
    : stream_{ std::move(stream) },
      request_{ std::move(request) },
      request_content_{ std::move(request_content) },
      response_content_{ std::move(response_content) }
  {}

  template< typename S >
  void
  exchange< S >::run()
  {
    std::error_code ec;

    run(ec);

    if (ec)
      throw ""; // todo change exception
  }

  template< typename S >
  void
  exchange< S >::run(std::error_code& ec)
  {
    // Write request
    // Check error
    // Write request content
    // Check error
    // Read response
    // Check error
    // Read response content
    // Check error
    protocol::write_request(s, request, ec); //, request_content, ec);

    if (ec)
      return 0;

    asio::write(s, request_content);

    if (ec)
      return 0;

    protocol::read_response(s, read_buffer, response, ec);

    if (ec)
      return 0;

    if (request.method() == "HEAD")
      return 0;

    return read_content(s, read_buffer, response, response_content, ec);
  }

  template< typename S >
  template< typename CompletionHandler >
  void
  exchange< S >::run_async(CompletionHandler handler)
  {
    protocol::
  }

  template< typename SyncReadStream,
            typename ConstBufferSequence,
            typename DynamicBuffer >
  size_t
  client_exchange(SyncReadStream& s,
                  protocol::request request,
                  DynamicBuffer& read_buffer,
                  ConstBufferSequence const& request_content,
                  protocol::response& response,
                  DynamicBuffer& response_content,
                  error_code& ec)
  {}

} // namespace client
} // namespace http
