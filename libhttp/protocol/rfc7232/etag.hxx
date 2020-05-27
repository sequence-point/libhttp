#ifndef libhttp__protocol__rfc7232__etag_hxx_
#define libhttp__protocol__rfc7232__etag_hxx_

#include <libhttp/protocol/rfc7232/entity-tag.hxx>

namespace http {
namespace protocol {
  namespace rfc7232 {

    struct ETag {
      using alias_type = entity_tag;

      static constexpr char const name[] = "etag";
    };

  } // namespace rfc7232
} // namespace protocol
} // namespace http

#endif
