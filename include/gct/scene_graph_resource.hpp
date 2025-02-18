#ifndef GCT_SCENE_GRAPH_RESOURCE_HPP
#define GCT_SCENE_GRAPH_RESOURCE_HPP
#include <string>
#include <cstdint>
#include <memory>
#include <optional>
#include <unordered_map>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/shader_flag.hpp>
#include <gct/color_space.hpp>
#include <gct/pool.hpp>
#include <gct/spv_member_pointer.hpp>

namespace gct {

class pipeline_layout_t;
class matrix_pool;
class aabb_pool;
class image_pool;
class texture_pool;
class sampler_pool;
class buffer_pool;
class mappable_buffer_t;
class vertex_buffer_pool;
class light_pool;
class descriptor_set_layout_t;
class named_resource;
}

namespace gct::scene_graph {

class primitive;
class instance;
class scene_graph_create_info;

struct scene_graph_resource {
  scene_graph_resource(
    const scene_graph_create_info &ci
  );
  [[nodiscard]] operator std::vector< named_resource > ();
  LIBGCT_SETTER( matrix )
  LIBGCT_SETTER( aabb )
  LIBGCT_SETTER( image )
  LIBGCT_SETTER( texture )
  LIBGCT_SETTER( sampler )
  LIBGCT_SETTER( primitive_resource_index )
  LIBGCT_SETTER( instance_resource_index )
  LIBGCT_SETTER( visibility )
  LIBGCT_SETTER( last_visibility )
  LIBGCT_SETTER( resource_pair )
  LIBGCT_SETTER( vertex )
  LIBGCT_SETTER( light )
  LIBGCT_SETTER( descriptor_set_layout )
  LIBGCT_SETTER( descriptor_set )
  LIBGCT_SETTER( texture_descriptor_set )
  LIBGCT_SETTER( image_descriptor_set )
  LIBGCT_SETTER( descriptor_set_id )
  LIBGCT_SETTER( texture_descriptor_set_id )
  LIBGCT_SETTER( image_descriptor_set_id )
  LIBGCT_SETTER( pipeline_layout )
  LIBGCT_SETTER( push_constant_mp )
  LIBGCT_SETTER( prim )
  LIBGCT_SETTER( inst )
  LIBGCT_SETTER( csmat )
  std::shared_ptr< matrix_pool > matrix;
  std::shared_ptr< aabb_pool > aabb;
  std::shared_ptr< image_pool > image;
  std::shared_ptr< texture_pool > texture;
  std::shared_ptr< sampler_pool > sampler;
  std::shared_ptr< buffer_pool > primitive_resource_index;
  std::shared_ptr< buffer_pool > instance_resource_index;
  std::shared_ptr< buffer_pool > visibility;
  std::shared_ptr< mappable_buffer_t > last_visibility;
  std::shared_ptr< mappable_buffer_t > resource_pair;
  std::shared_ptr< vertex_buffer_pool > vertex;
  std::shared_ptr< light_pool > light;
  std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_layout_t > > descriptor_set_layout;
  std::shared_ptr< descriptor_set_t > descriptor_set;
  std::shared_ptr< descriptor_set_t > image_descriptor_set;
  std::shared_ptr< descriptor_set_t > texture_descriptor_set;
  std::uint32_t descriptor_set_id = 0u;
  std::uint32_t texture_descriptor_set_id = 2u;
  std::uint32_t image_descriptor_set_id = 3u;
  std::shared_ptr< pipeline_layout_t > pipeline_layout;
  std::unordered_map< std::string, uint32_t > attr2index;
  std::optional< spv_member_pointer > push_constant_mp;
  pool< std::shared_ptr< primitive > > prim;
  pool< std::shared_ptr< instance > > inst;
  color_space_matrix csmat;
};

void to_json( nlohmann::json&, const scene_graph_resource& );

}

#endif
