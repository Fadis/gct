#ifndef GCT_TYPE_TRAITS_NTH_TYPE_HPP
#define GCT_TYPE_TRAITS_NTH_TYPE_HPP

#include <type_traits>

namespace gct::type_traits {

template< unsigned int, typename T, typename Enable = void >
struct nth_type {};
template< unsigned int i, template< typename... > typename L, typename Head, typename Next, typename ...Tail >
struct nth_type<
  i,
  L< Head, Next, Tail... >,
  std::enable_if_t< i != 0u >
> : public nth_type< i - 1u, L< Next, Tail... > > {};
template< unsigned int i, template< typename... > typename L, typename Head, typename Next, typename ...Tail >
struct nth_type<
  i,
  L< Head, Next, Tail... >,
  std::enable_if_t< i == 0u >
> {
  using type = Head;
};
template< unsigned int i, template< typename... > typename L, typename Head >
struct nth_type<
  i,
  L< Head >,
  std::enable_if_t< i == 0u >
> {
  using type = Head;
};
template< unsigned int i, typename T >
using nth_type_t = typename nth_type< i, T >::type;

}

#endif


