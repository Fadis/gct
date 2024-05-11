#ifndef GCT_RAW_RESOURCE_PAIR_TYPE_HPP
#define GCT_RAW_RESOURCE_PAIR_TYPE_HPP

#include <cstdint>
#include <gct/setter.hpp>

namespace gct::scene_graph {
struct raw_resource_pair_type {
  LIBGCT_SETTER( inst )
  LIBGCT_SETTER( prim )
  std::uint32_t inst = 0u;
  std::uint32_t prim = 0u;
};

}

#endif
