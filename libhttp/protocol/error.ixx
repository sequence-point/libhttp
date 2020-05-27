namespace http {
namespace protocol {

  inline std::error_category const&
  get_parse_error_category()
  {
    class error_category : public std::error_category {
    public:
      char const*
      name() const noexcept override
      {
        return "libhttp.parser";
      }

      std::string
      message(int ec) const override
      {
        switch (static_cast< parse_error >(ec)) {
          case parse_error::incomplete_message: return "incomplete message";
          case parse_error::bad_version: return "bad version";
          case parse_error::bad_header_field: return "bad header field";
          case parse_error::bad_header_value: return "bad header value";
          case parse_error::bad_terminator: return "bad terminator";
          case parse_error::bad_method: return "bad method";
          case parse_error::bad_target: return "bad target";
          case parse_error::bad_status: return "bad status";
          case parse_error::bad_reason: return "bad reason";
        }

        return "libhttp.parser error";
      }
    };

    static error_category const category;

    return category;
  }

  inline std::error_code
  make_error_code(parse_error error)
  {
    return { static_cast< int >(error), get_parse_error_category() };
  }

  inline std::error_category const&
  get_protocol_error_category()
  {
    class error_category : public std::error_category {
    public:
      char const*
      name() const noexcept override
      {
        return "libhttp.protocol";
      }

      std::string
      message(int ec) const override
      {
        switch (static_cast< protocol_error >(ec)) {
          case protocol_error::invalid_content_length:
            return "invalid content length";
          case protocol_error::request_too_large: return "request too large";
        }

        return "libhttp.protocol error";
      }
    };

    static error_category const category;
    return category;
  }

  inline std::error_code
  make_error_code(protocol_error error)
  {
    return { static_cast< int >(error), get_protocol_error_category() };
  }

} // namespace protocol
} // namespace http
