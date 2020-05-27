#ifndef libhttp__protocol__rfc7232__if_modified_since_hxx_
#define libhttp__protocol__rfc7232__if_modified_since_hxx_

#include <libhttp/optional.hxx>

#include <libhttp/protocol/rfc7232/entity-tag.hxx>

#include <string>
#include <system_error>
#include <vector>

namespace http {
namespace protocol {
  namespace rfc7232 {

    struct IfModifiedSince {
      static constexpr char const name[] = "if-modified-since";

      static std::optional< IfModifiedSince >
      try_parse(std::vector< std::string > const& strings, std::error_code& ec)
      {
        // FIXME Implement.
        return {};
      }
    };

    std::string
    to_string(IfModifiedSince const& if_modified_since)
    {
      // FIXME Implement.
      return {};
    }

  } // namespace rfc7232
} // namespace protocol
} // namespace http

#endif
