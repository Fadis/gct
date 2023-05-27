#ifndef GCT_TYPE_TRAITS_HAS_PUSH_BACK_HPP
#define GCT_TYPE_TRAITS_HAS_PUSH_BACK_HPP

#include <type_traits>
#include <gct/voider.hpp>

namespace gct::type_traits {

template< typename T, typename U, typename Enable = void >
struct has_push_back : public std::false_type {};
template< typename T, typename U >
struct has_push_back< T, U, void_t< decltype( std::declval< T& >().push_back( std::declval< U >() ) ) > >
  : public std::true_type {};
template< typename T, typename U = typename T::value_type >
constexpr bool has_push_back_v = has_push_back< T, U >::value;

}

#endif

