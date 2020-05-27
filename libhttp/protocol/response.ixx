namespace http {
namespace protocol {

  inline response::response(status_code status)
    : status_{ status }, reason_{ get_rfc7231_reason(status) }
  {}

  inline status_code const&
  response::status() const
  {
    return status_;
  }

  inline void
  response::set_status(status_code status)
  {
    status_ = std::move(status);
    reason_ = get_rfc7231_reason(status);
  }

  inline void
  response::set_status(status_code status, std::string reason)
  {
    status_ = std::move(status);
    reason_ = std::move(reason);
  }

  inline std::string const&
  response::reason() const
  {
    return reason_;
  }

  inline void
  response::set_reason(std::string reason)
  {
    reason_ = move(reason);
  }

  inline void
  to_buffers(std::vector< asio::const_buffer >& buffers, response const& r)
  {
    static char const colon_space[]{ ':', ' ' };
    static char const comma_space[]{ ',', ' ' };
    static char const crlf[]{ '\r', '\n' };
    static char const digits[]{
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };
    static char const space[]{ ' ' };

    to_buffers(buffers, r.version());
    buffers.push_back(asio::buffer(space));

    auto code = r.status();

    buffers.push_back(asio::buffer(&digits[code / 100], 1));
    buffers.push_back(asio::buffer(&digits[code % 100 / 10], 1));
    buffers.push_back(asio::buffer(&digits[code % 10], 1));
    buffers.push_back(asio::buffer(space));

    buffers.push_back(asio::buffer(r.reason()));
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
  to_buffers(response const& r)
  {
    std::vector< asio::const_buffer > buffers;
    to_buffers(buffers, r);
    return buffers;
  }

} // namespace protocol
} // namespace http
