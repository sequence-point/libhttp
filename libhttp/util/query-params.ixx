namespace http {
namespace util {

  std::map< std::string, std::string >
  parse_query_params(std::string const& query)
  {
    auto const query_end = query.end();

    std::map< std::string, std::string > params;

    for (auto it = query.begin(); it != query_end;) {
      std::string key;

      while (it != query_end && *it != '=')
        key += *it++;

      if (it != query_end)
        ++it; // skips '='

      std::string value;

      while (it != query_end && *it != '&')
        value += *it++;

      if (it != query_end)
        ++it; // skips '&'

      // allows empty key or empty value, but not both
      if (!key.empty() || !value.empty())
        params.emplace(std::move(key), std::move(value));
    }

    return params;
  }

} // namespace util
} // namespace http
