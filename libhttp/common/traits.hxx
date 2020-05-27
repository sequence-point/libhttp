#ifndef undefined__libhttp__common__traits_hxx_
#define undefined__libhttp__common__traits_hxx_

#include <libhttp/protocol/media-type.hxx>

#include <libhttp/protocol/rfc7232/entity-tag.hxx>

#include <chrono>
#include <type_traits>

namespace http::common::traits {

//
// function_traits
//

template< typename >
struct function_traits;

template< typename Ret, typename... Args >
struct function_traits< Ret(Args...) > {
  using return_type = std::decay_t< Ret >;
  using argument_tuple = std::tuple< std::decay_t< Args >... >;
};

template< typename Ret, typename Class, typename... Args >
struct function_traits< Ret (Class::*)(Args...) > {
  using return_type = std::decay_t< Ret >;
  using class_type = Class;
  using argument_tuple = std::tuple< std::decay_t< Args >... >;
};

template< typename Ret, typename Class, typename... Args >
struct function_traits< Ret (Class::*)(Args...) const > {
  using return_type = std::decay_t< Ret >;
  using class_type = Class;
  using argument_tuple = std::tuple< std::decay_t< Args >... >;
};

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
// has_last_modified
//

template< typename, typename = std::void_t<> >
struct has_last_modified : std::false_type {
};

template< typename T >
struct has_last_modified<
  T,
  std::void_t< decltype(std::chrono::system_clock::time_point{
    std::declval< T const >().last_modified() }) > > : std::true_type {
};

template< typename T >
inline constexpr bool has_last_modified_v{ has_last_modified< T >::value };

} // namespace http::common::traits

#endif
