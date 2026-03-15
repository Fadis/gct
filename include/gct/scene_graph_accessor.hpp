#ifndef GCT_SCENE_GRAPH_ACCESSOR_HPP
#define GCT_SCENE_GRAPH_ACCESSOR_HPP
#include <nlohmann/json.hpp>
#include <gct/vertex_buffer_pool.hpp>
#include <gct/buffer_pool.hpp>
#include <gct/numeric_types.hpp>
#include <gct/setter.hpp>
#include <vulkan/vulkan_enums.hpp>
namespace gct::scene_graph {

enum class accessor_type_id {
  float_ = 0,
  i8 = 1,
  u8 = 2,
  i16 = 3,
  u16 = 4,
  u32 = 5
};

void to_json( nlohmann::json &dest, const accessor_type_id &src );
accessor_type_id to_type_id( const numeric_type_t &type );

enum class mesh_topology_id {
  point = 0,
  line = 1,
  triangle = 2
};

void to_json( nlohmann::json &dest, const mesh_topology_id &src );

enum class mesh_compression_method_id {
  uncompressed = 0,
  dgf = 1
};

void to_json( nlohmann::json &dest, const mesh_compression_method_id &src );

struct accessor_t {
  LIBGCT_SETTER( buffer )
  LIBGCT_SETTER( type )
  LIBGCT_SETTER( offset )
  LIBGCT_SETTER( stride )
  vertex_buffer_pool::vertex_buffer_descriptor buffer;
  numeric_type_t type; 
  std::uint32_t offset;
  std::uint32_t stride;
};

void to_json( nlohmann::json &dest, const accessor_t &src );

struct mesh_t {
  LIBGCT_SETTER( attribute )
  LIBGCT_SETTER( vertex_count )
  LIBGCT_SETTER( unique_vertex_count )
  LIBGCT_SETTER( topology )
  LIBGCT_SETTER( compression_method )
  std::unordered_map< std::uint32_t, accessor_t > attribute;
  std::uint32_t vertex_count = 0u;
  std::uint32_t unique_vertex_count = 0u;
  vk::PrimitiveTopology topology = vk::PrimitiveTopology::eTriangleList;
  mesh_compression_method_id compression_method = mesh_compression_method_id::uncompressed;
};

void to_json( nlohmann::json &dest, const mesh_t &src );

struct lod_t {
  LIBGCT_SETTER( level )
  std::vector< std::pair< mesh_t, float > > level;
};

void to_json( nlohmann::json &dest, const lod_t &src );

}

#endif

