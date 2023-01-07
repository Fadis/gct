#ifndef GCT_TYPE_TRAITS_REPLACE_SEQUENCE_HPP
#define GCT_TYPE_TRAITS_REPLACE_SEQUENCE_HPP

#include <type_traits>
#include <gct/type_traits/concat_sequence.hpp>

namespace gct::type_traits {

  template< typename T, typename U, typename V, typename Enable = void >
  struct replace_sequence {};
  template< template< typename ... > typename L, typename Head, typename ...Tail, typename U, typename V >
  struct replace_sequence<
    L< Head, Tail... >, U, V,
    std::enable_if_t< std::is_same_v< Head, U > >
  > {
    using type = concat_sequence_t< L< V >, typename replace_sequence< L< Tail... >, U, V >::type >;
  };
  template< template< typename ... > typename L, typename Head, typename ...Tail, typename U, typename V >
  struct replace_sequence<
    L< Head, Tail... >, U, V,
    std::enable_if_t< !std::is_same_v< Head, U > >
  > {
    using type = concat_sequence_t< L< Head >, typename replace_sequence< L< Tail... >, U, V >::type >;
  };
  template< template< typename ... > typename L, typename U >
  struct replace_sequence<
    L<>, U, void
  > {
    using type = L<>;
  };
  template< typename T, typename U, typename V >
  using replace_sequence_t = typename replace_sequence< T, U, V >::type;

}

#endif

