#ifndef GCT_TYPE_TRAITS_IS_LIFTED_HPP
#define GCT_TYPE_TRAITS_IS_LIFTED_HPP

#include <type_traits>

namespace gct::type_traits {

template< typename T, template< typename... > typename L >
struct is_lifted_by : public std::false_type {};
template< typename ...T, template< typename... > typename L >
struct is_lifted_by< L< T... >, L > : public std::true_type {};
template< typename T, template< typename... > typename L >
constexpr bool is_lifted_by_v = is_lifted_by< T, L >::value;

template< typename T >
struct is_lifted : public std::false_type {};
template< typename ...T, template< typename... > typename L >
struct is_lifted< L< T... > > : public std::true_type {};
template< typename T >
constexpr bool is_lifted_v = is_lifted< T >::value;

}

#endif

