namespace http {
namespace protocol {

  inline status_code::status_code(unsigned short code) : code_{ code }
  {
    if (code < 100 || 999 < code)
      throw std::invalid_argument{ "invalid status code" };
  }

  inline unsigned short
  status_code::code() const
  {
    return code_;
  }

  inline status_code::operator unsigned short()
  {
    return code_;
  }

  inline bool
  operator==(status_code const& lhs, status_code const& rhs)
  {
    return lhs.code() == rhs.code();
  }

  inline bool
  operator==(status_code const& lhs, unsigned short rhs)
  {
    return lhs.code() == rhs;
  }

  inline bool
  operator==(unsigned short lhs, status_code const& rhs)
  {
    return lhs == rhs.code();
  }

  inline bool
  operator!=(status_code const& lhs, status_code const& rhs)
  {
    return !(lhs == rhs);
  }

  inline bool
  operator!=(status_code const& lhs, unsigned short rhs)
  {
    return !(lhs == rhs);
  }

  inline bool
  operator!=(unsigned short lhs, status_code const& rhs)
  {
    return !(lhs == rhs);
  }

  inline bool
  is_informational(status_code const& sc)
  {
    return (sc.code() / 100) == 1;
  }

  inline bool
  is_success(status_code const& sc)
  {
    return (sc.code() / 100) == 2;
  }

  inline bool
  is_redirection(status_code const& sc)
  {
    return (sc.code() / 100) == 3;
  }

  inline bool
  is_client_error(status_code const& sc)
  {
    return (sc.code() / 100) == 4;
  }

  inline bool
  is_server_error(status_code const& sc)
  {
    return (sc.code() / 100) == 5;
  }

} // namespace protocol
} // namespace http
