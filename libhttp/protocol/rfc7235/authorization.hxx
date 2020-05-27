#ifndef libhttp__protocol__rfc7235__authorization_hxx_
#define libhttp__protocol__rfc7235__authorization_hxx_

#include <libhttp/protocol/rfc7235/credentials.hxx>

namespace http {
namespace protocol {
  namespace rfc7235 {

    struct Authorization {
      using alias_type = credentials;

      static constexpr const char* name = "authorization";
    };

  } // namespace rfc7235
} // namespace protocol
} // namespace http

#endif
