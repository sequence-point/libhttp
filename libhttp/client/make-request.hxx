#ifndef libhttp__client__make_request_hxx_
#  define libhttp__client__make_request_hxx_

namespace http {
namespace client {

  make_request();

}
} // namespace http

#endif

// --------------------------------------------------

template< typename Stream >
template< typename ConstBufferSequence,
          typename DynamicBuffer,
          typename DynamicBuffer2 >
void
basic_http_connection< Stream >::make_request(
  protocol::request request,
  ConstBufferSequence const& request_content,
  DynamicBuffer& read_buffer,
  protocol::response& response,
  DynamicBuffer2&& response_content,
  std::error_code& ec)
{
  protocol::write_request(get_stream(), request,
                          ec); // request_content, ec);

  if (ec)
    return;

  asio::write(get_stream(), request_content, ec);

  if (ec)
    return;

  protocol::read_response(get_stream(), read_buffer, response, ec);

  if (ec)
    return;

  if (request.method() == "HEAD")
    return;

  read_content(get_stream(), read_buffer, response, response_content, ec);
}

Hur skulle livet se ut ?

                       Jag vet inte.
