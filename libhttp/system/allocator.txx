namespace http {
namespace system {

  template< typename T >
  T&
  allocator::alloc()
  {
    return alloc_emplace< T >();
  }

  template< typename T >
  T&
  allocator::alloc(T const& object)
  {
    return alloc_emplace< T >(object);
  }

  template< typename T >
  T&
  allocator::alloc(T&& object)
  {
    return alloc_emplace< T >(object);
  }

  template< typename T, typename... Args >
  T&
  allocator::alloc_emplace(Args&&... args)
  {
    struct object_allocation : allocation {
      object_allocation(Args&&... args)
        : object{ std::forward< Args >(args)... }
      {}

      T object;
    };

    std::unique_ptr< object_allocation > a{ new object_allocation{
      std::forward< Args >(args)... } };

    auto& ref = a->object;

    allocations_.emplace_back(std::move(a));

    return ref;
  }

} // namespace system
} // namespace http
