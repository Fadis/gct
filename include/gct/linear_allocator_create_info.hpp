#ifndef GCT_LINEAR_ALLOCATOR_CREATE_INFO_HPP
#define GCT_LINEAR_ALLOCATOR_CREATE_INFO_HPP

#include <cstdint>
#include <type_traits>
#include <utility>
#include <gct/setter.hpp>

namespace gct {

struct linear_allocator_create_info {
  LIBGCT_SETTER( max )
  std::uint32_t max = 0u;
};

}

#endif

