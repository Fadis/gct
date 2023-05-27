#ifndef GCT_TYPE_TRAITS_IS_TUPLOID_HPP
#define GCT_TYPE_TRAITS_IS_TUPLOID_HPP

#include <tuple>
#include <type_traits>

namespace gct::type_traits {

template< typename T, typename Enable = void >
struct is_tuploid : public std::false_type {};
template< typename T >
struct is_tuploid< T, std::enable_if_t<
  std::is_lvalue_reference_v< decltype( std::get< 0u >( std::declval< T& >() ) ) > &&
  std::is_same_v<
    std::remove_cvref_t< decltype( std::get< 0u >( std::declval< T& >() ) ) >,
    std::tuple_element_t< 0u, T >
  >
> > : public std::true_type {};
template< typename T >
constexpr bool is_tuploid_v = is_tuploid< T >::value;

}

#endif

