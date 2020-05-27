#ifndef libhttp__client__exchange_hxx_
#define libhttp__client__exchange_hxx_

#include <libhttp/protocol/read-message.hxx>
#include <libhttp/protocol/write-message.hxx>

namespace http::client {

template< typename Stream >
class exchange {
public:
  using stream_type = Stream;

  template< typename typename RequestContent, typename ResponseContent >
  exchange(stream_type stream,
           protocol::request request,
           RequestContent request_content,
           ResponseContent response_content);

  void
  run();

  void
  run(std::error_code& ec);

  template< typename CompletionHandler >
  run_async(CompletionHandler handler);

  void
  resume();

private:
  RequestContent request_content_;
  ResponseContent response_content_;
};

template< typename Stream >
class async_exchange : public resumable {
public:
  async_exchange(read_buffer request request_content response_content

  );

  void
  resume() override;

private:
};

#if 0

    template< typename SyncReadStream,
              typename ConstBufferSequence,
              typename DynamicBuffer,
              typename MutableBufferSequence >
    size_t
    client_exchange(SyncReadStream& s,
                    protocol::request request,
                    ConstBufferSequence const& request_content,
                    DynamicBuffer& buffer,
                    protocol::response& response,
                    MutableBufferSequence const& response_content,
                    error_code& ec);

    template< typename SyncReadStream,
              typename ConstBufferSequence,
              typename DynamicBuffer,
              typename MutableBufferSequence >
    size_t
    client_exchange(SyncReadStream& s,
                    protocol::request request,
                    DynamicBuffer& buffer,
                    protocol::response& response,
                    MutableBufferSequence const& response_content,
                    error_code& ec);
#endif

} // namespace http::client

#include <libhttp/client/client.txx>

#endif
