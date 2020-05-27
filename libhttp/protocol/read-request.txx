namespace http {
namespace protocol {

  template< typename SyncReadStream, typename DynamicBuffer >
  std::size_t
  read_request(SyncReadStream& s, DynamicBuffer& buffer, request& r)
  {
    std::error_code ec;
    auto bytes_transferred = read_request(s, buffer, r, ec);

    if (ec)
      throw std::system_error{ ec };

    return bytes_transferred;
  }

  template< typename SyncReadStream, typename DynamicBuffer >
  std::size_t
  read_request(SyncReadStream& s,
               DynamicBuffer& buffer,
               request& r,
               std::error_code& ec)
  {
    using const_buffers_type =
      typename std::decay< DynamicBuffer >::type::const_buffers_type;
    using iterator_type =
      decltype(asio::buffers_begin(std::declval< const_buffers_type >()));
    using parser_type = Basic_request_parser< iterator_type >;
    return read_message< parser_type >(s, buffer, r, ec);
  }

  template< typename AsyncReadStream,
            typename DynamicBuffer,
            typename ReadHandler >
  ASIO_INITFN_RESULT_TYPE(ReadHandler, void(std::error_code, std::size_t))
  async_read_request(AsyncReadStream& s,
                     DynamicBuffer& buffer,
                     request& r,
                     ReadHandler&& handler)
  {
    using const_buffers_type =
      typename std::decay< DynamicBuffer >::type::const_buffers_type;
    using iterator_type =
      decltype(asio::buffers_begin(std::declval< const_buffers_type >()));
    using parser_type = Basic_request_parser< iterator_type >;
    return async_read_message< parser_type >(s, buffer, r, std::move(handler));
  }

} // namespace protocol
} // namespace http
