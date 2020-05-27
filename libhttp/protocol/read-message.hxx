#ifndef libhttp__protocol__read_message_hxx_
#define libhttp__protocol__read_message_hxx_

#include <libhttp/protocol/error.hxx>
#include <libhttp/protocol/match-message.hxx>
#include <libhttp/protocol/traits.hxx>

#include <type_traits>
#include <utility>

#include <asio.hpp>

namespace http {
namespace protocol {

  template< typename Parser,
            typename SyncReadStream,
            typename DynamicBuffer,
            typename Message >
  std::size_t
  read_message(SyncReadStream& stream, DynamicBuffer& buffer, Message& message);

  template< typename Parser,
            typename SyncReadStream,
            typename DynamicBuffer,
            typename Message >
  std::size_t
  read_message(SyncReadStream& stream,
               DynamicBuffer& buffer,
               Message& message,
               std::error_code& ec);

  template< typename Parser,
            typename AsyncReadStream,
            typename DynamicBuffer,
            typename Message,
            typename CompletionToken >
  auto
  async_read_message(AsyncReadStream& stream,
                     DynamicBuffer& buffer,
                     Message& message,
                     CompletionToken&& token);

} // namespace protocol
} // namespace http

#include <libhttp/protocol/read-message.txx>

#endif
