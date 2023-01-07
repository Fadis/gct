#ifndef GCT_TUPLISE_HPP
#define GCT_TUPLISE_HPP

#include <tuple>
#include <type_traits>
#include <gct/type_traits/is_lifted.hpp>

namespace gct {

  template< typename T >
  auto tuplise( T &&v ) -> std::enable_if_t< gct::type_traits::is_lifted_by_v< T, std::tuple >, T > {
    return std::forward< T >( v );
  }
  template< typename T >
  auto tuplise( T &&v ) -> std::enable_if_t< !gct::type_traits::is_lifted_by_v< T, std::tuple >, std::tuple< T > > {
    return std::make_tuple( std::forward< T >( v ) );
  }

}

#endif

