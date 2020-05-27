#ifndef libhttp__protocol__entity_tag_hxx_
#define libhttp__protocol__entity_tag_hxx_

#include <stdexcept>
#include <string>

namespace http {
namespace protocol {
  namespace rfc7232 {

    class entity_tag {
    public:
      enum class tag_type { strong, weak };

      static constexpr tag_type strong = tag_type::strong;
      static constexpr tag_type weak = tag_type::weak;

      entity_tag(std::string tag, tag_type type);

      std::string const&
      tag() const;

      tag_type
      type() const;

    private:
      std::string tag_;
      tag_type type_;
    };

    std::string
    to_string(entity_tag const& et);

    bool
    is_strong(entity_tag const& et);

    bool
    is_weak(entity_tag const& et);

    bool
    operator==(entity_tag const& lhs, entity_tag const& rhs);

    bool
    operator!=(entity_tag const& lhs, entity_tag const& rhs);

    bool
    strong_compare(entity_tag const& lhs, entity_tag const& rhs);

    bool
    weak_compare(entity_tag const& lhs, entity_tag const& rhs);

  } // namespace rfc7232
} // namespace protocol
} // namespace http

#include <libhttp/protocol/rfc7232/entity-tag.ixx>

#endif
