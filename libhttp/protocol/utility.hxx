#ifndef libhttp__protocol__utility_hxx_
#define libhttp__protocol__utility_hxx_

#include <libhttp/protocol/grammar.hxx>

#include <locale>
#include <string>

namespace http {
namespace protocol {

  template< typename Iterator >
  std::tuple< Iterator, Iterator >
  trim_whitespace(Iterator first, Iterator last);

  // Performs an in-place normalization of a header name.
  void
  normalize_header_name(std::string& name);

} // namespace protocol
} // namespace http

#include <libhttp/protocol/utility.ixx>
#include <libhttp/protocol/utility.txx>

#endif
