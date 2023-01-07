#ifndef GCT_TYPE_TRAITS_FILTER_SEQUENCE_HPP
#define GCT_TYPE_TRAITS_FILTER_SEQUENCE_HPP

#include <type_traits>
#include <gct/type_traits/concat_sequence.hpp>

namespace gct::type_traits {

  template< typename T, typename U, typename Enable = void >
  struct filter_sequence {};
  template< template< typename ... > typename L, typename Head, typename ...Tail, typename U >
  struct filter_sequence<
    L< Head, Tail... >, U,
    std::enable_if_t< std::is_same_v< Head, U > >
  > : public filter_sequence< L< Tail... >, U > {};
  template< template< typename ... > typename L, typename Head, typename ...Tail, typename U >
  struct filter_sequence<
    L< Head, Tail... >, U,
    std::enable_if_t< !std::is_same_v< Head, U > >
  > {
    using type = concat_sequence_t< L< Head >, typename filter_sequence< L< Tail... >, U >::type >;
  };
  template< template< typename ... > typename L, typename U >
  struct filter_sequence<
    L<>, U, void
  > {
    using type = L<>;
  };
  template< typename T, typename U >
  using filter_sequence_t = typename filter_sequence< T, U >::type;

}

#endif

