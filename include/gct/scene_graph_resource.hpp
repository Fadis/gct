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
  LIBGCT_SETTER( accessor )
  LIBGCT_SETTER( mesh )
  LIBGCT_SETTER( meshlet )
  LIBGCT_SETTER( meshlet_index )
  LIBGCT_SETTER( particle )
  LIBGCT_SETTER( rigid )
  LIBGCT_SETTER( distance_constraint )
  LIBGCT_SETTER( constraint )
  LIBGCT_SETTER( spatial_hash )
  LIBGCT_SETTER( vertex_to_primitive )
  LIBGCT_SETTER( lambda )
  LIBGCT_SETTER( last_visibility )
  LIBGCT_SETTER( resource_pair )
  LIBGCT_SETTER( vertex )
  LIBGCT_SETTER( light )
  LIBGCT_SETTER( camera )
  LIBGCT_SETTER( adjacency )
  LIBGCT_SETTER( ppll_state )
  LIBGCT_SETTER( descriptor_set_layout )
  LIBGCT_SETTER( descriptor_set )
  LIBGCT_SETTER( texture_descriptor_set )
  LIBGCT_SETTER( image_descriptor_set )
  LIBGCT_SETTER( vertex_buffer_descriptor_set )
  LIBGCT_SETTER( descriptor_set_id )
  LIBGCT_SETTER( texture_descriptor_set_id )
  LIBGCT_SETTER( image_descriptor_set_id )
  LIBGCT_SETTER( vertex_buffer_descriptor_set_id )
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
  std::shared_ptr< buffer_pool > accessor;
  std::shared_ptr< buffer_pool > mesh;
  std::shared_ptr< buffer_pool > meshlet;
  std::shared_ptr< buffer_pool > meshlet_index;
  std::shared_ptr< buffer_pool > particle;
  std::shared_ptr< buffer_pool > rigid;
  std::shared_ptr< buffer_pool > distance_constraint;
  std::shared_ptr< buffer_pool > constraint;
  std::shared_ptr< buffer_pool > spatial_hash;
  std::shared_ptr< buffer_pool > vertex_to_primitive;
  std::shared_ptr< buffer_pool > lambda;
  std::shared_ptr< buffer_pool > resource_pair;
  std::shared_ptr< buffer_pool > camera;
  std::shared_ptr< buffer_pool > adjacency;
  std::shared_ptr< buffer_pool > ppll_state;
  std::shared_ptr< mappable_buffer_t > last_visibility;
  std::shared_ptr< vertex_buffer_pool > vertex;
  std::shared_ptr< light_pool > light;
  std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_layout_t > > descriptor_set_layout;
  std::shared_ptr< descriptor_set_t > descriptor_set;
  std::shared_ptr< descriptor_set_t > image_descriptor_set;
  std::shared_ptr< descriptor_set_t > texture_descriptor_set;
  std::shared_ptr< descriptor_set_t > vertex_buffer_descriptor_set;
  std::uint32_t descriptor_set_id = 0u;
  std::uint32_t texture_descriptor_set_id = 2u;
  std::uint32_t image_descriptor_set_id = 3u;
  std::uint32_t vertex_buffer_descriptor_set_id = 4u;
  std::shared_ptr< pipeline_layout_t > pipeline_layout;
  std::unordered_map< std::string, uint32_t > attr2index;
  std::optional< spv_member_pointer > push_constant_mp;
  pool< std::shared_ptr< primitive > > prim;
  pool< std::shared_ptr< instance > > inst;
  color_space_matrix csmat;
};

void to_json( nlohmann::json&, const scene_graph_resource& );

