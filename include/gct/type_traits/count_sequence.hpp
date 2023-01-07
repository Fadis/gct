#ifndef GCT_TYPE_TRAITS_COUNT_SEQUENCE_HPP
#define GCT_TYPE_TRAITS_COUNT_SEQUENCE_HPP

#include <type_traits>
#include <gct/type_traits/concat_sequence.hpp>

namespace gct::type_traits {

  template< typename T, typename U, typename Enable = void >
  struct count_sequence {};
  template< template< typename ... > typename L, typename Head, typename ...Tail, typename U >
  struct count_sequence<
    L< Head, Tail... >, U,
    std::enable_if_t< std::is_same_v< Head, U > >
  > {
    constexpr static unsigned int value = count_sequence< L< Tail... >, U >::value + 1u;
  };
  template< template< typename ... > typename L, typename Head, typename ...Tail, typename U >
  struct count_sequence<
    L< Head, Tail... >, U,
    std::enable_if_t< !std::is_same_v< Head, U > >
  > : public count_sequence< L< Tail... >, U > {};
  template< template< typename ... > typename L, typename U >
  struct count_sequence<
    L<>, U, void
  > {
    constexpr static unsigned int value = 0u;
  };
  template< typename T, typename U >
  constexpr unsigned int count_sequence_v = count_sequence< T, U >::value;

}

#endif

