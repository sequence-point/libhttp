#ifndef stripes__protocol__rfc7231__server_hxx_
#define stripes__protocol__rfc7231__server_hxx_

#include <libhttp/protocol/rfc7231/product.hxx>

namespace http::protocol::rfc7231 {

struct Server {
  using alias_type = products;

  static constexpr const char* name = "server";

  static optional< products >
  try_parse(std::vector< std::string > const& strings)
  {
    if (auto it = strings.rbegin(); it != strings.rend())
      return try_parse_products(*it);

    return {};
  }
};

} // namespace http::protocol::rfc7231

#endif
