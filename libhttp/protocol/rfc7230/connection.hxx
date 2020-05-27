#ifndef libhttp__protocol__rfc7230__connection_hxx_
#define libhttp__protocol__rfc7230__connection_hxx_

#include <libhttp/protocol/token.hxx>

#include <initializer_list>
#include <set>
#include <string>

namespace http {
namespace protocol {
  namespace rfc7230 {

    struct Connection {
      static constexpr char const name[] = "connection";

      std::set< std::string > tokens;

      Connection(std::set< std::string > tokens) : tokens{ std::move(tokens) }
      {}

      Connection(std::initializer_list< char const* > tokens)
        : tokens{ tokens.begin(), tokens.end() }
      {}

      bool
      close() const
      {
        return tokens.find("close") != tokens.end();
      }

      bool
      keep_alive() const
      {
        return tokens.find("keep-alive") != tokens.end();
      }

      static optional< Connection >
      try_parse(std::vector< std::string > const& str)
      {
        std::vector< std::string > tokens;

        for (auto const& s : str) {
          auto begin = s.begin();
          auto end = s.end();
          auto t = try_parse_tokens(begin, end);

          if (begin != end)
            return {};

          for (auto const& j : t)
            tokens.push_back(j);
        }

        return { std::set< std::string >{ tokens.begin(), tokens.end() } };
      }
    };

    inline std::string
    to_string(Connection const& connection)
    {
      std::string joined;

      for (auto const& token : connection.tokens) {
        if (!joined.empty())
          joined.append(", ");

        joined.append(token);
      }

      return joined;
    }

  } // namespace rfc7230
} // namespace protocol
} // namespace http

#endif
