namespace http {
namespace protocol {
  namespace rfc7232 {

    inline entity_tag::entity_tag(std::string tag, tag_type type)
      : tag_{ std::move(tag) }, type_{ type }
    {
      if (auto p = this->tag().find('"'); p != std::string::npos)
        throw std::invalid_argument{ "ETag may not contain \"" };
    }

    inline std::string const&
    entity_tag::tag() const
    {
      return tag_;
    }

    inline entity_tag::tag_type
    entity_tag::type() const
    {
      return type_;
    }

    inline std::string
    to_string(entity_tag const& et)
    {
      if (is_strong(et))
        return "\"" + et.tag() + "\"";

      return "W/\"" + et.tag() + '"';
    }

    inline bool
    is_strong(entity_tag const& et)
    {
      return et.type() == entity_tag::strong;
    }

    inline bool
    is_weak(entity_tag const& et)
    {
      return et.type() == entity_tag::weak;
    }

    inline bool
    operator==(entity_tag const& lhs, entity_tag const& rhs)
    {
      return strong_compare(lhs, rhs);
    }

    inline bool
    operator!=(entity_tag const& lhs, entity_tag const& rhs)
    {
      return !(lhs == rhs);
    }

    inline bool
    strong_compare(entity_tag const& lhs, entity_tag const& rhs)
    {
      if (is_weak(lhs) || is_weak(rhs))
        return false;

      return lhs.tag() == rhs.tag();
    }

    inline bool
    weak_compare(entity_tag const& lhs, entity_tag const& rhs)
    {
      return lhs.tag() == rhs.tag();
    }

  } // namespace rfc7232
} // namespace protocol
} // namespace http
