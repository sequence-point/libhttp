namespace http {
namespace protocol {

  template< typename InputIterator >
  optional< media_type >
  media_type::try_parse(InputIterator& begin, InputIterator const& end)
  {
    optional< media_type > const failure;

    auto skip_whitespace = [&] {
      while (begin != end && grammar::is_space(*begin))
        ++begin;
    };

    auto try_parse_token = [&]() -> optional< std::string > {
      if (begin == end)
        return {};

      if (!grammar::is_tchar(*begin))
        return {};

      std::string token;
      token += *begin;
      ++begin;

      while (begin != end) {
        if (!grammar::is_tchar(*begin))
          break;

        token += *begin;
        ++begin;
      }

      return std::move(token);
    };

    auto try_parse_parameter =
      [&]() -> optional< std::pair< std::string, std::string > > {
      if (begin == end || *begin != ';')
        return {};

      ++begin; // consume ';'

      skip_whitespace();

      auto name = try_parse_token();

      if (!name || begin == end || *begin != '=')
        return {};

      ++begin; // consume '='

      std::cout << "got parameter name: " << *name << '\n';

      // Quoted-string value?
      if (begin != end && *begin == '"') {
        // TODO support quoted string values
        return {};
      }

      auto value = try_parse_token();

      if (!value)
        return {};

      // TODO normalize parameter name

      std::pair< std::string, std::string > param{ *name, *value };
      return param;
    };

    auto try_parse_parameters =
      [&]() -> optional< std::map< std::string, std::string > > {
      // If we're at the end we don't have any parameter,
      // so we return an empty map.
      if (begin == end)
        return std::map< std::string, std::string >{};

      std::map< std::string, std::string > params;

      while (begin != end) {
        skip_whitespace();

        if (begin == end)
          return std::map< std::string, std::string >{};

        auto param = try_parse_parameter();

        if (!param)
          return {};

        // TODO ensure parameter not already added

        params.insert(*param);
      }

      return params;
    };

    auto type = try_parse_token();

    if (!type)
      return failure;

    if (begin == end || *begin != '/')
      return failure;

    ++begin; // consume '/'

    auto subtype = try_parse_token();

    if (!subtype)
      return failure;

    auto params = try_parse_parameters();

    if (!params)
      return failure;

    return media_type{ std::move(*type), std::move(*subtype),
                       std::move(*params) };
  }

} // namespace protocol
} // namespace http
