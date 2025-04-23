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
  LIBGCT_SETTER( vertex_attribute )
  LIBGCT_SETTER( mesh )
  LIBGCT_SETTER( lod )
  LIBGCT_SETTER( last_visibility )
  LIBGCT_SETTER( resource_pair )
  LIBGCT_SETTER( vertex )
  LIBGCT_SETTER( light )
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
  std::shared_ptr< buffer_pool > vertex_attribute;
  std::shared_ptr< buffer_pool > mesh;
  std::shared_ptr< buffer_pool > lod;
  std::shared_ptr< mappable_buffer_t > last_visibility;
  std::shared_ptr< mappable_buffer_t > resource_pair;
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
      add_resource( { "visibility", ( r ) ->visibility->get_buffer() } ); \
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
    if( ( r ) ->vertex_attribute ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->vertex_attribute->get_props().allocator_set ); \
      } \
      add_resource( { "vertex_attribute_pool", ( r ) ->vertex_attribute->get_buffer() } ); \
    } \
    if( ( r ) ->mesh ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->mesh->get_props().allocator_set ); \
      } \
      add_resource( { "mesh_pool", ( r ) ->mesh->get_buffer() } ); \
    } \
    if( ( r ) ->lod ) { \
      if( !allocator_set.allocator ) { \
        set_allocator_set( ( r ) ->lod->get_props().allocator_set ); \
      } \
      add_resource( { "lod_pool", ( r ) ->lod->get_buffer() } ); \
    } \
    ignore_unused_descriptor = true; \
  }


}

#endif
