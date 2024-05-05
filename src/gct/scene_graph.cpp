#include <nlohmann/json.hpp>
#include <vulkan2json/IndexType.hpp>
#include <vulkan2json/VertexInputBindingDescription.hpp>
#include <vulkan2json/VertexInputAttributeDescription.hpp>
#include <gct/instance.hpp>
#include <gct/device.hpp>
#include <gct/shader_module_reflection.hpp>
#include <gct/descriptor_pool_create_info.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set_layout_create_info.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/pipeline_layout_create_info.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/sampler_create_info.hpp>
#include <gct/sampler.hpp>
#include <gct/allocator.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/get_device.hpp>
#include <gct/vertex_attributes.hpp>
#include <gct/exception.hpp>
#include <gct/scene_graph.hpp>

namespace gct::scene_graph {
void to_json( nlohmann::json &dest, const buffer_offset &src ) {
  dest = nlohmann::json::object();
  if( src.buffer ) {
    dest[ "buffer" ] = *src.buffer;
  }
  dest[ "offset" ] = src.offset;
}

void to_json( nlohmann::json &dest, const scene_graph_create_info &src ) {
  dest = nlohmann::json::object();
  if( src.allocator ) {
    dest[ "allocator" ] = *src.allocator;
  }
  if( src.descriptor_pool ) {
    dest[ "descriptor_pool" ] = *src.descriptor_pool;
  }
  if( src.pipeline_cache ) {
    dest[ "pipeline_cache" ] = *src.pipeline_cache;
  }
  dest[ "master_shader" ] = nlohmann::json::array();
  for( const auto &v: src.master_shader ) {
    dest[ "master_shader" ].push_back( v );
  }
  dest[ "matrix" ] = src.matrix;
  dest[ "aabb" ] = src.aabb;
  dest[ "image" ] = src.image;
  dest[ "texture" ] = src.texture;
  dest[ "sampler" ] = src.sampler;
  dest[ "primitive_resource_index" ] = src.primitive_resource_index;
  dest[ "visibility" ] = src.visibility;
  dest[ "vertex" ] = src.vertex;
  dest[ "prim_pool_size" ] = src.prim_pool_size;
  dest[ "inst_pool_size" ] = src.inst_pool_size;
  dest[ "descriptor_set_id" ] = src.descriptor_set_id;
  dest[ "texture_descriptor_set_id" ] = src.texture_descriptor_set_id;
}

void to_json( nlohmann::json &dest, const scene_graph_resource &src ) {
  dest = nlohmann::json::object();
  if( src.matrix ) {
    dest[ "matrix" ] = *src.matrix;
  }
  if( src.aabb ) {
    dest[ "aabb" ] = *src.aabb;
  }
  if( src.image ) {
    dest[ "image" ] = *src.image;
  }
  if( src.texture ) {
    dest[ "texture" ] = *src.texture;
  }
  if( src.sampler ) {
    dest[ "sampler" ] = *src.sampler;
  }
  if( src.primitive_resource_index ) {
    dest[ "primitive_resource_index" ] = *src.primitive_resource_index;
  }
  if( src.visibility ) {
    dest[ "visibility" ] = *src.visibility;
  }
  if( src.vertex ) {
    dest[ "vertex" ] = *src.vertex;
  }
  dest[ "descriptor_set_layout" ] = nlohmann::json::array();
  for( const auto &v: src.descriptor_set_layout ) {
    if( v ) {
      dest[ "descriptor_set_layout" ].push_back( *v );
    }
    else {
      dest[ "descriptor_set_layout" ].push_back( nullptr );
    }
  }
  if( src.descriptor_set ) {
    dest[ "descriptor_set" ] = *src.descriptor_set;
  }
  if( src.texture_descriptor_set ) {
    dest[ "texture_descriptor_set" ] = *src.texture_descriptor_set;
  }
  dest[ "primitive" ] = nlohmann::json::array();
  for( const auto &v: src.prim.get_value() ) {
    dest[ "primitive" ].push_back( *v );
  }
  dest[ "instance" ] = nlohmann::json::array();
  for( const auto &v: src.inst.get_value() ) {
    dest[ "instance" ].push_back( *v );
  }
}


void to_json( nlohmann::json &dest, const primitive_descriptor &src ) {
  dest = nlohmann::json::object();
  dest[ "vertex_buffer" ] = nlohmann::json::array();
  for( const auto &v: src.vertex_buffer ) {
    dest[ "vertex_buffer" ].push_back( *v );
  }
  dest[ "vertex_buffer_offset" ] = nlohmann::json::array();
  for( const auto &v: src.vertex_buffer_offset ) {
    dest[ "vertex_buffer_offset" ].push_back( v );
  }
  if( src.index_buffer ) {
    dest[ "index_buffer" ] = *src.index_buffer;
  }
  dest[ "index_buffer_offset" ] = src.index_buffer_offset;
  if( src.base_color_texture ) {
    dest[ "base_color_texture" ] = *src.base_color_texture;
  }
  if( src.metallic_roughness_texture ) {
    dest[ "metallic_roughness_texture" ] = *src.metallic_roughness_texture;
  }
  if( src.normal_texture ) {
    dest[ "normal_texture" ] = *src.normal_texture;
  }
  if( src.occlusion_texture ) {
    dest[ "occlusion_texture" ] = *src.occlusion_texture;
  }
  if( src.emissive_texture ) {
    dest[ "emissive_texture" ] = *src.emissive_texture;
  }
  if( src.resource_index ) {
    dest[ "resource_index" ] = *src.resource_index;
  }
  if( src.aabb ) {
    dest[ "aabb" ] = *src.aabb;
  }
}

void to_json( nlohmann::json &dest, const primitive &src ) {
  dest = nlohmann::json::object();
  dest[ "vertex_buffer" ] = nlohmann::json::object();
  for( const auto &v: src.vertex_buffer ) {
    dest[ "vertex_buffer" ][ std::to_string( v.first ) ] = v.second;
  }
  dest[ "indexed" ] = src.indexed;
  dest[ "index_buffer" ] = src.index_buffer;
  dest[ "count" ] = src.count;
  dest[ "index_buffer_type" ] = src.index_buffer_type;
  dest[ "aabb" ] = src.aabb;
  dest[ "vertex_input_binding" ] = nlohmann::json::array();
  for( const auto &v: src.vertex_input_binding ) {
    dest[ "vertex_input_binding" ].push_back( v );
  }
  dest[ "vertex_input_attribute" ] = nlohmann::json::array();
  for( const auto &v: src.vertex_input_attribute ) {
    dest[ "vertex_input_attribute" ].push_back( v );
  }
  dest[ "vs_flag" ] = int( src.vs_flag );
  dest[ "gs_flag" ] = int( src.gs_flag );
  dest[ "fs_flag" ] = int( src.fs_flag );
  dest[ "cull" ] = src.cull;
  dest[ "blend" ] = src.blend;
  dest[ "roughness" ] = src.roughness;
  dest[ "emissive" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != 4u; ++i ) {
    dest[ "emissive" ].push_back( src.emissive[ i ] );
  }
  dest[ "base_color" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != 4u; ++i ) {
    dest[ "base_color" ].push_back( src.emissive[ i ] );
  }
  dest[ "normal_scale" ] = src.normal_scale;
  dest[ "occlusion_strength" ] = src.occlusion_strength;
  dest[ "pipeline_create_info" ] = src.pipeline_create_info;
  dest[ "descriptor" ] = src.descriptor;
}

void to_json( nlohmann::json &dest, const instance_descriptor &src ) {
  dest = nlohmann::json::object();
  if( src.matrix ) {
    dest[ "matrix" ] = *src.matrix;
  }
  if( src.aabb ) {
    dest[ "aabb" ] = *src.aabb;
  }
  if( src.resource_index ) {
    dest[ "resource_index" ] = *src.resource_index;
  }
  if( src.prim ) {
    dest[ "primitive" ] = *src.prim;
  }
  if( src.visibility ) {
    dest[ "visibility" ] = *src.visibility;
  }
};

void to_json( nlohmann::json &dest, const instance &src ) {
  dest = nlohmann::json::object();
  dest[ "descriptor" ] = src.descriptor;
}

std::optional< aabb4 > node::get_initial_world_aabb() const {
  aabb4 a;
  bool first = true;
  if( inst.size() ) {
    a = prim_aabb;
    first = false;
  }
  for( const auto &v: child ) {
    auto child_aabb = v->get_initial_world_aabb();
    if( child_aabb ) {
      if( first ) {
        a = *child_aabb;
        first = false;
      }
      else {
        a = a | *child_aabb;
      }
    }
  }
  if( first ) return std::nullopt;
  else return a;
}

void node::to_json( nlohmann::json &dest ) const {
  dest = nlohmann::json::object();
  dest[ "name" ] = name;
  dest[ "child" ] = nlohmann::json::array();
  for( const auto &c: child ) {
    if( c ) {
      dest[ "child" ].push_back( nlohmann::json( *c ) );
    }
  }
  dest[ "instance" ] = nlohmann::json::array();
  for( const auto &i: inst ) {
    if( i ) {
      dest[ "instance" ].push_back( *i );
    }
  }
  dest[ "initial_world_matrix" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != 4; ++i ) {
    for( unsigned int j = 0u; j != 4; ++j ) {
      dest[ "initial_world_matrix" ].push_back( initial_world_matrix[ i ][ j ] );
    }
  }
  if( matrix ) {
    dest[ "matrix" ] = *matrix;
  }
}

void to_json( nlohmann::json &dest, const node &src ) {
  src.to_json( dest );
}
scene_graph_create_info::scene_graph_create_info() {
  primitive_resource_index.set_buffer_name( "primitive_resource_index" );
  instance_resource_index.set_buffer_name( "instance_resource_index" );
  visibility.set_buffer_name( "visibility" );  
}

scene_graph_create_info &scene_graph_create_info::set_shader( const std::filesystem::path &dir ) {
  matrix.set_shader( dir / "matrix_pool" );
  aabb.set_shader( dir / "aabb_pool" );
  primitive_resource_index.set_shader( dir / "primitive_resource_index_pool" );
  instance_resource_index.set_shader( dir / "instance_resource_index_pool" );
  visibility.set_shader( dir / "visibility_pool" );
  return *this;
}

scene_graph::scene_graph(
  const scene_graph_create_info &ci
) : props( new scene_graph_create_info( ci ) ), resource( new scene_graph_resource( ci ) ) {
  auto &device = get_device( *props->allocator );

  use_conditional = device.get_factory()->get_activated_extensions().find( VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME ) != device.get_factory()->get_activated_extensions().end();

  std::vector< descriptor_set_layout_create_info_t > descriptor_set_layout_create_info;
  bool indexing = false;
#if defined(VK_VERSION_1_2) || defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
  if( props->descriptor_pool->get_props().get_basic().flags & vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind ) {
    indexing = true;  
  }
#endif
  pipeline_layout_create_info_t pipeline_layout_create_info;
  std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t > vertex_attributes;
  for( const auto &d: props->master_shader  ) {
    for( const auto &f: std::filesystem::directory_iterator( d ) ) {
      if( f.path().extension() == ".spv" ) {
        shader_module_reflection_t reflection( f.path() );
        for( auto dsid: reflection.get_descriptor_set_id() ) {
          if( descriptor_set_layout_create_info.size() <= dsid ) {
            descriptor_set_layout_create_info.resize( dsid + 1u );
          }
          descriptor_set_layout_create_info[ dsid ].add_binding( reflection, dsid );
          if( indexing ) {
            descriptor_set_layout_create_info[ dsid ].set_indexing( true );
          }
        }
        pipeline_layout_create_info.add_push_constant_range( reflection );
        auto [visci,partial_vertex_attributes,stride] = get_vertex_attributes(
          device,
          reflection
        );
        if( reflection->shader_stage & SPV_REFLECT_SHADER_STAGE_VERTEX_BIT ) {
          for( const auto &a: partial_vertex_attributes ) {
            vertex_attributes.insert( a );
          }
        }
        auto pcmp = reflection.get_push_constant_member_pointer_maybe( "PushConstants" );
        if( pcmp ) {
          resource->push_constant_mp = *pcmp;
        }
      }
    }
  }
  for( const auto &v: descriptor_set_layout_create_info ) {
    resource->descriptor_set_layout.push_back( device.get_descriptor_set_layout( v ) );
    pipeline_layout_create_info.add_descriptor_set_layout( resource->descriptor_set_layout.back() );
  }
  resource->descriptor_set = props->descriptor_pool->allocate( resource->descriptor_set_layout[ props->descriptor_set_id ] );
  resource->descriptor_set_id = props->descriptor_set_id;
  resource->texture_descriptor_set = props->descriptor_pool->allocate( resource->descriptor_set_layout[ props->texture_descriptor_set_id ], props->texture.max_texture_count );
  resource->texture_descriptor_set_id = props->texture_descriptor_set_id;
  
  resource->pipeline_layout = device.get_pipeline_layout( pipeline_layout_create_info );
  if( vertex_attributes.find( vertex_attribute_usage_t::POSITION ) != vertex_attributes.end() ) {
    resource->attr2index.emplace( std::string( "POSITION" ), vertex_attributes[ vertex_attribute_usage_t::POSITION ].location );
  }
  if( vertex_attributes.find( vertex_attribute_usage_t::NORMAL ) != vertex_attributes.end() ) {
    resource->attr2index.emplace( std::string( "NORMAL" ), vertex_attributes[ vertex_attribute_usage_t::NORMAL ].location );
  }
  if( vertex_attributes.find( vertex_attribute_usage_t::TANGENT ) != vertex_attributes.end() ) {
    resource->attr2index.emplace( std::string( "TANGENT" ), vertex_attributes[ vertex_attribute_usage_t::TANGENT ].location );
  }
  if( vertex_attributes.find( vertex_attribute_usage_t::TEXCOORD ) != vertex_attributes.end() ) {
    resource->attr2index.emplace( std::string( "TEXCOORD_0" ), vertex_attributes[ vertex_attribute_usage_t::TEXCOORD ].location );
  }
  if( vertex_attributes.find( vertex_attribute_usage_t::COLOR ) != vertex_attributes.end() ) {
    resource->attr2index.emplace( std::string( "COLOR_0" ), vertex_attributes[ vertex_attribute_usage_t::COLOR ].location );
  }
  if( vertex_attributes.find( vertex_attribute_usage_t::JOINT ) != vertex_attributes.end() ) {
    resource->attr2index.emplace( std::string( "JOINTS_0" ), vertex_attributes[ vertex_attribute_usage_t::JOINT ].location );
  }
  if( vertex_attributes.find( vertex_attribute_usage_t::WEIGHT ) != vertex_attributes.end() ) {
    resource->attr2index.emplace( std::string( "WEIGHTS_0" ), vertex_attributes[ vertex_attribute_usage_t::WEIGHT ].location );
  }

  resource->matrix.reset( new matrix_pool(
    matrix_pool_create_info( props->matrix )
      .set_allocator( props->allocator )
      .set_descriptor_pool( props->descriptor_pool )
      .set_pipeline_cache( props->pipeline_cache )
  ) );
  resource->aabb.reset( new aabb_pool(
    aabb_pool_create_info( props->aabb )
      .set_allocator( props->allocator )
      .set_descriptor_pool( props->descriptor_pool )
      .set_pipeline_cache( props->pipeline_cache )
      .set_matrix_pool( resource->matrix->get_buffer() )
  ) );
  resource->sampler.reset( new sampler_pool(
    sampler_pool_create_info( props->sampler )
      .set_allocator( props->allocator )
  ) );
  resource->image.reset( new image_pool(
    image_pool_create_info( props->image )
      .set_allocator( props->allocator )
  ) );
  resource->texture.reset( new texture_pool(
    texture_pool_create_info( props->texture )
      .set_allocator( props->allocator )
      .set_descriptor_set( resource->texture_descriptor_set )
      .set_sampler( resource->sampler )
      .set_image( resource->image )
  ) );
  resource->primitive_resource_index.reset( new buffer_pool(
    buffer_pool_create_info( props->primitive_resource_index )
      .set_allocator( props->allocator )
      .set_descriptor_pool( props->descriptor_pool )
      .set_pipeline_cache( props->pipeline_cache )
  ) );
  resource->instance_resource_index.reset( new buffer_pool(
    buffer_pool_create_info( props->instance_resource_index )
      .set_allocator( props->allocator )
      .set_descriptor_pool( props->descriptor_pool )
      .set_pipeline_cache( props->pipeline_cache )
  ) );
  resource->visibility.reset( new buffer_pool(
    buffer_pool_create_info( props->visibility )
      .set_allocator( props->allocator )
      .set_descriptor_pool( props->descriptor_pool )
      .set_pipeline_cache( props->pipeline_cache )
  ) );
  resource->last_visibility = props->allocator->create_mappable_buffer(
    resource->visibility->get_buffer()->get_props().get_basic().size,
    use_conditional ?
      vk::BufferUsageFlagBits::eStorageBuffer|
      vk::BufferUsageFlagBits::eConditionalRenderingEXT :
      vk::BufferUsageFlagBits::eStorageBuffer
  );
  resource->vertex.reset( new vertex_buffer_pool(
    vertex_buffer_pool_create_info( props->vertex )
      .set_allocator( props->allocator )
  ) );
  resource->descriptor_set->update({
    { "primitive_resource_index", resource->primitive_resource_index->get_buffer() },
    { "instance_resource_index", resource->instance_resource_index->get_buffer() },
    { "visibility_pool", resource->visibility->get_buffer() },
    { "matrix_pool", resource->matrix->get_buffer() },
    { "aabb_pool", resource->aabb->get_buffer() }
  });

  root_node.reset( new node(
    props,
    resource,
    glm::mat4(
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, -1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
    )
  ) );
  clear_visibility = true;
}

void scene_graph::to_json( nlohmann::json &dest ) const {
  dest = nlohmann::json::object();
  if( props ) {
    dest[ "props" ] = *props;
  }
  if( root_node ) {
    dest[ "root_node" ] = *root_node;
  }
  if( resource ) {
    dest[ "resource" ] = *resource;
  }
}

void to_json( nlohmann::json &dest, const scene_graph &src ) {
  src.to_json( dest );
}

void scene_graph::operator()( command_buffer_recorder_t &rec ) const {
  if( clear_visibility ) {
    rec.fill( resource->visibility->get_buffer(), 0u );
    rec.barrier( {
      resource->visibility->get_buffer()
    }, {} );
    clear_visibility = false;
  }
  (*resource->matrix)( rec );
  rec.compute_barrier( { resource->matrix->get_buffer() }, {} );
  (*resource->aabb)( rec );
  (*resource->image)( rec );
  (*resource->sampler)();
  (*resource->texture)( rec );
  (*resource->visibility)( rec );
  (*resource->vertex)( rec );
  (*resource->primitive_resource_index)( rec );
  (*resource->instance_resource_index)( rec );
  rec.compute_to_graphics_barrier( {
    resource->matrix->get_buffer(),
    resource->aabb->get_buffer(),
    resource->visibility->get_buffer()
  }, {} );
}

void scene_graph::rotate_visibility( command_buffer_recorder_t &rec ) const {
  rec.barrier( {
    resource->visibility->get_buffer()
  }, {} );
  rec.copy( resource->visibility->get_buffer(), resource->last_visibility->get_buffer() );
  rec.barrier( {
    resource->visibility->get_buffer(),
    resource->last_visibility->get_buffer()
  }, {} );
  rec.sync_to_host( resource->last_visibility );
  rec.fill( resource->visibility->get_buffer(), 0u );
  rec.barrier( {
    resource->visibility->get_buffer()
  }, {} );
}

}

