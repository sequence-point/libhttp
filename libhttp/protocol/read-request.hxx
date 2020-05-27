#ifndef libhttp__protocol__read_request_message_hxx_
#define libhttp__protocol__read_request_message_hxx_

#include <libhttp/protocol/error.hxx>
#include <libhttp/protocol/parser.hxx>
#include <libhttp/protocol/read-message.hxx>
#include <libhttp/protocol/request.hxx>
#include <libhttp/protocol/traits.hxx>

#include <type_traits>
#include <utility>

#include <asio.hpp>

namespace http {
namespace protocol {

  template< typename SyncReadStream, typename DynamicBuffer >
  std::size_t
  read_request(SyncReadStream& s, DynamicBuffer& buffer, request& r);

  template< typename SyncReadStream, typename DynamicBuffer >
  std::size_t
  read_request(SyncReadStream& s,
               DynamicBuffer& buffer,
               request& r,
               std::error_code& ec);

  template< typename AsyncReadStream,
            typename DynamicBuffer,
            typename ReadHandler >
  ASIO_INITFN_RESULT_TYPE(ReadHandler, void(std::error_code, std::size_t))
  async_read_request(AsyncReadStream& s,
                     DynamicBuffer& buffer,
                     request& r,
                     ReadHandler&& handler);

} // namespace protocol
} // namespace http

#include <libhttp/protocol/read-request.txx>

#endif
