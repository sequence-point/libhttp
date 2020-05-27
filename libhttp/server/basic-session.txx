namespace http {
namespace server {

  template< typename CT >
  typename basic_session< CT >::connection_type&
  basic_session< CT >::get_connection()
  {
    return connection_;
  }

  template< typename CT >
  void
  basic_session< CT >::init_read_request()
  {
    auto self = shared_from_this();
    auto handler = [this, self](std::error_code const& ec, std::size_t) {
      on_request(request_, content_);
    };

    get_connection().async_read_request(request_, content_, handler);
  }

  template< typename CT >
  basic_session< CT >::basic_session(stream_type stream)
    : connection_{ std::move(stream) }
  {}

} // namespace server
} // namespace http
