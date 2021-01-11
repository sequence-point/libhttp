#ifndef libhttp__protocol__rc7232__last_modified_hxx_
#define libhttp__protocol__rc7232__last_modified_hxx_

#include <chrono>
#include <string>
#include <system_error>
#include <vector>

namespace http {
namespace protocol {
  namespace rfc7232 {

    struct LastModified {
      std::chrono::system_clock::time_point time_value;

      static constexpr const char name[] = "last-modified";
      static constexpr const char time_format[] = "%a, %d %b %Y %H:%M:%S GMT";

      static optional< LastModified >
      try_parse(std::vector< std::string > const& strings, std::error_code& ec)
      {
        // todo
        return {};
      }
    };

    inline std::string
    to_string(LastModified const& last_modified)
    {
      return {};
    }

  } // namespace rfc7232
} // namespace protocol
} // namespace http

#endif
