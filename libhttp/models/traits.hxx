#ifndef libhttp__models__traits_hxx_
#define libhttp__models__traits_hxx_

#include <variant>

#include <libhttp/common/traits.hxx>

namespace http::models {

template< typename T >
struct model_traits {
  using model_type = T;

  static constexpr bool has_content_type{
    common::traits::has_content_type_v< model_type >
  };

  static constexpr bool has_entity_tag{
    common::traits::has_entity_tag_v< model_type >
  };

  static constexpr bool has_last_modified{
    common::traits::has_last_modified_v< model_type >
  };

  static model_type
  read_from(std::istream& i)
  {
    return model_type::read_from(i);
  }
};

template< typename T, typename... >
struct first {
  using type = T;
};

template< typename... T >
struct model_traits< std::variant< T... > > {
  using model_type = std::variant< T... >;

  static constexpr bool has_content_type{ false };

  static constexpr bool has_entity_tag{ false };

  static constexpr bool has_last_modified{ false };

  static model_type
  read_from(std::istream& i)
  {
    // TODO implement
    return { first< T... >::type::read_from(i) };
  }
};

} // namespace http::models

#endif
