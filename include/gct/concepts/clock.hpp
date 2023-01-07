#ifndef GCT_CONCEPTS_CLOCK_HPP
#define GCT_CONCEPTS_CLOCK_HPP

#include <chrono>

namespace gct {

template< typename T >
concept Clock = std::chrono::is_clock_v< T >;

}

#endif

