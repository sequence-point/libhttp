namespace http {
namespace resources {

  template< typename R >
  std::vector< std::string >
  resource_traits< R >::allowed_methods()
  {
    std::vector< std::string > methods;

    if constexpr (can_get) {
      methods.push_back("GET");
      methods.push_back("HEAD");
    }

    if constexpr (can_update)
      methods.push_back("PUT");

    if constexpr (can_create)
      methods.push_back("POST");

    if constexpr (can_erase)
      methods.push_back("DELETE");

    return methods;
  }

} // namespace resources
} // namespace http
