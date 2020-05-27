#ifndef libhttp__protocol__read_content_hxx_
#define libhttp__protocol__read_content_hxx_

#include <libhttp/protocol/error.hxx>
#include <libhttp/protocol/request.hxx>
#include <libhttp/protocol/response.hxx>

#include <libhttp/protocol/rfc7230/content-length.hxx>

#include <algorithm>
#include <functional>
#include <iostream> // TODO remove
#include <system_error>
#include <type_traits>

namespace http {
namespace protocol {

  template< typename SyncReadStream,
            typename DynamicBuffer,
            typename MutableBufferSequence >
  size_t
  read_content(SyncReadStream& s,
               DynamicBuffer& input_buffer,
               MutableBufferSequence const& content,
               size_t content_length);

  template< typename SyncReadStream,
            typename DynamicBuffer,
            typename MutableBufferSequence >
  size_t
  read_content(SyncReadStream& s,
               DynamicBuffer& input_buffer,
               MutableBufferSequence const& content,
               size_t content_length,
               std::error_code& ec);

  template< typename SyncReadStream,
            typename DynamicBuffer,
            typename DynamicBuffer2 >
  size_t
  read_content(SyncReadStream& s,
               DynamicBuffer& input_buffer,
               DynamicBuffer2&& content,
               request const& request);

  template< typename SyncReadStream,
            typename DynamicBuffer,
            typename DynamicBuffer2 >
  size_t
  read_content(SyncReadStream& s,
               DynamicBuffer& input_buffer,
               DynamicBuffer2&& content,
               request const& request,
               std::error_code& ec);

  template< typename SyncReadStream,
            typename DynamicBuffer,
            typename DynamicBuffer2 >
  size_t
  read_content(SyncReadStream& s,
               DynamicBuffer& input_buffer,
               DynamicBuffer2&& content,
               response const& r);

  template< typename SyncReadStream,
            typename DynamicBuffer,
            typename DynamicBuffer2 >
  size_t
  read_content(SyncReadStream& s,
               DynamicBuffer& input_buffer,
               DynamicBuffer2&& content,
               response const& r,
               std::error_code& ec);

#if 0
    template< typename SyncReadStream,
              typename DynamicBuffer,
              typename MutableBufferSequence,
              typename ReadHandler >
    void
    async_read_content(SyncReadStream& s,
                       DynamicBuffer& input_buffer,
                       MutableBufferSequence const& content,
                       size_t content_length,
                       ReadHandler&& handler);

    template< typename SyncReadStream,
              typename DynamicBuffer,
              typename DynamicBuffer2,
              typename ReadHandler >
    void
    async_read_content(SyncReadStream& s,
                       DynamicBuffer& input_buffer,
                       DynamicBuffer2&& content,
                       request const& request,
                       ReadHandler&& handler);

    template< typename SyncReadStream,
              typename DynamicBuffer,
              typename DynamicBuffer2,
              typename ReadHandler >
    void
    async_read_content(SyncReadStream& s,
                       DynamicBuffer& input_buffer,
                       DynamicBuffer2&& content,
                       response const& r,
                       ReadHandler&& handler);
#endif

} // namespace protocol
} // namespace http

#include <libhttp/protocol/impl/async-read-content-op.hxx>

#include <libhttp/protocol/read-content.txx>

#endif
