#ifndef libhttp__resources__traits_hxx_
#define libhttp__resources__traits_hxx_

#include <libhttp/common/traits.hxx>

#include <libhttp/protocol/media-type.hxx>
#include <libhttp/protocol/rfc7232/entity-tag.hxx>

#include <chrono>
#include <string>
#include <type_traits>
#include <vector>

namespace http::resources {

namespace traits {

  struct local_void {
  };

  template< class... >
  using local_void_t = local_void;

  //
  // can_get
  //

  template< typename, typename = std::void_t<> >
  struct can_get : std::false_type {
  };

  template< typename T >
  struct can_get< T, std::void_t< decltype(&T::get) > > : std::true_type {
  };

  template< typename T >
  inline constexpr bool can_get_v{ can_get< T >::value };

  //
  // can_update
  //

  template< typename, typename = std::void_t<> >
  struct can_update : std::false_type {
  };

  template< typename T >
  struct can_update< T, std::void_t< decltype(&T::update) > > : std::true_type {
  };

  template< typename T >
  inline constexpr bool can_update_v{ can_update< T >::value };

  //
  // can_create
  //

  template< typename, typename = std::void_t<> >
  struct can_create : std::false_type {
  };

  template< typename T >
  struct can_create< T, std::void_t< decltype(&T::create) > > : std::true_type {
  };

  template< typename T >
  inline constexpr bool can_create_v{ can_create< T >::value };

  //
  // can_erase
  //

  template< typename, typename = std::void_t<> >
  struct can_erase : std::false_type {
  };

  template< typename T >
  struct can_erase< T,
                    std::void_t< decltype(std::declval< T const >().erase()) > >
    : std::true_type {
  };

  template< typename T >
  inline constexpr bool can_erase_v{ can_erase< T >::value };

  //
  // has_content_type
  //

  template< typename, typename = std::void_t<> >
  struct has_content_type : std::false_type {
  };

  template< typename T >
  struct has_content_type< T,
                           std::void_t< decltype(protocol::media_type{
                             std::declval< T const >().content_type() }) > >
    : std::true_type {
  };

  template< typename T >
  inline constexpr bool has_content_type_v{ has_content_type< T >::value };

  //
  // has_entity_tag
  //

  template< typename, typename = std::void_t<> >
  struct has_entity_tag : std::false_type {
  };

  template< typename T >
  struct has_entity_tag< T,
                         std::void_t< decltype(protocol::rfc7232::entity_tag{
                           std::declval< T const >().etag() }) > >
    : std::true_type {
  };

  template< typename T >
  inline constexpr bool has_entity_tag_v{ has_entity_tag< T >::value };

  //
  // has_location
  //

  template< typename, typename = std::void_t<> >
  struct has_location : std::false_type {
  };

  template< typename T >
  struct has_location< T,
                       std::void_t< decltype(std::string{
                         std::declval< T const >().location() }) > >
    : std::true_type {
  };

  template< typename T >
  inline constexpr bool has_location_v{ has_location< T >::value };

  //
  // model_type_for_update
  //

  template< typename T, bool can_update = can_update_v< T > >
  struct model_type_for_update {
    using type = void;
  };

  template< typename T >
  struct model_type_for_update< T, true > {
    using function_traits =
      common::traits::function_traits< decltype(&T::update) >;

    using type =
      std::tuple_element_t< 0, typename function_traits::argument_tuple >;
  };

  template< typename T >
  using model_type_for_update_t = typename model_type_for_update< T >::type;

  //
  // model_type_for_create
  //

  template< typename T, bool can_create = can_create_v< T > >
  struct model_type_for_create {
    using type = void;
  };

  template< typename T >
  struct model_type_for_create< T, true > {
    using function_traits =
      common::traits::function_traits< decltype(&T::create) >;

    using type =
      std::tuple_element_t< 0, typename function_traits::argument_tuple >;
  };

  template< typename T >
  using model_type_for_create_t = typename model_type_for_create< T >::type;

} // namespace traits

template< typename T >
struct resource_traits {
  using resource_type = T;

  using model_type_for_update = traits::model_type_for_update_t< T >;
  using model_type_for_create = traits::model_type_for_create_t< T >;

  template< typename U >
  struct rebind {
    using other = resource_traits< U >;
  };

  static constexpr bool has_location{ traits::has_location_v< resource_type > };

  static constexpr bool has_entity_tag{
    common::traits::has_entity_tag_v< resource_type >
  };

  static constexpr bool has_last_modified{
    common::traits::has_last_modified_v< resource_type >
  };

  // can_get implies HEAD-support
  static constexpr bool can_get{ traits::can_get_v< resource_type > };
  static constexpr bool can_update{ traits::can_update_v< resource_type > };
  static constexpr bool can_create{ traits::can_create_v< resource_type > };
  static constexpr bool can_erase{ traits::can_erase_v< resource_type > };

  static std::vector< std::string >
  allowed_methods();
};

template< typename T >
using create_input_type = typename T::input_type;

} // namespace http::resources

#include <libhttp/resources/traits.txx>

#endif
