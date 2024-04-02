#include <gct/is_pot.hpp>

namespace gct {
  std::uint32_t get_pot( std::uint32_t v ) {
    for( std::uint32_t i = 0u; i != 32u; ++i )
      if( ( 1u << i ) >= v ) return i;
    return 32u;
  }
  bool is_pot( std::uint32_t v ) {
    return 1u << get_pot( v ) == v;
  }
}

