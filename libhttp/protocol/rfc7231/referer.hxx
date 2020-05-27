#ifndef libhttp__protocol__rfc7231__referer_hxx_
#define libhttp__protocol__rfc7231__referer_hxx_

namespace http {
namespace protocol {
  namespace rfc7231 {

    struct Referer {
      using alias_type = string;

      static constexpr char const name[] = "referer";

      static optional< Server >
      try_parse(strings const& str)
      {
        if (auto it = str.rbegin(); it != str.rend())
          return *it;

        return {};
      }
    };

  } // namespace rfc7231
} // namespace protocol
} // namespace http

#endif
