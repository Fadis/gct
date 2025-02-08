#ifndef GCT_LOAD_OBJ_PARTICLE_HPP
#define GCT_LOAD_OBJ_PARTICLE_HPP

#include <utility>
#include <cstdint>
#include <vector>
#include <filesystem>
#include <tuple>
#include <gct/aabb.hpp>
#include <gct/vertex_attributes.hpp>

namespace gct {
[[nodiscard]] std::tuple<
  std::vector< std::uint8_t >,
  std::uint32_t,
  aabb
>
load_obj_particle(
  const std::filesystem::path &fileanme,
  const std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t > &vamap,
  std::uint32_t stride
);
}

#endif

