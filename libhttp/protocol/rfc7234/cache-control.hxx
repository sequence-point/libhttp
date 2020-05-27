#ifndef libhttp__protocol__rfc7234__cache_control_hxx_
#define libhttp__protocol__rfc7234__cache_control_hxx_

#include <libhttp/protocol/rfc7234/cache-parameters.hxx>

namespace http {
namespace protocol {
  namespace rfc7234 {

    struct CacheControl {
      using alias_type = cache_parameters;

      static constexpr char const name[] = "cache-control";

      static optional< cache_parameters >
      try_parse(strings const& strings)
      {
        if (auto it = strings.rbegin(); it != strings.rend()) {
          try {
            // FIXME Implement.
          }
          catch (...) {
            // ignored!
          }
        }

        return {};
      }
    };

  } // namespace rfc7234
} // namespace protocol
} // namespace http

#endif
