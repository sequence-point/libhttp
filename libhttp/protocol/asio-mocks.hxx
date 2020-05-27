#ifndef asio_mocks_hxx_
#define asio_mocks_hxx_

#include <cstddef>
#include <system_error>

#include <asio.hpp>

namespace asio_mocks {

template< typename ConstBufferSequence >
struct sync_read_stream {
  ConstBufferSequence data;

  template< typename MutableBufferSequence >
  std::size_t
  read_some(MutableBufferSequence const& buffers)
  {
    return asio::buffer_copy(buffers, data);
  }

  template< typename MutableBufferSequence >
  std::size_t
  read_some(MutableBufferSequence const& buffers, std::error_code&)
  {
    return asio::buffer_copy(buffers, data);
  }
};

template< typename ConstBufferSequence >
struct sync_read_stream_once {
  ConstBufferSequence data;
  bool done{ false };

  template< typename MutableBufferSequence >
  std::size_t
  read_some(MutableBufferSequence const& buffers)
  {
    if (done)
      return 0;
    return asio::buffer_copy(buffers, data);
  }

  template< typename MutableBufferSequence >
  std::size_t
  read_some(MutableBufferSequence const& buffers, std::error_code&)
  {
    if (done)
      return 0;
    return asio::buffer_copy(buffers, data);
  }
};

template< typename ConstBufferSequence >
sync_read_stream_once< ConstBufferSequence >
make_sync_read_stream_once(ConstBufferSequence&& data)
{
  return sync_read_stream_once< ConstBufferSequence >{ data, false };
}

struct eof_sync_read_stream {
  template< typename MutableBufferSequence >
  std::size_t
  read_some(MutableBufferSequence const&)
  {
    return 0;
  }

  template< typename MutableBufferSequence >
  std::size_t
  read_some(MutableBufferSequence const&, std::error_code&)
  {
    return 0;
  }
};

} // namespace asio_mocks

#endif
