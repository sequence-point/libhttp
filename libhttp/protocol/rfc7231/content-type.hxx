#ifndef libhttp__protocol__rc7231__content_type_hxx_
#define libhttp__protocol__rc7231__content_type_hxx_

#include <libhttp/protocol/media-type.hxx>

namespace http::protocol::rfc7231 {

struct ContentType {
  using alias_type = media_type;

  static constexpr const char* name = "content-type";

  static optional< media_type >
  try_parse(std::vector< std::string > const& strings)
  {
    if (auto it = strings.rbegin(); it != strings.rend())
      return media_type::try_parse(*it);

    return {};
  }
};

} // namespace http::protocol::rfc7231

#endif
