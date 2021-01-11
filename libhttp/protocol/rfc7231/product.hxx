#ifndef libhttp__protocol__rfc7231__product_hxx_
#define libhttp__protocol__rfc7231__product_hxx_

#include <libhttp/protocol/optional.hxx>
#include <libhttp/protocol/token.hxx>

#include <string>
#include <vector>

namespace http::protocol::rfc7231 {

struct product {
  product() = default;

  explicit product(std::string name) : name{ std::move(name) }
  {}

  product(std::string name, std::string version)
    : name{ std::move(name) }, version{ std::move(version) }
  {}

  std::string name;
  std::string version;
};

using products = std::vector< product >;

inline void
to_stream(std::ostream& o, product const& p)
{
  if (p.name.empty())
    return;

  o << p.name;

  if (!p.version.empty())
    o << '/' << p.version;
}

inline std::string
to_string(product const& p)
{
  std::ostringstream str_stream;
  to_stream(str_stream, p);
  return str_stream.str();
}

inline void
to_stream(std::ostream& o, products const& ps)
{
  for (auto const& j : ps) {
    to_stream(o, j);
    o << ' ';
  }
}

inline std::string
to_string(products const& ps)
{
  std::ostringstream str_stream;
  to_stream(str_stream, ps);
  return str_stream.str();
}

template< typename InputIterator >
optional< product >
try_parse_product(InputIterator& first, InputIterator last)
{
  product p;

  p.name = try_parse_token(first, last);

  if (first != last && *first == '/') {
    ++first; // skips '/'
    p.version = try_parse_token(first, last);
  }

  return { p };
}

inline optional< product >
try_parse_product(std::string const& str)
{
  auto begin = str.begin();
  return try_parse_product(begin, str.end());
}

template< typename InputIterator >
products
try_parse_products(InputIterator& first, InputIterator last)
{
  products ps;

  while (first != last) {
    auto p = try_parse_product(first, last);

    if (p)
      ps.emplace_back(std::move(*p));

    while (first != last && grammar::is_space(*first))
      ++first; // skips whitespace
  }

  return ps;
}

inline products
try_parse_products(std::string const& str)
{
  auto begin = str.begin();
  return try_parse_products(begin, str.end());
}

} // namespace http::protocol::rfc7231

#endif
