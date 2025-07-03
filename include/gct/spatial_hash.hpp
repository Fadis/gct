#ifndef GCT_SPATIAL_HASH_HPP
#define GCT_SPATIAL_HASH_HPP

#include <cstdint>
#include <gct/setter.hpp>

namespace gct {

struct spatial_hash_config {
  LIBGCT_SETTER( offset )
  LIBGCT_SETTER( size )
  LIBGCT_SETTER( scale )
  std::uint32_t offset = 0u;
  std::uint32_t size = 0u;
  float scale = 0.f;
};

}

#endif

