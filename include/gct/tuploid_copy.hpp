#ifndef GCT_TUPLOID_COPY_HPP
#define GCT_TUPLOID_COPY_HPP
#include <tuple>
#include <gct/concepts/tuploid.hpp>

namespace gct {

namespace detail {
template< Tuploid From, typename To, typename Index = std::make_index_sequence< std::tuple_size_v< From > > >
struct tuploid_copy {};
template< Tuploid From, typename To, std::size_t ...I >
struct tuploid_copy< From, To, std::index_sequence< I... > > {
  To operator()( From &&v ) const {
    return To(
      std::get< I >( std::forward< From >( v ) )...
    );
  }
};
template<
  Tuploid F1,
  Tuploid F2,
  typename To,
  typename I1 = std::make_index_sequence< std::tuple_size_v< F1 > >,
  typename I2 = std::make_index_sequence< std::tuple_size_v< F2 > >
>
struct tuploid_cat {};
template< Tuploid F1, Tuploid F2, typename To, std::size_t ...I, std::size_t ...J >
struct tuploid_cat< F1, F2, To, std::index_sequence< I... >, std::index_sequence< J... > > {
  To operator()( F1 &&f1, F2 &&f2 ) const {
    return To(
      std::get< I >( std::forward< F1 >( f1 ) )...,
      std::get< J >( std::forward< F2 >( f2 ) )...
    );
  }
};

}

template< Tuploid From, typename To >
To tuploid_copy( From &&v ) {
  return detail::tuploid_copy< From, To >( std::forward< From >( v ) );
}

template< Tuploid F1, Tuploid F2, typename To >
To tuploid_cat( F1 &&f1, F2 &&f2 ) {
  return detail::tuploid_cat< F1, F2, To >( std::forward< F1 >( f1 ), std::forward< F2 >( f2 ) );
}

}

#endif

