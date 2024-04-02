#ifndef GCT_IS_POT_HPP
#define GCT_IS_POT_HPP

#include <cstdint>

namespace gct {
  std::uint32_t get_pot( std::uint32_t v );
  bool is_pot( std::uint32_t v );
}

#endif

