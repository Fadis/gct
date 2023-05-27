#ifndef GCT_TYPE_TRAITS_TUPLOID_TO_SEQUENCE_HPP
#define GCT_TYPE_TRAITS_TUPLOID_TO_SEQUENCE_HPP

#include <tuple>
#include <type_traits>

namespace gct::type_traits {

template< typename T, std::size_t i = 0u, typename Enable = void >
struct tuploid_to_sequence {};
template< typename T, std::size_t i >
struct tuploid_to_sequence<
  T, i, std::enable_if_t< i != std::tuple_size_v< T > >
> {
  using type = concat_sequence_t<
    type_sequence< std::tuple_element_t< i, T > >,
    typename tuploid_to_sequence< T, i + 1u >::type
  >;
};
template< typename T, std::size_t i >
struct tuploid_to_sequence<
  T, i, std::enable_if_t< i == std::tuple_size_v< T > >
> {
  using type = type_sequence<>;
};
template< typename T >
using tuploid_to_sequence_t = typename tuploid_to_sequence< T >::type;

}

#endif

