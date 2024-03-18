#ifndef GCT_LINEAR_ALLOCATOR_CREATE_INFO_HPP
#define GCT_LINEAR_ALLOCATOR_CREATE_INFO_HPP

#include <cstdint>
#include <type_traits>
#include <utility>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>

namespace gct {

struct linear_allocator_create_info {
  LIBGCT_SETTER( max )
  std::uint32_t max = 0u;
};
void to_json( nlohmann::json &dest, const linear_allocator_create_info& );
void from_json( const nlohmann::json &src, linear_allocator_create_info& );
}

#endif

