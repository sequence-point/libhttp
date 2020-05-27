namespace http {
namespace routing {

  inline bool
  match_path(std::string const& pattern,
             std::string const& subject,
             route_parameters& params)
  {
    route_parameters tmp_params;

    // p/pend = pattern iterators.
    auto p = pattern.begin();
    auto const pend = pattern.end();

    // s/send = subject iterators.
    auto s = subject.begin();
    auto const send = subject.end();

    auto match_param = [&]() {
      // k/kend = local pattern iterators.
      auto const k = p;
      while (p != pend && '}' != *p)
        ++p;
      auto const kend = p;
      ++p;

      if (k == kend)
        throw std::invalid_argument{ "empty parameter name" };

      bool const greedy{ '*' == *k && 1 == std::distance(k, kend) };

      // v/vend = local subject iterators.
      auto const v = s;
      while (s != send && (greedy || '/' != *s))
        ++s;
      auto const vend = s;

      tmp_params.emplace(std::string{ k, kend }, std::string{ v, vend });
    };

    while (p != pend && s != send) {
      if (*p == '{') {
        ++p;
        match_param();
        continue;
      }

      if (*p != *s)
        return false;

      ++p;
      ++s;
    }

    if (p != pend || s != send)
      return false;

    params = std::move(tmp_params);
    return true;
  }

  inline std::string
  render_path(std::string const& pattern, route_parameters const& params)
  {
    std::string text;

    auto p = std::begin(pattern);
    auto pend = std::end(pattern);

    while (p != pend) {
      if (*p == '{') {
        ++p;
        auto const k = p;
        while (p != pend && '}' != *p)
          ++p;
        auto const kend = p;

        text += params.at(std::string{ k, kend });
      }
      else {
        text += *p;
      }

      ++p;
    }

    return text;
  }

} // namespace routing
} // namespace http
