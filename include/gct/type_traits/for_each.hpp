#ifndef GCT_TYPE_TRAITS_FOR_EACH_HPP
#define GCT_TYPE_TRAITS_FOR_EACH_HPP

#include <type_traits>

namespace gct::type_traits {

template< template< typename ... > typename F, typename T >
struct for_each {};
template< template< typename ... > typename F, template< typename ... > typename L, typename Head, typename ...Tail >
struct for_each< F, L< Head, Tail... > > {
  using type = concat_sequence_t< L< typename F< Head >::type >, typename for_each< F, L< Tail... > >::type >;
};
template< template< typename ... > typename F, template< typename ... > typename L >
struct for_each< F, L<> > {
  using type = L<>;
};

template< template< typename ... > typename F, typename T >
using for_each_t = typename for_each< F, T >::type;

}

#endif

