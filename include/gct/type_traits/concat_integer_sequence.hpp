#ifndef GCT_TYPE_TRAITS_CONCAT_INTEGER_SEQUENCE_HPP
#define GCT_TYPE_TRAITS_CONCAT_INTEGER_SEQUENCE_HPP

namespace gct::type_traits {

  template< typename T, typename U  >
  struct concat_integer_sequence_pair {};
  template< typename I, I ...s1, I ...s2 >
  struct concat_integer_sequence_pair<
    std::integer_sequence< I, s1... >, std::integer_sequence< I, s2... >
  > {
    using type = std::integer_sequence< I, s1..., s2... >;
  };
  template< typename T, typename U >
  using concat_integer_sequence_pair_t = typename concat_integer_sequence_pair< T, U >::type;

  template< typename ...T >
  struct concat_integer_sequence {
    using type = void;
  };
  template< typename Head, typename Next, typename ...Tail >
  struct concat_integer_sequence< Head, Next, Tail... > :
    public concat_integer_sequence< concat_integer_sequence_pair_t< Head, Next >, Tail... > {};
  template< typename Head >
  struct concat_integer_sequence< Head > {
    using type = Head;
  };
  template< typename ...T >
  using concat_integer_sequence_t = typename concat_integer_sequence< T... >::type;
}

#endif

