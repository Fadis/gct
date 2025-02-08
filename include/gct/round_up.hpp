#ifndef GCT_ROUND_UP_HPP
#define GCT_ROUND_UP_HPP

#include <cstdint>

namespace gct {
  [[nodiscard]] std::uint32_t round_up( std::uint32_t value, std::uint32_t alignment );
}

#endif

