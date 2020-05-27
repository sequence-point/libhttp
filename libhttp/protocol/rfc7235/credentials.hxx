#ifndef libhttp__protocol__rfc7235__credentials_hxx_
#define libhttp__protocol__rfc7235__credentials_hxx_

#include <libhttp/protocol/rfc7235/credentials.hxx>

namespace http {
namespace protocol {
  namespace rfc7235 {

    class credentials {
    public:
      credentials(string scheme);

      credentials(string scheme, string param);

      credentials(string scheme, string_map params);

      string const&
      scheme() const;

      string const&
      param() const;

      string_map const&
      params() const;

    private:
      string scheme_;
      string param_;
      string_map params_;
    };

    string
    to_string(credentials const& the_credentials);

  } // namespace rfc7235
} // namespace protocol
} // namespace http

#include <libhttp/protocol/rfc7235/credentials.ixx>

#endif
