#ifndef GCT_TYPE_TRAITS_HAS_REBUILD_CHAIN_HPP
#define GCT_TYPE_TRAITS_HAS_REBUILD_CHAIN_HPP

#include <type_traits>
#include <gct/voider.hpp>

namespace gct::type_traits {

template< typename T, typename Enable = void >
struct has_rebuild_chain : public std::false_type {};
template< typename T >
struct has_rebuild_chain< T, void_t< decltype( std::declval< T& >().rebuild_chain() ) > >
  : public std::true_type {};
template< typename T >
constexpr bool has_rebuild_chain_v = has_rebuild_chain< T >::value;

}

#endif

