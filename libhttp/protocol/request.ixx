namespace http {
namespace protocol {

  inline request::request(std::string method,
                          std::string target,
                          protocol::version version)
  {
    set_method(std::move(method));
    set_target(std::move(target));
    set_version(version);
  }

  inline std::string const&
  request::method() const
  {
    return method_;
  }

  inline void
  request::set_method(std::string method)
  {
    method_ = std::move(method);
  }

  inline std::string const&
  request::target() const
  {
    return target_;
  }

  inline void
  request::set_target(std::string target)
  {
    target_ = std::move(target);
  }

  inline uri::uri const&
  request::target_uri() const
  {
    return target_uri_;
  }

  inline void
  request::set_target_uri(uri::uri target_uri)
  {
    target_uri_ = std::move(target_uri);
  }

  inline void
  to_buffers(std::vector< asio::const_buffer >& buffers, request const& r)
  {
    static char const colon_space[]{ ':', ' ' };
    static char const comma_space[]{ ',', ' ' };
    static char const crlf[]{ '\r', '\n' };
    static char const digits[]{
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };
    static char const space[]{ ' ' };

    buffers.push_back(asio::buffer(r.method()));
    buffers.push_back(asio::buffer(space));
    buffers.push_back(asio::buffer(r.target()));
    buffers.push_back(asio::buffer(space));

    to_buffers(buffers, r.version());
    buffers.push_back(asio::buffer(crlf));

    for (auto const& header : r.headers()) {
      buffers.push_back(asio::buffer(header.first));
      buffers.push_back(asio::buffer(colon_space));
      buffers.push_back(asio::buffer(header.second));
      buffers.push_back(asio::buffer(crlf));
    }

    buffers.push_back(asio::buffer(crlf));
  }

  inline std::vector< asio::const_buffer >
  to_buffers(request const& r)
  {
    std::vector< asio::const_buffer > buffers;
    to_buffers(buffers, r);
    return buffers;
  }

} // namespace protocol
} // namespace http
