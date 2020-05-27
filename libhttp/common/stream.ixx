#include <libhttp/common/stream.hxx>

namespace http {
namespace common {

  inline asio::executor
  stream::get_executor()
  {
    return stream_->get_executor();
  }

  inline std::size_t
  read_some(asio::mutable_buffer const& buffer)
  {
    return stream_->read_some(buffer);
  }

  inline std::size_t
  read_some(asio::mutable_buffer const& buffer, std::error_code& ec)
  {
    return stream_->read_some(buffer, ec);
  }

  inline void
  stream::async_read_some(asio::mutable_buffer const& buffer,
                          read_handler handler)
  {
    return stream_->async_read_some(buffer, handler);
  }

  inline std::size_t
  stream::write_some(asio::const_buffer const& buffer)
  {
    return stream_->write_some(buffer);
  }

  inline std::size_t
  stream::write_some(asio::const_buffer const& buffer, std::error_code& ec)
  {
    return stream_->write_some(buffer, ec);
  }

  inline void
  stream::async_write_some(asio::const_buffer const& buffer,
                           write_handler handler)
  {
    return stream_->async_write_some(buffer, handler);
  }

} // namespace common
} // namespace http
