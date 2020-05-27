#ifndef libhttp__resources__utility_hxx_
#define libhttp__resources__utility_hxx_

#include <functional>

namespace http {
namespace resources {
  namespace utility {

    template< typename T >
    struct deref_helper {
      using type = T;

      static T const&
      get(T const& value)
      {
        return value;
      }
    };

    template< typename T >
    struct deref_helper< std::reference_wrapper< T > > {
      using type = T;

      static T const&
      get(T const& value)
      {
        return value;
      }
    };

    template< typename T >
    typename deref_helper< T >::type
    deref(T& value)
    {
      return deref_helper< T >::get(value);
    }

    template< typename T >
    typename deref_helper< T >::type
    deref(T const& value)
    {
      return deref_helper< T >::get(value);
    }

  } // namespace utility
} // namespace resources
} // namespace http

#endif
