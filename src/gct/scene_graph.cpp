#include <nlohmann/json.hpp>
#include <vulkan2json/IndexType.hpp>
#include <vulkan2json/VertexInputBindingDescription.hpp>
#include <vulkan2json/VertexInputAttributeDescription.hpp>
#include <gct/instance.hpp>
#include <gct/device.hpp>
#include <gct/shader_module_reflection.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/sampler.hpp>
#include <gct/allocator.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/get_device.hpp>
#include <gct/vertex_attributes.hpp>
#include <gct/exception.hpp>
#include <gct/scene_graph.hpp>
#include <gct/color_space.hpp>

namespace gct::scene_graph {

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
  dest[ "light" ] = nlohmann::json::array();
  for( const auto &i: light ) {
    if( i ) {
      dest[ "light" ].push_back( *i );
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
  accessor.set_buffer_name( "accessor_pool" );  
  mesh.set_buffer_name( "mesh_pool" );  
  meshlet.set_buffer_name( "meshlet_pool" );  
  meshlet_index.set_buffer_name( "meshlet_index_pool" );  
  resource_pair.set_buffer_name( "resource_pair" );
}

scene_graph_create_info &scene_graph_create_info::set_shader( const std::filesystem::path &dir ) {
  matrix.set_shader( dir / "matrix_pool" );
  aabb.set_shader( dir / "aabb_pool" );
  image.set_shader( dir / "image_pool" );
  primitive_resource_index.set_shader( dir / "primitive_resource_index_pool" );
  instance_resource_index.set_shader( dir / "instance_resource_index_pool" );
  visibility.set_shader( dir / "visibility_pool" );
  accessor.set_shader( dir / "accessor" );
  mesh.set_shader( dir / "mesh" );
  meshlet.set_shader( dir / "meshlet" );
  meshlet_index.set_shader( dir / "meshlet_index" );
  resource_pair.set_shader( dir / "resource_pair" );
  light.set_shader( dir / "light_pool" );
  return *this;
}

scene_graph::scene_graph(
  const scene_graph_create_info &ci
) : props( new scene_graph_create_info( ci ) ), resource( new scene_graph_resource( ci ) ) {
  auto &device = get_device( *props->allocator_set.allocator );

#ifdef VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME
  use_conditional = device.get_activated_extensions().find( VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME ) != device.get_activated_extensions().end();
#else
  use_conditional = false;
#endif

  std::unordered_map< unsigned int, descriptor_set_layout_create_info_t > descriptor_set_layout_create_info;
  bool indexing = false;
#if defined(VK_VERSION_1_2) || defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
  if( props->allocator_set.descriptor_pool->get_props().get_basic().flags & vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind ) {
    indexing = true;  
  }
#endif
  pipeline_layout_create_info_t pipeline_layout_create_info;
  pipeline_layout_create_info.add_push_constant_range(
    vk::PushConstantRange()
      .setStageFlags( vk::ShaderStageFlagBits::eAll )
      .setOffset( 0u )
      .setSize( 128u )
  );
  std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t > vertex_attributes;
  for( const auto &d: props->master_shader  ) {
    for( const auto &f: std::filesystem::directory_iterator( d ) ) {
      if( f.path().extension() == ".spv" ) {
        shader_module_reflection_t reflection( f.path() );
        for( auto dsid: reflection.get_descriptor_set_id() ) {
          descriptor_set_layout_create_info[ dsid ].add_binding( reflection, dsid );
          if( indexing ) {
            descriptor_set_layout_create_info[ dsid ].set_indexing( true );
          }
        }
        //pipeline_layout_create_info.add_push_constant_range( reflection );
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
    resource->descriptor_set_layout[ v.first ] =  device.get_descriptor_set_layout( v.second );
    pipeline_layout_create_info.add_descriptor_set_layout( v.first, resource->descriptor_set_layout[ v.first ] );
  }
  resource->descriptor_set = props->allocator_set.descriptor_pool->allocate( resource->descriptor_set_layout[ props->descriptor_set_id ] );
  resource->descriptor_set_id = props->descriptor_set_id;
  if( resource->descriptor_set_layout.size() > props->texture_descriptor_set_id ) {
    resource->texture_descriptor_set = props->allocator_set.descriptor_pool->allocate( resource->descriptor_set_layout[ props->texture_descriptor_set_id ], props->texture.max_texture_count );
    resource->texture_descriptor_set_id = props->texture_descriptor_set_id;
  }
  if( props->enable_linear && resource->descriptor_set_layout.size() > props->image_descriptor_set_id ) {
    resource->image_descriptor_set = props->allocator_set.descriptor_pool->allocate( resource->descriptor_set_layout[ props->image_descriptor_set_id ], props->image.max_image_count );
    resource->image_descriptor_set_id = props->image_descriptor_set_id;
  }
  if( resource->descriptor_set_layout.size() > props->vertex_buffer_descriptor_set_id ) {
    resource->vertex_buffer_descriptor_set = props->allocator_set.descriptor_pool->allocate( resource->descriptor_set_layout[ props->vertex_buffer_descriptor_set_id ], props->vertex.max_vertex_buffer_count );
    resource->vertex_buffer_descriptor_set_id = props->vertex_buffer_descriptor_set_id;
  }
  
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
      .set_allocator_set( props->allocator_set )
  ) );
  resource->aabb.reset( new aabb_pool(
    aabb_pool_create_info( props->aabb )
      .set_allocator_set( props->allocator_set )
      .set_matrix_pool( resource->matrix->get_buffer() )
  ) );
  resource->sampler.reset( new sampler_pool(
    sampler_pool_create_info( props->sampler )
      .set_allocator( props->allocator_set.allocator )
  ) );
  resource->csmat = allocate_color_space_matrix( *resource->matrix );
  image_pool_create_info ipci = props->image;
  ipci
    .set_allocator_set( props->allocator_set )
    .set_descriptor_set_layout( resource->descriptor_set_layout )
    .add_external_descriptor_set( props->descriptor_set_id, resource->descriptor_set )
    .add_external_descriptor_set( props->texture_descriptor_set_id, resource->texture_descriptor_set )
    .set_external_pipeline_layout( resource->pipeline_layout )
    .set_matrix_pool( resource->matrix->get_buffer() )
    .set_csmat( resource->csmat )
    .set_enable_linear( props->enable_linear );
  if( resource->descriptor_set_layout.size() > props->image_descriptor_set_id ) {
    ipci
      .set_image_descriptor_set_id( props->image_descriptor_set_id )
      .add_external_descriptor_set( props->image_descriptor_set_id, resource->image_descriptor_set );
  }
  if( resource->descriptor_set_layout.size() > props->texture_descriptor_set_id ) {
    ipci
      .add_external_descriptor_set( props->texture_descriptor_set_id, resource->texture_descriptor_set );
  }

  resource->image.reset( new image_pool( ipci ) );
  resource->texture.reset( new texture_pool(
    texture_pool_create_info( props->texture )
      .set_allocator( props->allocator_set.allocator )
      .set_descriptor_set( resource->texture_descriptor_set )
      .set_sampler( resource->sampler )
      .set_image( resource->image )
  ) );
  resource->primitive_resource_index.reset( new buffer_pool(
    buffer_pool_create_info( props->primitive_resource_index )
      .set_allocator_set( props->allocator_set )
  ) );
  resource->instance_resource_index.reset( new buffer_pool(
    buffer_pool_create_info( props->instance_resource_index )
      .set_allocator_set( props->allocator_set )
  ) );
  resource->visibility.reset( new buffer_pool(
    buffer_pool_create_info( props->visibility )
      .set_allocator_set( props->allocator_set )
  ) );
  if(
    std::filesystem::exists( props->light.read_shader ) &&
    std::filesystem::exists( props->light.write_shader ) &&
    std::filesystem::exists( props->light.update_shader )
  ) {
    resource->light.reset( new light_pool(
      light_pool_create_info( props->light )
        .set_allocator_set( props->allocator_set )
        .set_matrix_pool( resource->matrix->get_buffer() )
    ) );
  }
  if(
    std::filesystem::exists( props->accessor.read_shader ) &&
    std::filesystem::exists( props->accessor.write_shader )
  ) {
    resource->accessor.reset( new buffer_pool(
      buffer_pool_create_info( props->accessor )
        .set_allocator_set( props->allocator_set )
    ) );
  }
  if(
    std::filesystem::exists( props->mesh.read_shader ) &&
    std::filesystem::exists( props->mesh.write_shader )
  ) {
    resource->mesh.reset( new buffer_pool(
      buffer_pool_create_info( props->mesh )
        .set_allocator_set( props->allocator_set )
    ) );
  }
  if(
    std::filesystem::exists( props->meshlet.read_shader ) &&
    std::filesystem::exists( props->meshlet.write_shader )
  ) {
    resource->meshlet.reset( new buffer_pool(
      buffer_pool_create_info( props->meshlet )
        .set_allocator_set( props->allocator_set )
    ) );
  }
  if(
    std::filesystem::exists( props->meshlet_index.read_shader ) &&
    std::filesystem::exists( props->meshlet_index.write_shader )
  ) {
    resource->meshlet_index.reset( new buffer_pool(
      buffer_pool_create_info( props->meshlet_index )
        .set_allocator_set( props->allocator_set )
    ) );
  }
  if(
    std::filesystem::exists( props->resource_pair.read_shader ) &&
    std::filesystem::exists( props->resource_pair.write_shader )
  ) {
    resource->resource_pair.reset( new buffer_pool(
      buffer_pool_create_info( props->resource_pair )
        .set_allocator_set( props->allocator_set )
    ) );
  }
  resource->last_visibility = props->allocator_set.allocator->create_mappable_buffer(
    resource->visibility->get_buffer()->get_props().get_basic().size,
    use_conditional ?
      vk::BufferUsageFlagBits::eStorageBuffer|
      vk::BufferUsageFlagBits::eConditionalRenderingEXT :
      vk::BufferUsageFlagBits::eStorageBuffer
  );
  /*resource->resource_pair = props->allocator_set.allocator->create_mappable_buffer(
    resource->visibility->get_props().max_buffer_count * sizeof( raw_resource_pair_type ),
    vk::BufferUsageFlagBits::eStorageBuffer
  );*/
  auto vbpci =
    vertex_buffer_pool_create_info( props->vertex )
      .set_allocator_set( props->allocator_set )
      .set_descriptor_set_layout( resource->descriptor_set_layout )
      .set_external_pipeline_layout( resource->pipeline_layout );
  if( resource->descriptor_set_layout.size() > props->vertex_buffer_descriptor_set_id ) {
    vbpci
      .set_vertex_buffer_descriptor_set_id( props->vertex_buffer_descriptor_set_id )
      .add_external_descriptor_set( props->vertex_buffer_descriptor_set_id, resource->vertex_buffer_descriptor_set );
  }

  resource->vertex.reset( new vertex_buffer_pool( vbpci ) );
  std::vector< write_descriptor_set_t > u{
    { "primitive_resource_index", resource->primitive_resource_index->get_buffer() },
    { "instance_resource_index", resource->instance_resource_index->get_buffer() },
    { "visibility_pool", resource->visibility->get_buffer() },
    { "last_visibility_pool", resource->last_visibility->get_buffer() },
    { "matrix_pool", resource->matrix->get_buffer() },
    { "aabb_pool", resource->aabb->get_buffer() },
    { "resource_pair", resource->resource_pair->get_buffer() }
  };
  if( resource->light ) {
    u.push_back( { "light_pool", resource->light->get_buffer() } );
  }
  if( resource->accessor ) {
    u.push_back( { "accessor_pool", resource->accessor->get_buffer() } );
  }
  if( resource->mesh ) {
    u.push_back( { "mesh_pool", resource->mesh->get_buffer() } );
  }
  if( resource->meshlet ) {
    u.push_back( { "meshlet_pool", resource->meshlet->get_buffer() } );
  }
  if( resource->meshlet_index ) {
    u.push_back( { "meshlet_index_pool", resource->meshlet_index->get_buffer() } );
  }
  resource->descriptor_set->update( std::move( u ) );

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
  init_visibility = true;

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
  if( init_visibility ) {
    rec.fill( resource->visibility->get_buffer(), 0u );
    rec.barrier( {
      resource->visibility->get_buffer()
    }, {} );
    init_visibility = false;
  }
  syncable s;
  (*resource->matrix)( rec );
  rec.compute_barrier( { resource->matrix->get_buffer() }, {} );
  s.add( resource->matrix->get_buffer() );
  (*resource->aabb)( rec );
  s.add( resource->aabb->get_buffer() );
  (*resource->image)( rec );
  (*resource->sampler)();
  (*resource->texture)( rec );
  (*resource->visibility)( rec );
  s.add( resource->visibility->get_buffer() );
  (*resource->vertex)( rec );
  (*resource->primitive_resource_index)( rec );
  s.add( resource->primitive_resource_index->get_buffer() );
  (*resource->instance_resource_index)( rec );
  s.add( resource->instance_resource_index->get_buffer() );
  if( resource->light ) {
    (*resource->light)( rec );
    s.add( resource->light->get_buffer() );
  }
  if( resource->accessor ) {
    (*resource->accessor)( rec );
    s.add( resource->accessor->get_buffer() );
  }
  if( resource->mesh ) {
    (*resource->mesh)( rec );
    s.add( resource->mesh->get_buffer() );
  }
  if( resource->meshlet ) {
    (*resource->meshlet)( rec );
    s.add( resource->meshlet->get_buffer() );
  }
  if( resource->meshlet_index ) {
    (*resource->meshlet_index)( rec );
    s.add( resource->meshlet_index->get_buffer() );
  }
  if( resource->resource_pair ) {
    (*resource->resource_pair)( rec );
    s.add( resource->resource_pair->get_buffer() );
  }

  rec.compute_to_graphics_barrier( s );
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
void scene_graph::clear_visibility( command_buffer_recorder_t &rec ) const {
  rec.fill( resource->visibility->get_buffer(), 0u );
  rec.barrier( {
    resource->visibility->get_buffer()
  }, {} );
}
void scene_graph::fill_visibility( command_buffer_recorder_t &rec, std::uint32_t v ) const {
  rec.fill( resource->visibility->get_buffer(), v );
  rec.barrier( {
    resource->visibility->get_buffer()
  }, {} );
}

}

