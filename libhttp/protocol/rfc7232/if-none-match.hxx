#ifndef libhttp__protocol__rfc7232__if_none_match_hxx_
#define libhttp__protocol__rfc7232__if_none_match_hxx_

#include <libhttp/optional.hxx>

#include <libhttp/protocol/rfc7232/entity-tag.hxx>

#include <system_error>

namespace http {
namespace protocol {
  namespace rfc7232 {

    struct IfNoneMatch {
      static constexpr char const name[] = "if-none-match";

      static std::optional< IfNoneMatch >
      try_parse(std::vector< std::string > const& strings, std::error_code& ec)
      {
        // FIXME Implement.
        return {};
      }
    };

    std::string
    to_string(IfNoneMatch const& if_none_match)
    {
      // FIXME Implement.
      return {};
    }

  } // namespace rfc7232
} // namespace protocol
} // namespace http

#endif
