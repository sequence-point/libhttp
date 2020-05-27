#ifndef libhttp__protocol__read_response_message_hxx_
#define libhttp__protocol__read_response_message_hxx_

#include <libhttp/protocol/error.hxx>
#include <libhttp/protocol/parser.hxx>
#include <libhttp/protocol/read-message.hxx>
#include <libhttp/protocol/response.hxx>
#include <libhttp/protocol/traits.hxx>

#include <type_traits>
#include <utility>

#include <asio.hpp>

namespace http {
namespace protocol {

  template< typename SyncReadStream, typename DynamicBuffer >
  std::size_t
  read_response(SyncReadStream& s, DynamicBuffer& buffer, response& r);

  template< typename SyncReadStream, typename DynamicBuffer >
  std::size_t
  read_response(SyncReadStream& s,
                DynamicBuffer& buffer,
                response& r,
                std::error_code& ec);

  template< typename AsyncReadStream,
            typename DynamicBuffer,
            typename ReadHandler >
  ASIO_INITFN_RESULT_TYPE(ReadHandler, void(std::error_code, std::size_t))
  async_read_response(AsyncReadStream& s,
                      DynamicBuffer& buffer,
                      response& r,
                      ReadHandler&& handler);

} // namespace protocol
} // namespace http

#include <libhttp/protocol/read-response.txx>

#endif
