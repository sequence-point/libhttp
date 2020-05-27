namespace http {
namespace protocol {

  template< typename SyncWriteStream >
  size_t
  write_request(SyncWriteStream& s, request const& r)
  {
    return asio::write(s, to_buffers(r));
  }

  template< typename SyncWriteStream >
  size_t
  write_request(SyncWriteStream& s, request const& r, std::error_code& ec)
  {
    return asio::write(s, to_buffers(r), ec);
  }

  template< typename AsyncReadStream, typename WriteHandler >
  ASIO_INITFN_RESULT_TYPE(WriteHandler, void(std::error_code))
  async_write_request(AsyncReadStream& s,
                      request const& r,
                      WriteHandler&& handler)
  {
    auto on_write = [handler](std::error_code ec, std::size_t) { handler(ec); };

    return asio::async_write(s, to_buffers(r), on_write);
  }

  template< typename SyncWriteStream >
  size_t
  write_response(SyncWriteStream& s, response const& r)
  {
    return asio::write(s, to_buffers(r));
  }

  template< typename SyncWriteStream >
  size_t
  write_response(SyncWriteStream& s, response const& r, std::error_code& ec)
  {
    return asio::write(s, to_buffers(r), ec);
  }

  template< typename AsyncReadStream, typename WriteHandler >
  ASIO_INITFN_RESULT_TYPE(WriteHandler, void(std::error_code))
  async_write_response(AsyncReadStream& s,
                       response const& r,
                       WriteHandler&& handler)
  {
    auto on_write = [handler](std::error_code ec, std::size_t) { handler(ec); };

    return asio::async_write(s, to_buffers(r), on_write);
  }

} // namespace protocol
} // namespace http
