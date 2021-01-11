namespace http::common {

template< typename Stream >
basic_connection< Stream >::basic_connection(stream_type stream)
  // TODO make input_buffer_ max size configurable
  : stream_{ std::move(stream) }, input_buffer_{ 1024 * 1024 * 4 }
{}

template< typename Stream >
basic_connection< Stream >::basic_connection(basic_connection&& other)
  // TODO make input_buffer_ max size configurable
  : stream_{ std::move(other.stream_) }, input_buffer_{ 1024 * 1024 * 4 }
{
  auto bytes_copied =
    asio::buffer_copy(input_buffer_.prepare(other.input_buffer_.size()),
                      other.input_buffer_.data());
  input_buffer_.commit(bytes_copied);
}

template< typename Stream >
typename basic_connection< Stream >::stream_type&
basic_connection< Stream >::get_stream()
{
  return stream_;
}

template< typename Stream >
typename basic_connection< Stream >::stream_type const&
basic_connection< Stream >::get_stream() const
{
  return stream_;
}

template< typename Stream >
std::size_t
basic_connection< Stream >::read_request(protocol::request& r)
{
  std::error_code ec;

  auto bytes_transferred = read_request(r, ec);

  if (ec)
    throw std::system_error{ ec };

  return bytes_transferred;
}

template< typename Stream >
std::size_t
basic_connection< Stream >::read_request(protocol::request& r,
                                         std::error_code& ec)
{
  return protocol::read_request(get_stream(), input_buffer_, r, ec);
}

template< typename Stream >
template< typename CompletionToken >
auto
basic_connection< Stream >::async_read_request(protocol::request& r,
                                               CompletionToken token)
{
  return protocol::async_read_request(get_stream(), input_buffer_, r,
                                      std::move(token));
}

template< typename Stream >
std::size_t
basic_connection< Stream >::read_response(protocol::response& r)
{
  std::error_code ec;

  auto bytes_transferred = read_response(r, ec);

  if (ec)
    throw std::system_error{ ec };

  return bytes_transferred;
}

template< typename Stream >
std::size_t
basic_connection< Stream >::read_response(protocol::response& r,
                                          std::error_code& ec)
{
  return protocol::read_response(get_stream(), input_buffer_, r, ec);
}

template< typename Stream >
template< typename CompletionToken >
auto
basic_connection< Stream >::async_read_response(protocol::response& r,
                                                CompletionToken token)
{
  return protocol::async_read_response(get_stream(), input_buffer_, r,
                                       std::move(token));
}

template< typename Stream >
template< typename ContentBuffer, typename CompletionToken >
auto
basic_connection< Stream >::async_read_content(protocol::request const& r,
                                               ContentBuffer& content,
                                               CompletionToken&& token)
{
  return protocol::async_read_content(get_stream(), input_buffer_, content, r,
                                      std::move(token));
}

template< typename Stream >
template< typename ContentBuffer, typename CompletionToken >
auto
basic_connection< Stream >::async_read_content(protocol::response const& r,
                                               ContentBuffer& content,
                                               CompletionToken&& token)
{
  return protocol::async_read_content(get_stream(), input_buffer_, content, r,
                                      std::move(token));
}

template< typename Stream >
template< typename ConstBufferSequence >
std::size_t
basic_connection< Stream >::write_request(protocol::request const& r,
                                          ConstBufferSequence const& content)
{
  auto bytes_transferred = protocol::write_request(get_stream(), r);
  return bytes_transferred + asio::write(get_stream(), content);
}

template< typename Stream >
template< typename ConstBufferSequence >
std::size_t
basic_connection< Stream >::write_request(protocol::request const& r,
                                          ConstBufferSequence const& content,
                                          std::error_code& ec)
{
  auto bytes_transferred = protocol::write_request(get_stream(), r, ec);

  if (ec)
    return bytes_transferred;

  return bytes_transferred + asio::write(get_stream(), content, ec);
}

template< typename Stream >
template< typename ConstBufferSequence >
std::size_t
basic_connection< Stream >::write_response(protocol::response const& r,
                                           ConstBufferSequence const& content)
{
  auto bytes_transferred = protocol::write_response(get_stream(), r);
  return bytes_transferred + asio::write(get_stream(), content);
}

template< typename Stream >
template< typename ConstBufferSequence >
std::size_t
basic_connection< Stream >::write_response(protocol::response const& r,
                                           ConstBufferSequence const& content,
                                           std::error_code& ec)
{
  auto bytes_transferred = protocol::write_response(get_stream(), r, ec);

  if (ec)
    return bytes_transferred;

  return bytes_transferred + asio::write(get_stream(), content, ec);
}

} // namespace http::common
