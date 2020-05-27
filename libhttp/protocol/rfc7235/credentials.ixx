namespace http {
namespace protocol {
  namespace rfc7235 {

    inline credentials::credentials(string scheme)
      : scheme_{ std::move(scheme) }
    {}

    inline credentials::credentials(string scheme, string param)
      : scheme_{ std::move(scheme) }, param_{ std::move(param) }
    {}

    inline credentials::credentials(string scheme, string_map params)
      : scheme_{ std::move(scheme) }, params_{ std::move(params) }
    {}

    inline string const&
    credentials::scheme() const
    {
      return scheme_;
    }

    inline string const&
    credentials::param() const
    {
      return param_;
    }

    inline string_map const&
    credentials::params() const
    {
      return params_;
    }

    inline string
    to_string(credentials const& the_credentials)
    {
      string text{ scheme() };

      if (!param().empty()) {
        text += ' ';
        text += param();
        return text;
      }

      auto it = params().begin();

      if (it != params().end()) {
        text += ' ';
        text += it->first;
        text += '=';
        text += it->second;
        ++it;
      }

      while (it != params().end()) {
        text += ',';
        text += ' ';
        text += it->first;
        text += '=';
        text += it->second;
        ++it;
      }

      return text;
    }

  } // namespace rfc7235
} // namespace protocol
} // namespace http
