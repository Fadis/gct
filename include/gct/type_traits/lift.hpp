#ifndef GCT_TYPE_TRAITS_LIFT_HPP
#define GCT_TYPE_TRAITS_LIFT_HPP

#include <type_traits>

namespace gct::type_traits {

template< template< typename... > typename L, typename T >
struct lift {
  using type = L< T >;
};
template< template< typename... > typename L, typename ...T >
struct lift< L, L< T... > > {
  using type = L< T... >;
};
template< template< typename... > typename L, typename T >
using lift_t = typename lift< L, T >::type;


template< template< typename... > typename L, typename T >
struct unlift {
  using type = T;
};
template< template< typename... > typename L, typename T >
struct unlift< L, L< T > > {
  using type = T;
};
template< template< typename... > typename L, typename T >
using unlift_t = typename unlift< L, T >::type;

template< template< typename... > typename L1, template< typename... > typename L2, typename T >
struct chlift {
  using type = T;
};
template< template< typename... > typename L1, template< typename... > typename L2, typename ...T >
struct chlift< L1, L2, L1< T... > > {
  using type = L2< T... >;
};
template< template< typename... > typename L1, template< typename... > typename L2, typename T >
using chlift_t = typename chlift< L1, L2, T >::type;

}

#endif

