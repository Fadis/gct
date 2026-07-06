#ifndef GCT_MESH_TYPE_H
#define GCT_MESH_TYPE_H
#include <cstdint>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>

namespace gct {

struct mesh_type {
  std::uint32_t accessor = 0; // 最初のアクセサのオフセット
  std::uint32_t vertex_count = 0; // 頂点数
  std::uint32_t topology = 0; // トポロジ
  std::uint32_t meshlet = 0; // メッシュレット単位の情報
  std::uint32_t unique_vertex_count = 0;
  std::uint32_t particle_offset = 0;
  std::uint32_t distance_constraint_offset = 0;
  std::uint32_t constraint_offset = 0;
  std::uint32_t fluid_constraint_offset = 0;
  std::uint32_t lambda_offset = 0;
  std::uint32_t vertex_to_primitive_offset = 0;
  float stiffness = 0.0f;
  std::uint32_t adjacency_offset = 0;
  std::uint32_t same_position_offset = 0;
  std::uint32_t compression_method = 0;
  std::uint32_t meshlet_count = 0;
};

void to_json( nlohmann::json &dest, const mesh_type &src );
void from_json( const nlohmann::json &dest, mesh_type &src );

}

#endif

