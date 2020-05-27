#ifndef libhttp__system__allocator_hxx_
#define libhttp__system__allocator_hxx_

#include <memory>
#include <vector>

namespace http {
namespace system {

  class allocator {
  public:
    template< typename T >
    T&
    alloc();

    template< typename T >
    T&
    alloc(T const& object);

    template< typename T >
    T&
    alloc(T&& object);

    template< typename T, typename... Args >
    T&
    alloc_emplace(Args&&... args);

  private:
    struct allocation {
      virtual ~allocation() = default;
    };

    std::vector< std::unique_ptr< allocation > > allocations_;
  };

} // namespace system
} // namespace http

#include <libhttp/system/allocator.txx>

#endif
