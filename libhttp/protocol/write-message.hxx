#ifndef libhttp__protocol__write_message_hxx_
#define libhttp__protocol__write_message_hxx_

#include <libhttp/protocol/response.hxx>

namespace http {
namespace protocol {

  template< typename SyncWriteStream >
  size_t
  write_request(SyncWriteStream& s, request const& r);

  template< typename SyncWriteStream >
  size_t
  write_request(SyncWriteStream& s, request const& r, std::error_code& ec);

  template< typename AsyncWriteStream, typename WriteHandler >
  ASIO_INITFN_RESULT_TYPE(WriteHandler, void(std::error_code))
  async_write_request(AsyncWriteStream& s,
                      request const& r,
                      WriteHandler&& handler);

  template< typename SyncWriteStream >
  size_t
  write_response(SyncWriteStream& s, response const& r);

  template< typename SyncWriteStream >
  size_t
  write_response(SyncWriteStream& s, response const& r, std::error_code& ec);

  template< typename AsyncWriteStream, typename WriteHandler >
  ASIO_INITFN_RESULT_TYPE(WriteHandler, void(std::error_code))
  async_write_response(AsyncWriteStream& s,
                       response const& r,
                       WriteHandler&& handler);

  // todo error code overload

} // namespace protocol
} // namespace http

#include <libhttp/protocol/write-message.txx>

#endif
