#include <gct/round_up.hpp>

namespace gct {
  std::uint32_t round_up( std::uint32_t value, std::uint32_t alignment ) {
    return ( ( value + alignment - 1 ) / alignment ) * alignment;
  }
}

