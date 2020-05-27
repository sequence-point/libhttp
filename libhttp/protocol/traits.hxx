#ifndef libhttp__protocol__traits_hxx_
#define libhttp__protocol__traits_hxx_

#include <type_traits>

namespace http {
namespace protocol {
  namespace traits {

    //
    // enable_for_dynamic_buffer_t
    //

    template< typename MutableBufferSequence >
    using enable_for_mutable_buffer_sequence_t = typename std::enable_if<
      asio::is_mutable_buffer_sequence< MutableBufferSequence >::value >::type*;

    template< typename DynamicBuffer >
    using enable_for_dynamic_buffer_t =
      typename std::enable_if< asio::is_dynamic_buffer<
        typename std::decay< DynamicBuffer >::type >::value >::type*;

    //
    // has_alias_type
    //

    template< typename, typename = std::void_t<> >
    struct has_alias_type : std::false_type {
    };

    template< typename T >
    struct has_alias_type<
      T,
      std::void_t< decltype(std::declval< typename T::alias_type >()) > >
      : std::true_type {
    };

    template< typename T >
    inline constexpr bool has_alias_type_v = has_alias_type< T >::value;

    //
    // alias_type
    //

    template< typename T, bool = has_alias_type_v< T > >
    struct alias_type {
      using type = T;
    };

    template< typename T >
    struct alias_type< T, true > {
      using type = typename T::alias_type;
    };

    template< typename T >
    using alias_type_t = typename alias_type< T >::type;

    //
    // has_overridden_to_string
    //

    template< typename, typename = std::void_t<> >
    struct has_overridden_to_string : std::false_type {
    };

    template< typename T >
    struct has_overridden_to_string< T,
                                     std::void_t< decltype(T::to_string(
                                       std::declval< alias_type_t< T > >())) > >
      : std::true_type {
    };

    template< typename T >
    inline constexpr bool has_overridden_to_string_v =
      has_overridden_to_string< T >::value;

  } // namespace traits
} // namespace protocol
} // namespace http

#endif
