#include <iostream>
#include <chrono>
#include <gct/gbuffer.hpp>
#include <gct/allocator.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/shader_module_reflection.hpp>
#include <gct/scene_graph.hpp>
#include <gct/compiled_scene_graph.hpp>
#include <gct/command_buffer.hpp>
#include <gct/instance_list.hpp>
#include <gct/nohc_benchmark.hpp>
#include <gct/primitive.hpp>
#include <gct/get_device.hpp>
#include <gct/device.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/buffer_pool.hpp>
#include <gct/image.hpp>
#include <gct/cubemap.hpp>
#include <gct/sampler.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/spv_member_pointer.hpp>
#include <gct/color.hpp>
#include <gct/gltf.hpp>

namespace gct {
nohc_benchmark::nohc_benchmark(
  const nohc_benchmark_create_info &ci
) : props( ci ) {
  gbuf = std::make_shared< gct::gbuffer >(
    gct::gbuffer_create_info()
      .set_allocator( props.allocator )
      .set_width( props.width )
      .set_height( props.height )
      .set_layer( 1u )
      .set_swapchain_image_count( 1u )
      .set_color_buffer_count( props.color_buffer_count )
      .set_format( vk::Format::eR32G32B32A32Sfloat ) 
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
  );
  sg = std::make_shared< gct::scene_graph::scene_graph >(
    gct::scene_graph::scene_graph_create_info()
      .set_allocator( props.allocator )
      .set_descriptor_pool( props.descriptor_pool )
      .set_pipeline_cache( props.pipeline_cache )
      .add_master_shader( props.shader / "geometry" )
      .set_shader( props.shader )
  );

  std::unique_ptr< shader_module_reflection_t > vs_reflection;
  for( const auto &f: std::filesystem::directory_iterator( props.shader / "geometry" ) ) {
    if( f.path().extension() == ".spv" ) {
      std::unique_ptr< shader_module_reflection_t > reflection( new shader_module_reflection_t( f.path() ) );
      if( (*reflection)->shader_stage & SPV_REFLECT_SHADER_STAGE_VERTEX_BIT ) {
        vs_reflection = std::move( reflection );
        break;
      }
    }
  }

  const auto [vistat,vamap,stride] = get_vertex_attributes(
    get_device( *props.allocator ),
    *vs_reflection
  );
  const auto [high_piasci,high_vertices,high_vertex_count] = primitive::create_sphere( vamap, stride, 120, 60 );
  const auto [low_piasci,low_vertices,low_vertex_count] = primitive::create_sphere( vamap, stride, 12, 6 );

  const auto pci = graphics_pipeline_create_info_t()
    .set_vertex_input( vistat )
    .set_input_assembly( high_piasci )
    .set_viewport(
      pipeline_viewport_state_create_info_t()
        .add_viewport( gbuf->get_viewport() )
        .add_scissor( gbuf->get_scissor() )
    )
    .set_rasterization(
      pipeline_rasterization_state_create_info_t()
        .set_basic(
          vk::PipelineRasterizationStateCreateInfo()
            .setDepthClampEnable( VK_FALSE )
            .setRasterizerDiscardEnable( VK_FALSE )
            .setPolygonMode( vk::PolygonMode::eFill )
            .setCullMode( vk::CullModeFlagBits::eBack )
            .setFrontFace( vk::FrontFace::eCounterClockwise )
            .setDepthBiasEnable( VK_FALSE )
            .setLineWidth( 1.0f )
        )
    )
    .set_multisample(
      pipeline_multisample_state_create_info_t()
        .set_basic(
          vk::PipelineMultisampleStateCreateInfo()
        )
    )
    .set_dynamic(
      pipeline_dynamic_state_create_info_t()
    )
    .set_layout( sg->get_resource()->pipeline_layout )
    .rebuild_chain();

  const auto high_vertex_buffer_desc = sg->get_resource()->vertex->allocate( high_vertices );
  const auto low_vertex_buffer_desc = sg->get_resource()->vertex->allocate( low_vertices );

  auto p = scene_graph::primitive()
    .set_vs_flag( shader_flag_t::vertex )
    .set_gs_flag( shader_flag_t::geometry )
    .set_fs_flag( shader_flag_t::fragment )
    .set_cull( true )
    .set_blend( false )
    .set_vertex_input_binding( vistat.get_vertex_input_binding_description() )
    .set_vertex_input_attribute( vistat.get_vertex_input_attribute_description() )
    .set_indexed( false )
    .set_roughness( 0.8f )
    .set_metalness( 0.0f )
    .emplace_emissive( 0.f, 0.f, 0.f, 1.f )
    .emplace_emissive( 1.f, 1.f, 1.f, 1.f )
    .set_normal_scale( 0.f )
    .set_occlusion_strength( 0.f )
    .set_aabb(
     aabb4()
       .emplace_min( -1.f, -1.f, -1.f, 1.f )
       .emplace_max(  1.f,  1.f,  1.f, 1.f )
    );
  
  auto p_rimp = sg->get_resource()->primitive_resource_index->get_member_pointer();
  std::vector< std::uint8_t > p_ri( p_rimp.get_aligned_size() );

  p_ri.data()->*p_rimp[ "roughness" ] = p.roughness;
  p_ri.data()->*p_rimp[ "metallic" ] = p.metalness;
  p_ri.data()->*p_rimp[ "emissive" ] = p.emissive;
  p_ri.data()->*p_rimp[ "base_color" ] = p.base_color;
  p_ri.data()->*p_rimp[ "normal_scale" ] = p.normal_scale;
  p_ri.data()->*p_rimp[ "occlusion_strength" ] = p.occlusion_strength;
  p_ri.data()->*p_rimp[ "base_color_texture" ] = 0u;
  p_ri.data()->*p_rimp[ "metallic_roughness_texture" ] = 0u;
  p_ri.data()->*p_rimp[ "normal_texture" ] = 0u;
  p_ri.data()->*p_rimp[ "occlusion_texture" ] = 0u;
  p_ri.data()->*p_rimp[ "emissive_texture" ] = 0u;

  p.descriptor.resource_index = sg->get_resource()->primitive_resource_index->allocate( p_ri.data(), std::next( p_ri.data(), p_ri.size() ) );
  p.descriptor.aabb = sg->get_resource()->aabb->allocate( p.aabb );
  p.set_pipeline_create_info( pci );
    
  auto high_p = std::make_shared< scene_graph::primitive >(
    scene_graph::primitive( p )
      .set_vertex_buffer(
        std::unordered_map< std::uint32_t, scene_graph::buffer_offset >{
          {
            0u,
            scene_graph::buffer_offset()
              .set_buffer( high_vertex_buffer_desc )
          }
        }
      )
      .set_count( high_vertex_count )
  );
  auto low_p = std::make_shared< scene_graph::primitive >(
    scene_graph::primitive( p )
      .set_vertex_buffer(
        std::unordered_map< std::uint32_t, scene_graph::buffer_offset >{
          {
            0u,
            scene_graph::buffer_offset()
              .set_buffer( low_vertex_buffer_desc )
          }
        }
      )
      .set_count( low_vertex_count )
  );

  auto high_p_desc = sg->get_resource()->prim.allocate( high_p );
  auto low_p_desc = sg->get_resource()->prim.allocate( low_p );

  auto node = sg->get_root_node();
  
  auto i_rimp = sg->get_resource()->instance_resource_index->get_member_pointer();

  auto high_i = std::make_shared< scene_graph::instance >();
  high_i->descriptor.set_prim( high_p_desc );
  high_i->descriptor.set_matrix( node->matrix );
  high_i->descriptor.set_aabb( sg->get_resource()->aabb->allocate( high_p->descriptor.aabb, node->matrix ) );
  high_i->descriptor.set_visibility( sg->get_resource()->visibility->allocate() );
  std::vector< std::uint8_t > high_i_ri( i_rimp.get_aligned_size() );
  high_i->initial_world_matrix = node->initial_world_matrix;
  high_i->initial_world_aabb = node->initial_world_matrix * high_p->aabb;
  high_i_ri.data()->*i_rimp[ "world_matrix" ] = *high_i->descriptor.matrix;
  high_i_ri.data()->*i_rimp[ "aabb" ] = *high_i->descriptor.aabb;
  high_i_ri.data()->*i_rimp[ "visibility" ] = *high_i->descriptor.visibility;
  high_i->descriptor.resource_index = sg->get_resource()->instance_resource_index->allocate( high_i_ri.data(), std::next( high_i_ri.data(), high_i_ri.size() ) );
  high_i_desc = sg->get_resource()->inst.allocate( high_i );
  node->inst.push_back( high_i_desc );
  
  auto low_i = std::make_shared< scene_graph::instance >();
  low_i->descriptor.set_prim( low_p_desc );
  low_i->descriptor.set_matrix( node->matrix );
  low_i->descriptor.set_aabb( sg->get_resource()->aabb->allocate( low_p->descriptor.aabb, node->matrix ) );
  low_i->descriptor.set_visibility( sg->get_resource()->visibility->allocate() );
  std::vector< std::uint8_t > low_i_ri( i_rimp.get_aligned_size() );
  low_i->initial_world_matrix = node->initial_world_matrix;
  low_i->initial_world_aabb = node->initial_world_matrix * low_p->aabb;
  low_i_ri.data()->*i_rimp[ "world_matrix" ] = *low_i->descriptor.matrix;
  low_i_ri.data()->*i_rimp[ "aabb" ] = *low_i->descriptor.aabb;
  low_i_ri.data()->*i_rimp[ "visibility" ] = *low_i->descriptor.visibility;
  low_i->descriptor.resource_index = sg->get_resource()->instance_resource_index->allocate( low_i_ri.data(), std::next( low_i_ri.data(), low_i_ri.size() ) );
  low_i_desc = sg->get_resource()->inst.allocate( low_i );
  node->inst.push_back( low_i_desc );

  csg = std::make_shared< gct::scene_graph::compiled_scene_graph >(
    gct::scene_graph::compiled_scene_graph_create_info()
      .set_shader( props.shader / "geometry" )
      .set_render_pass( gbuf->get_render_pass() )
      .set_dynamic_cull_mode( false ),
    *sg
  );
  
  il = std::make_shared< gct::scene_graph::instance_list >(
    gct::scene_graph::instance_list_create_info(),
    *sg
  );
  const auto global_descriptor_set_layout = get_device( *props.allocator ).get_descriptor_set_layout(
    std::vector< std::filesystem::path >{
      props.shader / "geometry"
    },
    1u
  );
  global_uniform =
    props.allocator->create_mappable_buffer(
      sizeof( gct::gltf::dynamic_uniforms_t ),
      vk::BufferUsageFlagBits::eUniformBuffer
    );
  global_descriptor_set =
    props.descriptor_pool->allocate(
      global_descriptor_set_layout
    );
  const unsigned int shadow_map_size = 1024u;
  shadow = std::make_shared< gbuffer >(
    gbuffer_create_info()
      .set_allocator( props.allocator )
      .set_width( shadow_map_size )
      .set_height( shadow_map_size )
      .set_layer( 6 )
      .set_swapchain_image_count( 1u )
      .set_color_buffer_count( 1 )
      .set_flags( vk::ImageCreateFlagBits::eCubeCompatible )
      .set_format( vk::Format::eR32G32Sfloat ) 
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
      .set_clear_color( gct::color::web::white )
  );
  gct::cubemap_images2 cubemap_images( shadow->get_image_views() );
  cubemap_sampler = get_device( *props.allocator ).get_sampler(
    gct::get_basic_linear_sampler_create_info()
  );
  global_descriptor_set->update({
    { "global_uniforms", global_uniform },
    { "shadow", cubemap_sampler, cubemap_images.get_cube_image_views()[ 0 ], vk::ImageLayout::eShaderReadOnlyOptimal }
  });
  
  oq = std::make_shared< gct::occlusion_query< unsigned int > >(
    gct::occlusion_query_create_info()
      .set_allocator( props.allocator )
      .set_descriptor_pool( props.descriptor_pool )
      .set_pipeline_cache( props.pipeline_cache )
      .set_color_attachment_count( 0u )
      .set_depth_image( gbuf->get_depth_views()[ 0 ] )
      .set_shader( props.shader / "aabb" )
      .set_dynamic_cull_mode( false )
      .set_max_query_count( 1000 )
  );

}

void nohc_benchmark::draw(
  command_buffer_recorder_t &rec,
  float scale,
  bool high,
  const std::shared_ptr< gct::scene_graph::instance_list > &view_il
) {
  {
    rec.bind_descriptor_set(
      vk::PipelineBindPoint::eGraphics,
      1u,
      sg->get_resource()->pipeline_layout,
      global_descriptor_set
    );
    auto render_pass_token = rec.begin_render_pass(
      gbuf->get_render_pass_begin_info( 0 ),
      vk::SubpassContents::eInline
    );
    (*view_il)( rec, *csg );
  }
}
std::shared_ptr< gct::scene_graph::instance_list > nohc_benchmark::setup(
  command_buffer_recorder_t &rec,
  float scale,
  bool high
) {
  (*sg)( rec ); 
  const auto global_data = gct::gltf::dynamic_uniforms_t()
    .set_projection_matrix( glm::mat4(
      1.f, 0.f, 0.f, 0.f,
      0.f, 1.f, 0.f, 0.f,
      0.f, 0.f, 1.f, 0.f,
      0.f, 0.f, 0.f, 1.f
    ) )
    .set_camera_matrix( glm::mat4(
      scale, 0.f, 0.f, 0.f,
      0.f, scale, 0.f, 0.f,
      0.f, 0.f, scale, 0.f,
      0.f, 0.f, 0.f, 1.f
    ) )
    .emplace_eye_pos( 0.f, 0.f, -2.f, 1.f )
    .emplace_light_pos( 0.f, 2.f, 0.f, 1.f )
    .set_light_energy( 5.f )
    .set_light_size( 3.f )
    .set_ambient( 0.1f )
    .set_inversed_voxel( glm::mat4(
      1.f, 0.f, 0.f, 0.f,
      0.f, 1.f/scale, 0.f, 0.f,
      0.f, 0.f, 1.f/scale, 0.f,
      0.f, 0.f, 0.f, 1.f
    ) );
  rec.copy( global_data, global_uniform );
  rec.transfer_to_graphics_barrier( { global_uniform->get_buffer() }, {} );
  shadow->clear_color( rec, 0 );
  rec.convert_image( shadow->get_image( 0 ), vk::ImageLayout::eShaderReadOnlyOptimal );
  auto view_il = std::make_shared< gct::scene_graph::instance_list >(
    *il
  );
  auto dl = view_il->get_draw_list();

  const auto i_desc = high ? high_i_desc : low_i_desc;

  auto i = std::find_if( dl.begin(), dl.end(), [&]( const auto &v ) { return *v.inst == *i_desc; } );
  view_il->get_draw_list() = std::vector< scene_graph::resource_pair >{ *i };
  return view_il;
}

void nohc_benchmark::operator()(
  std::shared_ptr< bound_command_buffer_t > &command_buffer
) {
  std::shared_ptr< gct::scene_graph::instance_list > view_il;
  {
    {
      auto rec = command_buffer->begin();
      view_il = setup( rec, 0.0001f, true );
    }
    command_buffer->execute_and_wait();
  }
  {
    {
      auto rec = command_buffer->begin();
      draw( rec, 0.0001f, true, view_il );
    }
    const auto begin = std::chrono::high_resolution_clock::now();
    command_buffer->execute_and_wait();
    const auto end = std::chrono::high_resolution_clock::now();
    highv_lowp = std::chrono::duration_cast< std::chrono::nanoseconds >( end - begin );
  }
  {
    {
      auto rec = command_buffer->begin();
      view_il = setup( rec, 0.0001f, false );
    }
    command_buffer->execute_and_wait();
  }
  {
    {
      auto rec = command_buffer->begin();
      draw( rec, 0.0001f, false, view_il );
    }
    const auto begin = std::chrono::high_resolution_clock::now();
    command_buffer->execute_and_wait();
    const auto end = std::chrono::high_resolution_clock::now();
    lowv_lowp = std::chrono::duration_cast< std::chrono::nanoseconds >( end - begin );
  }
  {
    {
      auto rec = command_buffer->begin();
      view_il = setup( rec, 1.0f, false );
    }
    command_buffer->execute_and_wait();
  }
  {
    {
      auto rec = command_buffer->begin();
      draw( rec, 1.f, false, view_il );
    }
    const auto begin = std::chrono::high_resolution_clock::now();
    command_buffer->execute_and_wait();
    const auto end = std::chrono::high_resolution_clock::now();
    lowv_highp = std::chrono::duration_cast< std::chrono::nanoseconds >( end - begin );
  }
  {
    {
      auto rec = command_buffer->begin();
      view_il = setup( rec, 1.0f, false );
    }
    command_buffer->execute_and_wait();
  }
  std::uniform_real_distribution<float> dist( -5.0f, 5.0f );
  std::mt19937 engine( std::random_device{}() );
  {
    oq->reset();
    oq->push(
      aabb4()
        .emplace_min( -1.f, -1.f, -1.f, 1.f )
        .emplace_min(  1.f,  1.f,  1.f, 1.f ),
      0u
    );
    const auto begin = std::chrono::high_resolution_clock::now();
    {
      {
        auto rec = command_buffer->begin();
        (*oq)(
          rec,
          glm::mat4(
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f
          )
        );
      }
    }
    command_buffer->execute_and_wait();
    std::vector< unsigned int > result;
    oq->get_result( result );
    const auto end = std::chrono::high_resolution_clock::now();
    oq1_highp = std::chrono::duration_cast< std::chrono::nanoseconds >( end - begin );
  }
  {
    oq->reset();
    oq->push(
      aabb4()
        .emplace_min( -0.0001f, -0.0001f, -0.0001f, 1.f )
        .emplace_min(  0.0001f,  0.0001f,  0.0001f, 1.f ),
      0u
    );
    const auto begin = std::chrono::high_resolution_clock::now();
    {
      {
        auto rec = command_buffer->begin();
        (*oq)(
          rec,
          glm::mat4(
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f
          )
        );
      }
    }
    command_buffer->execute_and_wait();
    std::vector< unsigned int > result;
    oq->get_result( result );
    const auto end = std::chrono::high_resolution_clock::now();
    oq1_lowp = std::chrono::duration_cast< std::chrono::nanoseconds >( end - begin );
  }
  {
    oq->reset();
    for( unsigned int i = 0u; i!= 1000u; ++i ) {
      oq->push(
        aabb4()
          .emplace_min( -0.0001f, -0.0001f, -0.0001f, 1.f )
          .emplace_min(  0.0001f,  0.0001f,  0.0001f, 1.f ),
        0u
      );
    }
    const auto begin = std::chrono::high_resolution_clock::now();
    {
      {
        auto rec = command_buffer->begin();
        (*oq)(
          rec,
          glm::mat4(
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f
          )
        );
      }
    }
    command_buffer->execute_and_wait();
    std::vector< unsigned int > result;
    oq->get_result( result );
    const auto end = std::chrono::high_resolution_clock::now();
    oq1000 = std::chrono::duration_cast< std::chrono::nanoseconds >( end - begin );
  }
  const float tos_tangent = ( oq1000.count() - oq1_lowp.count() )/999.f;
  tos = oq1000.count() - tos_tangent * 1000.f;
  top = ( ( oq1_highp.count() - tos ) - ( oq1_lowp.count() - tos ) )/( 0.5f*0.5f*M_PI - 0.00005f*0.00005f*M_PI );

  trv = ( highv_lowp.count() - lowv_lowp.count() )/( 120.f*60.f - 12.f*6.f );
  trs = highv_lowp.count() - trv * 120.f*60.f;
  trf = ( lowv_highp.count() - lowv_lowp.count() )/( 0.5f*0.5f*M_PI - 0.00005f*0.00005f*M_PI );
}

}
