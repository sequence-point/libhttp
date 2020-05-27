namespace http {
namespace system {

  template< typename T >
  T&
  extension_context::use()
  {
    static std::type_index const key{ typeid(T) };

    return *(static_cast< T* >(use(key)));
  }

  template< typename T >
  T const&
  extension_context::use() const
  {
    static std::type_index const key{ typeid(T) };
    return *(static_cast< T* >(use(key)));
  }

  template< typename T >
  void
  extension_context::extend(T* ptr)
  {
    static std::type_index const key{ typeid(T) };
    extend(key, ptr);
  }

  template< typename T >
  void
  extension_context::erase()
  {
    static std::type_index const key{ typeid(T) };
    erase_extension(key);
  }

} // namespace system
} // namespace http
