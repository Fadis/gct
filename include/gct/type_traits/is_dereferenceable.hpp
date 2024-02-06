#ifndef GCT_TYPE_TRAITS_IS_DEREFERENCEABLE_HPP
#define GCT_TYPE_TRAITS_IS_DEREFERENCEABLE_HPP

#include <type_traits>
#include <gct/voider.hpp>

namespace gct::type_traits {

template< typename T, typename Enable = void >
struct is_dereferenceable : public std::false_type {};
template< typename T >
struct is_dereferenceable<
  T,
  void_t< decltype( *std::declval< T >() ) >
> : public std::true_type {};
template< typename T >
constexpr bool is_dereferenceable_v = is_dereferenceable< T >::value;

}

#endif

