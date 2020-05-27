namespace http {
namespace protocol {

  inline bool
  operator==(version const& lhs, version const& rhs) noexcept
  {
    return lhs.major == rhs.major && lhs.minor == rhs.minor;
  }

  inline bool
  operator!=(version const& lhs, version const& rhs) noexcept
  {
    return !(lhs == rhs);
  }

  inline std::ostream&
  to_stream(std::ostream& o, version const& v)
  {
    o << "HTTP/" << v.major << '.' << v.minor;
    return o;
  }

  inline std::string
  to_string(version const& v)
  {
    std::stringstream str;
    to_stream(str, v);
    return str.str();
  }

  inline std::ostream&
  operator<<(std::ostream& o, version const& v)
  {
    return to_stream(o, v);
  }

  inline void
  to_buffers(std::vector< asio::const_buffer >& buffers, version const& v)
  {
    static char constexpr http10[]{ 'H', 'T', 'T', 'P', '/', '1', '.', '0' };

    static char constexpr http11[]{ 'H', 'T', 'T', 'P', '/', '1', '.', '1' };

    static char constexpr http[]{ 'H', 'T', 'T', 'P', '/' };
    static char constexpr digits[]{ '0', '1', '2', '3', '4',
                                    '5', '6', '7', '8', '9' };
    static char constexpr dot[]{ '.' };

    if (v == http_1_0) {
      buffers.push_back(asio::buffer(http10));
      return;
    }

    if (v == http_1_1) {
      buffers.push_back(asio::buffer(http11));
      return;
    }

    buffers.push_back(asio::buffer(http));
    buffers.push_back(asio::buffer(&digits[v.major % 10], 1));
    buffers.push_back(asio::buffer(dot));
    buffers.push_back(asio::buffer(&digits[v.minor % 10], 1));
  }

} // namespace protocol
} // namespace http
