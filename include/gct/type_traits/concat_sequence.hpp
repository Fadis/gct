#ifndef GCT_TYPE_TRAITS_CONCAT_SEQUENCE_HPP
#define GCT_TYPE_TRAITS_CONCAT_SEQUENCE_HPP

#include <type_traits>

namespace gct::type_traits {

  template< typename T, typename U  >
  struct concat_sequence_pair {};
  template< template< typename ... > typename L, typename ...T1, typename ...T2 >
  struct concat_sequence_pair<
    L< T1... >, L< T2... >
  > {
    using type = L< T1..., T2... >;
  };
  template< typename T, typename U >
  using concat_sequence_pair_t = typename concat_sequence_pair< T, U >::type;

  template< typename ...T >
  struct concat_sequence {
    using type = void;
  };
  template< typename Head, typename Next, typename ...Tail >
  struct concat_sequence< Head, Next, Tail... > :
    public concat_sequence< concat_sequence_pair_t< Head, Next >, Tail... > {};
  template< typename Head >
  struct concat_sequence< Head > {
    using type = Head;
  };
  template< typename ...T >
  using concat_sequence_t = typename concat_sequence< T... >::type;
}

#endif