#define LIBGCT_SET_SCENE_GRAPH_IMPL( r ) \
  if( ( r ) ) { \
    external_pipeline_layout = ( r ) ->pipeline_layout; \
    { \
      const auto iter = ( r ) ->descriptor_set_layout.find( ( r ) ->image_descriptor_set_id ); \
      if( iter != ( r ) ->descriptor_set_layout.end() && ( r ) ->image_descriptor_set ) { \
        descriptor_set_layout[ ( r ) ->image_descriptor_set_id ] = iter->second; \
        external_descriptor_set[ ( r ) ->image_descriptor_set_id ] = ( r ) ->image_descriptor_set; \
      } \
    } \
    { \
      const auto iter = ( r ) ->descriptor_set_layout.find( ( r ) ->texture_descriptor_set_id ); \
      if( iter != ( r ) ->descriptor_set_layout.end() && ( r ) ->texture_descriptor_set ) { \
        descriptor_set_layout[ ( r ) ->texture_descriptor_set_id ] = iter->second; \
        external_descriptor_set[ ( r ) ->texture_descriptor_set_id ] = ( r ) ->texture_descriptor_set; \
      } \
    } \
    { \
      const auto iter = ( r ) ->descriptor_set_layout.find( ( r ) ->vertex_buffer_descriptor_set_id ); \
      if( iter != ( r ) ->descriptor_set_layout.end() && ( r ) ->vertex_buffer_descriptor_set ) { \
        descriptor_set_layout[ ( r ) ->vertex_buffer_descriptor_set_id ] = iter->second; \
        external_descriptor_set[ ( r ) ->vertex_buffer_descriptor_set_id ] = ( r ) ->vertex_buffer_descriptor_set; \
      } \
    } \
    if( ( r ) ->matrix ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->matrix->get_props().allocator_set ); \
      } \
      add_resource( { "matrix_pool", ( r ) ->matrix->get_buffer() } ); \
    } \
    if( ( r ) ->aabb ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->aabb->get_props().allocator_set ); \
      } \
      add_resource( { "aabb_pool", ( r ) ->aabb->get_buffer() } ); \
    } \
    if( ( r ) ->primitive_resource_index ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->primitive_resource_index->get_props().allocator_set ); \
      } \
      add_resource( { "primitive_resource_index", ( r ) ->primitive_resource_index->get_buffer() } ); \
    } \
    if( ( r ) ->instance_resource_index ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->instance_resource_index->get_props().allocator_set ); \
      } \
      add_resource( { "instance_resource_index", ( r ) ->instance_resource_index->get_buffer() } ); \
    } \
    if( ( r ) ->visibility ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->visibility->get_props().allocator_set ); \
      } \
      add_resource( { "visibility_pool", ( r ) ->visibility->get_buffer() } ); \
    } \
    if( ( r ) ->last_visibility ) { \
      add_resource( { "last_visibility_pool", ( r ) ->last_visibility->get_buffer() } ); \
    } \
    if( ( r ) ->light ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->light->get_props().allocator_set ); \
      } \
      add_resource( { "light_pool", ( r ) ->light->get_buffer() } ); \
    } \
    if( ( r ) ->accessor ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->accessor->get_props().allocator_set ); \
      } \
      add_resource( { "accessor_pool", ( r ) ->accessor->get_buffer() } ); \
    } \
    if( ( r ) ->mesh ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->mesh->get_props().allocator_set ); \
      } \
      add_resource( { "mesh_pool", ( r ) ->mesh->get_buffer() } ); \
    } \
    if( ( r ) ->meshlet ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->meshlet->get_props().allocator_set ); \
      } \
      add_resource( { "meshlet_pool", ( r ) ->meshlet->get_buffer() } ); \
    } \
    if( ( r ) ->meshlet_index ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->meshlet_index->get_props().allocator_set ); \
      } \
      add_resource( { "meshlet_index_pool", ( r ) ->meshlet_index->get_buffer() } ); \
    } \
    if( ( r ) ->resource_pair ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->resource_pair->get_props().allocator_set ); \
      } \
      add_resource( { "resource_pair", ( r ) ->resource_pair->get_buffer() } ); \
    } \
    if( ( r ) ->particle ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->particle->get_props().allocator_set ); \
      } \
      add_resource( { "particle_pool", ( r ) ->particle->get_buffer() } ); \
    } \
    if( ( r ) ->rigid ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->rigid->get_props().allocator_set ); \
      } \
      add_resource( { "rigid_pool", ( r ) ->rigid->get_buffer() } ); \
    } \
    if( ( r ) ->distance_constraint ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->distance_constraint->get_props().allocator_set ); \
      } \
      add_resource( { "distance_constraint_pool", ( r ) ->distance_constraint->get_buffer() } ); \
    } \
    if( ( r ) ->constraint ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->constraint->get_props().allocator_set ); \
      } \
      add_resource( { "constraint_pool", ( r ) ->constraint->get_buffer() } ); \
    } \
    if( ( r ) ->spatial_hash ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->spatial_hash->get_props().allocator_set ); \
      } \
      add_resource( { "spatial_hash_pool", ( r ) ->spatial_hash->get_buffer() } ); \
    } \
    if( ( r ) ->vertex_to_primitive ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->vertex_to_primitive->get_props().allocator_set ); \
      } \
      add_resource( { "vertex_to_primitive_pool", ( r ) ->vertex_to_primitive->get_buffer() } ); \
    } \
    if( ( r ) ->lambda ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->lambda->get_props().allocator_set ); \
      } \
      add_resource( { "lambda_pool", ( r ) ->lambda->get_buffer() } ); \
    } \
    if( ( r ) ->camera ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->camera->get_props().allocator_set ); \
      } \
      add_resource( { "camera_pool", ( r ) ->camera->get_buffer() } ); \
    } \
    if( ( r ) ->adjacency ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->adjacency->get_props().allocator_set ); \
      } \
      add_resource( { "adjacency_pool", ( r ) ->adjacency->get_buffer() } ); \
    } \
    if( ( r ) ->ppll_state ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->ppll_state->get_props().allocator_set ); \
      } \
      add_resource( { "ppll_state_pool", ( r ) ->ppll_state->get_buffer() } ); \
    } \
    ignore_unused_descriptor = true; \
  }


}

#endif
