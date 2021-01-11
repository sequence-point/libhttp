namespace http::routing {

inline bool
match_path(std::string const& pattern,
           std::string const& subject,
           route_parameters& params)
{
  // TODO normalize paths so that requests for /foo and /foo/ both match /foo.

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

    // A greedy parameter will eat the rest of the subject.
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
  std::stringstream str;

  auto p = pattern.begin();
  auto pend = pattern.end();

  while (p != pend) {
    if (*p == '{') {
      ++p;
      auto const k = p;
      while (p != pend && '}' != *p)
        ++p;
      auto const kend = p;

      str << params.at(std::string{ k, kend });
    }
    else {
      str << *p;
    }

    ++p;
  }

  return str.str();
}

} // namespace http::routing
