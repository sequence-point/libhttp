#ifndef libhttp__optional_hxx_
#define libhttp__optional_hxx_

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

#if __has_include(<optional>)
#elif __has_include(<experimental/optional>)
namespace std {
using std::experimental::make_optional;
using std::experimental::nullopt;
using std::experimental::optional;
} // namespace std
#endif

#endif
