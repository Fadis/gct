#ifndef GCT_TYPE_TRAITS_IF_HPP
#define GCT_TYPE_TRAITS_IF_HPP

#include <type_traits>

namespace gct::type_traits {

template< bool cond, typename T, typename U >
struct if_ {};
template< typename T, typename U >
struct if_< true, T, U > {
  using type = T;
};
template< typename T, typename U >
struct if_< false, T, U > {
  using type = U;
};

template< bool cond, typename T, typename U >
using if_t = typename if_< cond, T, U >::type;

}

#endif

