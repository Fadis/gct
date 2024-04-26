#ifndef GCT_TYPE_TRAITS_IS_COMPARABLE_HPP
#define GCT_TYPE_TRAITS_IS_COMPARABLE_HPP

#include <type_traits>

namespace gct::type_traits {

template< typename T, typename U, typename Enable = void >
struct is_equality_comparable : public std::false_type {};
template< typename T, typename U >
struct is_equality_comparable<
  T, U,
  std::enable_if_t<
    std::is_same_v< decltype( std::declval< T >() == std::declval< U >() ), bool > &&
    std::is_same_v< decltype( std::declval< T >() != std::declval< U >() ), bool >
  >
> : public std::true_type {};
template< typename T, typename U >
constexpr bool is_equality_comparable_v = is_equality_comparable< T, U >::value;

}

#endif


