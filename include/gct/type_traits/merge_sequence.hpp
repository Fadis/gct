#ifndef GCT_TYPE_TRAITS_MERGE_SEQUENCE_HPP
#define GCT_TYPE_TRAITS_MERGE_SEQUENCE_HPP

#include <type_traits>

namespace gct::type_traits {

  template< typename T, typename U, typename Enable = void >
  struct contains {};
  template< template< typename ... > typename L, typename Head, typename ...Tail, typename U >
  struct contains<
    L< Head, Tail... >, U,
    std::enable_if_t< std::is_same_v< Head, U > >
  > : public std::true_type {};
  template< template< typename ... > typename L, typename Head, typename ...Tail, typename U >
  struct contains<
    L< Head, Tail... >, U,
    std::enable_if_t< !std::is_same_v< Head, U > >
  > : public contains< L<  Tail... >, U > {};
  template< template< typename ... > typename L, typename U >
  struct contains<
    L<>, U, void
  > : public std::false_type {};
  template< typename T, typename U >
  constexpr bool contains_v = contains< T, U >::value;


  template< typename T, typename U, typename Enable = void >
  struct merge_sequence_pair {};
  template< template< typename ... > typename L, typename ...Args1, typename Head, typename ...Tail >
  struct merge_sequence_pair<
    L< Args1... >,
    L< Head, Tail... >,
    std::enable_if_t< contains_v< L< Args1... >, Head > >
  > {
    using type = typename merge_sequence_pair<
      L< Args1... >,
      L< Tail... >
    >::type;
  };
  template< template< typename ... > typename L, typename ...Args1, typename Head, typename ...Tail >
  struct merge_sequence_pair<
    L< Args1... >,
    L< Head, Tail... >,
    std::enable_if_t< !contains_v< L< Args1... >, Head > >
  > {
    using type = typename merge_sequence_pair<
      L< Args1..., Head >,
      L< Tail... >
    >::type;
  };
  template< template< typename ... > typename L, typename ...Args1 >
  struct merge_sequence_pair<
    L< Args1... >,
    L<>,
    void
  > {
    using type = L< Args1... >;
  };
  template< typename T, typename U >
  using merge_sequence_pair_t = typename merge_sequence_pair< T, U >::type;
  
  template< typename ...T >
  struct merge_sequence {
    using type = void;
  };
  template< typename Head, typename Next, typename ...Tail >
  struct merge_sequence< Head, Next, Tail... > :
    public merge_sequence< merge_sequence_pair_t< Head, Next >, Tail... > {};
  template< typename Head >
  struct merge_sequence< Head > {
    using type = Head;
  };
  template< typename ...T >
  using merge_sequence_t = typename merge_sequence< T... >::type;

}

#endif

