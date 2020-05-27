#ifndef libhttp__system__extension_context_hxx_
#define libhttp__system__extension_context_hxx_

#include <libhttp/system/allocator.hxx>

#include <map>
#include <typeindex>
#include <utility>

namespace http {
namespace system {

  class extension_context {
  public:
    template< typename E >
    E&
    use();

    template< typename E >
    E const&
    use() const;

    template< typename E >
    void
    extend(E* ptr);

    template< typename E >
    void
    erase();

  private:
    void*
    use(std::type_index const& key) const;

    void
    extend(std::type_index const& key, void* ptr);

    void
    erase_extension(std::type_index const& key);

    std::map< std::type_index, void* > extensions_;
  };

  class extension_not_found {
    // FIXME implement
  };

  class duplicate_extension {
    // FIXME implement
  };

  template< typename E >
  class extend {
  public:
    using extension_type = typename std::decay< E >::type;

    extend(extension_context& target, extension_type& ext) : target_{ &target }
    {
      target.extend(&ext);
    }

    extend(extension_context& extension_context,
           extension_type&& extension) = delete;

    extend(extend const&) = delete;

    extend(extend&& other) noexcept
    {
      std::swap(target_, other.target_);
    }

    ~extend()
    {
      if (target_)
        target_->erase< extension_type >();
    }

    extend&
    operator=(extend const&) = delete;

    extend&
    operator=(extend&& other) noexcept
    {
      std::swap(target_, other.target_);
    }

  private:
    extension_context* target_{};
  };

} // namespace system
} // namespace http

#include <libhttp/system/extension-context.ixx>
#include <libhttp/system/extension-context.txx>

#endif
