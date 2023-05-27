#ifndef GCT_TYPE_TRAITS_HAS_INSERT_HPP
#define GCT_TYPE_TRAITS_HAS_INSERT_HPP

#include <type_traits>
#include <gct/voider.hpp>

namespace gct::type_traits {

template< typename T, typename U, typename Enable = void >
struct has_insert : public std::false_type {};
template< typename T, typename U >
struct has_insert< T, U, void_t< decltype( std::declval< T& >().insert( std::declval< U >() ) ) > >
  : public std::true_type {};
template< typename T, typename U = typename T::value_type >
constexpr bool has_insert_v = has_insert< T, U >::value;

}

#endif


