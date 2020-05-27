#ifndef libhttp__protocol__read_with_content_hxx_
#define libhttp__protocol__read_with_content_hxx_

#include <libhttp/protocol/request.hxx>
#include <libhttp/protocol/response.hxx>

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
                                  CompletionToken&& token);

  template< typename AsyncReadStream,
            typename InputBuffer,
            typename ContentBuffer,
            typename CompletionToken >
  auto
  async_read_response_with_content(AsyncReadStream& stream,
                                   InputBuffer& input_buffer,
                                   request& r,
                                   ContentBuffer& content_buffer,
                                   CompletionToken&& token);

} // namespace protocol
} // namespace http

#include <libhttp/protocol/impl/async-read-with-content-impl.hxx>
#include <libhttp/protocol/read-with-content.txx>

#endif
