#ifndef libhttp__protocol__optional_hxx_
#define libhttp__protocol__optional_hxx_

#if !defined(__has_include)
#  error "__has_include not supported"
#endif

#if __has_include(<optional>)
#  include <optional>
#elif __has_include(<experimental/optional>)
#  include <experimental/optional>
#else
#  error "No support for <optional>"
#endif

namespace http {
namespace protocol {

#if __has_include(<optional>)
  using std::make_optional;
  using std::nullopt;
  using std::optional;
#elif __has_include(<experimental/optional>)
  using std::experimental::make_optional;
  using std::experimental::nullopt;
  using std::experimental::optional;
#endif

} // namespace protocol
} // namespace http

#endif
