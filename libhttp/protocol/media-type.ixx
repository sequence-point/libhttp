namespace http {
namespace protocol {

  inline media_type::media_type(std::string type, std::string subtype)
    : type_{ std::move(type) }, subtype_{ std::move(subtype) }
  {}

  inline media_type::media_type(std::string type,
                                std::string subtype,
                                std::map< std::string, std::string > params)
    : type_{ std::move(type) },
      subtype_{ std::move(subtype) },
      params_{ std::move(params) }
  {}

  inline std::string const&
  media_type::type() const noexcept
  {
    return type_;
  }

  inline std::string const&
  media_type::subtype() const noexcept
  {
    return subtype_;
  }

  inline std::map< std::string, std::string > const&
  media_type::params() const noexcept
  {
    return params_;
  }

  inline optional< media_type >
  media_type::try_parse(std::string const& str)
  {
    auto begin = str.begin();
    return try_parse(begin, str.end());
  }

  inline bool
  operator==(media_type const& lhs, media_type const& rhs)
  {
    if (lhs.type() != rhs.type()) {
      if ("*" != lhs.type() && "*" != rhs.type()) {
        return false;
      }
    }

    if (lhs.subtype() != rhs.subtype()) {
      if ("*" != lhs.subtype() && "*" != rhs.subtype()) {
        return false;
      }
    }

    return true;
  }

  inline bool
  operator!=(media_type const& lhs, media_type const& rhs)
  {
    return !(lhs == rhs);
  }

  inline std::ostream&
  to_stream(std::ostream& o, media_type const& mt)
  {
    // TODO parameters
    return o << mt.type() << '/' << mt.subtype();
  }

  inline std::string
  to_string(media_type const& mt)
  {
    std::stringstream str;
    to_stream(str, mt);
    return str.str();
  }

  inline std::ostream&
  operator<<(std::ostream& o, media_type const& mt)
  {
    return to_stream(o, mt);
  }

} // namespace protocol
} // namespace http
