namespace http {
namespace protocol {

  template< typename H >
  bool
  has(message const& m)
  {
    // FIXME also try to parse header.
    return m.headers().contains(H::name);
  }

  template< typename H >
  bool
  has_quick(message const& m)
  {
    return m.headers().contains(H::name);
  }

  template< typename H >
  optional< traits::alias_type_t< H > >
  get(message const& m)
  {
    std::error_code ignored_ec;
    return get< H >(m, ignored_ec);
  }

  template< typename H >
  optional< traits::alias_type_t< H > >
  get(message const& m, std::error_code& ec)
  {
    return H::try_parse(m.headers().get(H::name), ec);
  }

  template< typename H, typename... Args >
  void
  set(message& m, Args&&... args)
  {
    using type = traits::alias_type_t< H >;

    if constexpr (traits::has_overridden_to_string_v< H >) {
      m.set_header(H::name,
                   H::to_string(type{ std::forward< Args >(args)... }));
    }
    else {
      using std::to_string;
      m.set_header(H::name, to_string(type{ std::forward< Args >(args)... }));
    }
  }

  template< typename H, typename... Args >
  void
  set_if_not_set(message& m, Args&&... args)
  {
    if (has_quick< H >())
      return;

    set< H >(m, std::forward< Args >(args)...);
  }

} // namespace protocol
} // namespace http
