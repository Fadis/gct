#ifndef GCT_CONCEPTS_TUPLOID_HPP
#define GCT_CONCEPTS_TUPLOID_HPP

#include <gct/type_traits/is_tuploid.hpp>

namespace gct {

template< typename T >
concept Tuploid = gct::type_traits::is_tuploid_v< T >;

}

#endif


