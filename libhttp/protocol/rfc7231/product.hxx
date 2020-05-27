#ifndef libhttp__protocol__rfc7231__product_hxx_
#define libhttp__protocol__rfc7231__product_hxx_

#include <libhttp/protocol/optional.hxx>
#include <libhttp/protocol/token.hxx>

#include <string>
#include <vector>

namespace http {
namespace protocol {
  namespace rfc7231 {

    struct product {
      std::string product;
      std::string version;
    };

    using products = std::vector< product >;

    inline void
    to_stream(std::ostream& o, product const& the_product)
    {
      if (the_product.product.empty())
        return;

      o << the_product.product;

      if (!the_product.version.empty())
        o << '/' << the_product.version;
    }

    inline std::string
    to_string(product const& the_product)
    {
      std::ostringstream str_stream;
      to_stream(str_stream, the_product);
      return str_stream.str();
    }

    inline void
    to_stream(std::ostream& o, products const& the_products)
    {
      for (auto const& j : the_products) {
        to_stream(o, j);
        o << ' ';
      }
    }

    inline std::string
    to_string(products const& the_products)
    {
      std::ostringstream str_stream;
      to_stream(str_stream, the_products);
      return str_stream.str();
    }

    template< typename InputIterator >
    optional< product >
    try_parse_product(InputIterator& first, InputIterator last)
    {
      product the_product;

      the_product.product = try_parse_token(first, last);

      if (first != last && *first == '/') {
        ++first; // skips '/'
        the_product.version = try_parse_token(first, last);
      }

      return { the_product };
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
      products the_products;

      while (first != last) {
        auto the_product = try_parse_product(first, last);

        if (the_product)
          the_products.emplace_back(std::move(*the_product));

        while (first != last && grammar::is_space(*first))
          ++first; // skips whitespace
      }

      return the_products;
    }

    inline products
    try_parse_products(std::string const& str)
    {
      auto begin = str.begin();
      return try_parse_products(begin, str.end());
    }

  } // namespace rfc7231
} // namespace protocol
} // namespace http

#endif
