#ifndef GCT_TYPE_TRAITS_FILTERED_INDEX_SEQUENCE_HPP
#define GCT_TYPE_TRAITS_FILTERED_INDEX_SEQUENCE_HPP

#include <limits>
#include <type_traits>
#include <gct/type_traits/concat_integer_sequence.hpp>

namespace gct::type_traits {

  template< typename T, typename U, std::size_t i = 0u, typename Enable = void >
  struct filter_index_sequence {};
  template< template< typename ... > typename L, typename Head, typename ...Tail, typename U, std::size_t i >
  struct filter_index_sequence<
    L< Head, Tail... >, U, i,
    std::enable_if_t< std::is_same_v< Head, U > >
  > {
    using type = concat_integer_sequence_t<
      std::index_sequence< std::numeric_limits< std::size_t >::max() >,
      typename filter_index_sequence< L< Tail... >, U, i >::type
    >;
  };
  template< template< typename ... > typename L, typename Head, typename ...Tail, typename U, std::size_t i >
  struct filter_index_sequence<
    L< Head, Tail... >, U, i,
    std::enable_if_t< !std::is_same_v< Head, U > >
  > {
    using type = concat_integer_sequence_t<
      std::index_sequence< i >,
      typename filter_index_sequence< L< Tail... >, U, i + 1u >::type
    >;
  };
  template< template< typename ... > typename L, typename U, std::size_t i >
  struct filter_index_sequence<
    L<>, U, i, void
  > {
    using type = std::index_sequence<>;
  };
  template< typename T, typename U >
  using filter_index_sequence_t = typename filter_index_sequence< T, U >::type;

}

#endif

