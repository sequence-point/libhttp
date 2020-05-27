#ifndef libhttp__protocol__rfc7230__content_length_hxx_
#define libhttp__protocol__rfc7230__content_length_hxx_

#include <libhttp/protocol/error.hxx>

#include <cstddef>
#include <string>
#include <system_error>
#include <vector>

namespace http {
namespace protocol {
  namespace rfc7230 {

    struct ContentLength {
      using alias_type = std::size_t;

      static constexpr char const name[] = "content-length";

      static optional< std::size_t >
      try_parse(std::vector< std::string > const& strings, std::error_code& ec)
      {
        if (strings.size() > 1) {
          ec = protocol_error::invalid_content_length;
          return {};
        }

        if (strings.size() == 1) {
          try {
            return std::stoull(strings[0]);
          }
          catch (...) {
            ec = protocol_error::invalid_content_length;
            return {};
          }
        }

        return {};
      }
    };

  } // namespace rfc7230
} // namespace protocol
} // namespace http

#endif
