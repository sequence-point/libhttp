#ifndef libhttp__protocol__rfc7230__host_hxx_
#define libhttp__protocol__rfc7230__host_hxx_

#include <liburi/grammar.hxx>

#include <string>

namespace http::protocol::rfc7230 {

struct Host {
  std::string host;
  optional< std::string > port;

  static constexpr char const name[] = "host";

  static optional< Host >
  try_parse(std::vector< std::string > const& strings, std::error_code& ec)
  {
    std::string host;
    optional< std::string > opt_port;

    if (auto it = strings.rbegin(); it != strings.rend()) {
      auto first = it->begin();
      auto last = it->end();

      auto try_parse_host = [&](auto init) {
        auto c = init;

        while (c != last && uri::grammar::is_host(*c))
          host += *c++;

        return c;
      };

      auto try_parse_port = [&](auto init) {
        auto c = init;

        if (c != last && *c == ':') {
          ++c; // skips ':'

          opt_port = std::string{};

          while (c != last && uri::grammar::is_digit(*c))
            *opt_port += *c++;

          return c;
        }

        return init;
      };

      first = try_parse_host(first);
      first = try_parse_port(first);

      if (first != last)
        return {};
    }

    return Host{ host, opt_port };
  }

  static std::string
  to_string(Host const& host)
  {
    std::string str{ host.host };

    if (host.port)
      str += ":" + *host.port;

    return str;
  }
};

} // namespace http::protocol::rfc7230

#endif
