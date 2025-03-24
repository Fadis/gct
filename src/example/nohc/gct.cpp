#include <iostream>
#include <algorithm>
#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <gct/aabb.hpp>
#include <gct/get_extensions.hpp>
#include <gct/instance.hpp>
#include <gct/glfw.hpp>
#include <gct/queue.hpp>
#include <gct/device.hpp>
#include <gct/allocator.hpp>
#include <gct/device_create_info.hpp>
#include <gct/buffer.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/image_create_info.hpp>
#include <gct/image.hpp>
#include <gct/swapchain.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/sampler_create_info.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/pipeline_layout_create_info.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/buffer_view_create_info.hpp>
#include <gct/compute_pipeline_create_info.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/render_pass_begin_info.hpp>
#include <gct/submit_info.hpp>
#include <gct/shader_module.hpp>
#include <gct/wait_for_sync.hpp>
#include <gct/present_info.hpp>
#include <gct/color.hpp>
#include <gct/timer.hpp>
#include <gct/cubemap.hpp>
#include <gct/gbuffer.hpp>
#include <gct/hysteresis.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_pool.hpp>
#include <gct/framebuffer.hpp>
#include <gct/render_pass.hpp>
#include <gct/cubemap_matrix_generator.hpp>
#include <gct/image_filter.hpp>
#include <gct/named_resource.hpp>
#include <gct/compute.hpp>
#include <gct/common_sample_setup.hpp>
#include <gct/tone_mapping.hpp>
#include <gct/hbao.hpp>
#include <gct/scene_graph.hpp>
#include <gct/gltf.hpp>
#include <gct/gltf2.hpp>
#include <gct/gltf2_create_info.hpp>
#include <gct/image_pool.hpp>
#include <gct/compiled_scene_graph.hpp>
#include <gct/compiled_aabb_scene_graph.hpp>
#include <gct/instance_list.hpp>
#include <gct/kdtree.hpp>
#include <gct/skyview.hpp>
#include <gct/skyview_froxel.hpp>
#include <gct/occlusion_query.hpp>
#include <gct/nohc_benchmark.hpp>
#include <gct/chc.hpp>
#include <gct/nohc.hpp>
#include <vulkan/vulkan_structs.hpp>

struct fb_resources_t {
  std::shared_ptr< gct::semaphore_t > image_acquired;
  std::shared_ptr< gct::semaphore_t > draw_complete;
  std::shared_ptr< gct::semaphore_t > image_ownership;
  std::shared_ptr< gct::bound_command_buffer_t > command_buffer;
  bool initial = true;
};

int main( int argc, const char *argv[] ) {
  const gct::common_sample_setup res(
    argc, argv,
    {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME,
      VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME,
      VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME,
      VK_KHR_MAINTENANCE1_EXTENSION_NAME,
      VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME,
      VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME
    },
    gct::descriptor_pool_create_info_t()
      .set_basic(
        vk::DescriptorPoolCreateInfo()
          .setFlags(
            vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet|
            vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind
          )
          .setMaxSets( 65536 * 5 )
      )
      .set_descriptor_pool_size( vk::DescriptorType::eUniformBuffer, 16 )
      .set_descriptor_pool_size( vk::DescriptorType::eStorageBuffer, 65536*2 )
      .set_descriptor_pool_size( vk::DescriptorType::eCombinedImageSampler, 65536*2 )
      .set_descriptor_pool_size( vk::DescriptorType::eStorageImage, 16 )
      .rebuild_chain()
  );

  auto command_buffer = res.queue->get_command_pool()->allocate();
  gct::nohc_parameter nohc_param;
  {
    gct::nohc_benchmark nohc_benchmark{
      gct::nohc_benchmark_create_info()
        .set_allocator( res.allocator )
        .set_descriptor_pool( res.descriptor_pool )
         .set_pipeline_cache( res.pipeline_cache )
         .set_shader( CMAKE_CURRENT_BINARY_DIR ) 
         .set_width( res.width )
         .set_height( res.height )
         .set_color_buffer_count( 8u )
    };
    nohc_benchmark( command_buffer );
    nohc_param = nohc_benchmark.get();
  }


  gct::cubemap_matrix_generator shadow_mat(
    res.allocator,
    res.descriptor_pool,
    res.pipeline_cache,
    CMAKE_CURRENT_BINARY_DIR "/shadow_mat/shadow_mat.comp.spv",
    1u
  );

  const unsigned int shadow_map_size = 1024u;
  gct::gbuffer shadow_gbuffer(
    gct::gbuffer_create_info()
      .set_allocator( res.allocator )
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

  gct::cubemap_images2 cubemap_images( shadow_gbuffer.get_image_views() );

  auto cubemap_sampler = res.device->get_sampler(
    gct::get_basic_linear_sampler_create_info()
  );
  
  gct::gbuffer depth_gbuffer(
    gct::gbuffer_create_info()
      .set_allocator( res.allocator )
      .set_width( res.width )
      .set_height( res.height )
      .set_layer( 1u )
      .set_swapchain_image_count( 1u )
      .set_color_buffer_count( 0u )
      .set_format( vk::Format::eR32G32B32A32Sfloat ) 
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
  );

  constexpr std::size_t gbuf_count = 8u;
  gct::gbuffer gbuffer(
    gct::gbuffer_create_info()
      .set_allocator( res.allocator )
      .set_width( res.width )
      .set_height( res.height )
      .set_layer( 1u )
      .set_swapchain_image_count( 1u )
      .set_color_buffer_count( gbuf_count )
      .set_format( vk::Format::eR32G32B32A32Sfloat ) 
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
      .set_external_depth( depth_gbuffer.get_depth_views() )
  );

  const auto sg = std::make_shared< gct::scene_graph::scene_graph >(
    gct::scene_graph::scene_graph_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .add_master_shader( CMAKE_CURRENT_BINARY_DIR "/shadow" )
      .add_master_shader( CMAKE_CURRENT_BINARY_DIR "/geometry" )
      .add_master_shader( CMAKE_CURRENT_BINARY_DIR "/depth" )
      .set_shader( CMAKE_CURRENT_BINARY_DIR )
  );

  gct::gltf::gltf2 doc(
    gct::gltf::gltf2_create_info()
      .set_filename( res.model_filename )
      .set_graph( sg )
      .set_root( sg->get_root_node() )
      .set_aspect_ratio( float( res.width )/float( res.height ) )
  );

  const auto geometry_csg = std::make_shared< gct::scene_graph::compiled_scene_graph >(
    gct::scene_graph::compiled_scene_graph_create_info()
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/geometry" )
      .set_render_pass( gbuffer.get_render_pass() )
      .set_dynamic_cull_mode( true ),
    *sg
  );
  
  const auto depth_csg = std::make_shared< gct::scene_graph::compiled_scene_graph >(
    gct::scene_graph::compiled_scene_graph_create_info()
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/depth" )
      .set_render_pass( depth_gbuffer.get_render_pass() )
      .set_dynamic_cull_mode( true ),
    *sg
  );

  const auto shadow_csg = std::make_shared< gct::scene_graph::compiled_scene_graph >(
    gct::scene_graph::compiled_scene_graph_create_info()
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/shadow" )
      .set_render_pass( shadow_gbuffer.get_render_pass() )
      .set_dynamic_cull_mode( true ),
    *sg
  );

  const auto il = std::make_shared< gct::scene_graph::instance_list >(
    gct::scene_graph::instance_list_create_info(),
    *sg
  );
  
  auto occlusion_query = gct::occlusion_query< gct::kdtree< gct::scene_graph::resource_pair, gct::scene_graph::nohc_node_data > >(
    gct::occlusion_query_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_color_attachment_count( 0u )
      .set_depth_image( depth_gbuffer.get_depth_views()[ 0 ] )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/aabb" )
      .set_dynamic_cull_mode( true )
      .set_max_query_count( il->get_draw_list().size() )
  );
  auto draw_aabb = gct::basic_occlusion_query(
    gct::occlusion_query_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_color_attachment_count( 1u )
      .set_depth_image( depth_gbuffer.get_depth_views()[ 0 ] )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/aabb_test" )
      .set_dynamic_cull_mode( true )
      .set_max_query_count( il->get_draw_list().size() )
      .set_query( false )
  );

  gct::kdtree< gct::scene_graph::resource_pair, gct::scene_graph::nohc_node_data > full_kd;
  gct::scene_graph::append(
    *il->get_resource(),
    il->get_draw_list(),
    full_kd
  );
  auto view_il = std::make_shared< gct::scene_graph::instance_list >(
    *il
  );
  auto last_visible_il = std::make_shared< gct::scene_graph::instance_list >(
    *il
  );
  
  {
    {
      auto recorder = command_buffer->begin();
      (*sg)( recorder );
    }
    command_buffer->execute_and_wait();
  }
  const auto global_descriptor_set_layout = res.device->get_descriptor_set_layout(
    {
      CMAKE_CURRENT_BINARY_DIR "/geometry",
      CMAKE_CURRENT_BINARY_DIR "/shadow",
      CMAKE_CURRENT_BINARY_DIR "/depth"
    },
    1u
  );
  std::shared_ptr< gct::mappable_buffer_t > global_uniform;
  std::shared_ptr< gct::descriptor_set_t > global_descriptor_set;
  global_uniform =
    res.allocator->create_mappable_buffer(
      sizeof( gct::gltf::dynamic_uniforms_t ),
      vk::BufferUsageFlagBits::eUniformBuffer
    );
  global_descriptor_set =
    res.descriptor_pool->allocate(
      global_descriptor_set_layout
    );
  global_descriptor_set->update({
    { "global_uniforms", global_uniform },
    { "shadow", cubemap_sampler, cubemap_images.get_cube_image_views()[ 0 ], vk::ImageLayout::eShaderReadOnlyOptimal }
  });

  const auto rgba32ici =
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( res.width, res.height )
          .setUsage( vk::ImageUsageFlagBits::eStorage )
      );

  std::vector< fb_resources_t > framebuffers;
  for( std::size_t i = 0u; i != res.swapchain_images.size(); ++i ) {
    framebuffers.emplace_back(
      fb_resources_t{
        res.device->get_semaphore(),
        res.device->get_semaphore(),
        res.device->get_semaphore(),
        res.queue->get_command_pool()->allocate()
      }
    );
  }
  
  const auto diffuse = res.allocator->create_image_view(
    rgba32ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  const auto specular = res.allocator->create_image_view(
    rgba32ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );

  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( diffuse, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( specular, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }

  const auto lighting = gct::compute(
    gct::compute_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/lighting/lighting.comp.spv" )
      .set_swapchain_image_count( 1u )
      .add_resource( { "gbuffer", gbuffer.get_image_views(), vk::ImageLayout::eGeneral } )
      .add_resource( { "diffuse_image", diffuse } )
      .add_resource( { "specular_image", specular } )
      .add_resource( { "dynamic_uniforms", global_uniform } )
  );

  gct::hbao hbao(
    gct::hbao_create_info()
      .set_allocator( res.allocator )
      .set_pipeline_cache( res.pipeline_cache )
      .set_descriptor_pool( res.descriptor_pool )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/ao/" )
      .set_input_name( "gbuffer" )
      .set_input( gbuffer.get_image_views() )
      .add_resource( { "dynamic_uniforms", global_uniform } ) 
  );

  gct::skyview skyview(
    gct::skyview_create_info()
      .set_allocator( res.allocator )
      .set_pipeline_cache( res.pipeline_cache )
      .set_descriptor_pool( res.descriptor_pool )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/skyview/" )
  );
  const auto skyview_param = gct::skyview_parameter();
  
  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto rec = command_buffer->begin();
      skyview( rec, skyview_param );
    }
    command_buffer->execute_and_wait();
  }
  
  const auto skyview_froxel = gct::skyview_froxel(
    gct::skyview_froxel_create_info()
      .set_allocator( res.allocator )
      .set_pipeline_cache( res.pipeline_cache )
      .set_descriptor_pool( res.descriptor_pool )
      .set_width( res.width )
      .set_height( res.height )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/skyview/" )
      .set_gbuffer( gbuffer.get_image_view( 0 ) )
      .set_transmittance( skyview.get_transmittance() )
      .set_multiscat( skyview.get_multiscat() )
  );

  const auto mixed_out = res.allocator->create_image_view(
    rgba32ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );

  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( mixed_out, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }

  const gct::tone_mapping tone(
    gct::tone_mapping_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/tone/tone.comp.spv" )
      .set_input( std::vector< std::shared_ptr< gct::image_view_t > >{ mixed_out } )
  );
  
  auto linear_sampler = res.device->get_sampler(
    gct::get_basic_linear_sampler_create_info()
  );

  const auto bloom_gauss = gct::image_filter(
    gct::image_filter_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/mix_ao/mix_ao.comp.spv" )
      .set_input_name( "diffuse_image" )
      .set_input( std::vector< std::shared_ptr< gct::image_view_t > >{ diffuse } )
      .set_output_name( "bloom_image" )
      .add_resource( { "gbuffer", gbuffer.get_image_views(), vk::ImageLayout::eGeneral } )
      .add_resource( { "occlusion", hbao.get_output(), vk::ImageLayout::eGeneral } )
      .add_resource( { "skyview", linear_sampler, skyview.get_output(), vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "scattering", skyview_froxel.get_output(), vk::ImageLayout::eGeneral } )
      .add_resource( { "specular_image", specular } )
      .add_resource( { "dest_image", mixed_out } )
      .add_resource( { "tone", tone.get_buffer() } )
      .add_resource( { "aabb", draw_aabb.get_image_view(), vk::ImageLayout::eGeneral } )
      .add_resource( { "dynamic_uniforms", global_uniform } )
  )(
    gct::image_filter_create_info()
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/gauss/h12_32.comp.spv" )
  )(
    gct::image_filter_create_info()
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/gauss/v12_32.comp.spv" )
  );
  
  const auto gamma = gct::image_filter(
    gct::image_filter_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/gamma/gamma.comp.spv" )
      .set_input( std::vector< std::shared_ptr< gct::image_view_t > >( res.swapchain_images.size(), mixed_out ) )
      .set_output( res.swapchain_image_views )
      .add_resource( { "bloom_image", bloom_gauss.get_output(), vk::ImageLayout::eGeneral } )
  );

  const auto scene_aabb = sg->get_root_node()->get_initial_world_aabb();
  if( !scene_aabb ) throw -1;

  const auto center = ( scene_aabb->min + scene_aabb->max ) / 2.f;
  const auto scale = std::abs( glm::length( scene_aabb->max - scene_aabb->min ) );

  const auto znear = std::min(0.1f*scale,0.5f);
  const glm::mat4 projection = glm::perspective( 0.6981317007977318f, (float(res.width)/float(res.height)), znear, scale );
  const float light_size = 0.3;

  std::cout << scene_aabb->min[ 0 ] << " " << scene_aabb->min[ 1 ] << " " << scene_aabb->min[ 2 ] << std::endl;
  std::cout << scene_aabb->max[ 0 ] << " " << scene_aabb->max[ 1 ] << " " << scene_aabb->max[ 2 ] << std::endl;

  gct::glfw_walk walk( center, scale, res.walk_state_filename );
  res.window->set_on_key(
    [&walk]( gct::glfw_window &p, int key, int scancode, int action, int mods ) {
      walk( p, key, scancode, action, mods );
    }
  );
  res.window->set_on_closed(
    [&walk]( auto & ) {
      walk.set_end();
    }
  );

  std::shared_ptr< gct::mappable_buffer_t > shadow_uniform;
  std::shared_ptr< gct::descriptor_set_t > shadow_descriptor_set;
  shadow_uniform =
    res.allocator->create_mappable_buffer(
      sizeof( gct::gltf::dynamic_uniforms_t ),
      vk::BufferUsageFlagBits::eUniformBuffer
    );
  shadow_descriptor_set =
    res.descriptor_pool->allocate(
      global_descriptor_set_layout
    );
  shadow_descriptor_set->update({
    { "global_uniforms", shadow_uniform },
    { "matrices", shadow_mat.get_buffer()[ 0 ] }
  });

  std::vector< gct::scene_graph::resource_pair > last_visible;
  std::vector< gct::scene_graph::resource_pair > visible;
  gct::kdtree< gct::scene_graph::resource_pair, gct::scene_graph::nohc_node_data > visible_tree;
  std::unordered_set< unsigned int > visible_set;
  std::unordered_set< unsigned int > invisible_set;
  std::uint32_t current_frame = 0u;
  std::uint32_t frame_counter = 0u;
  while( !walk.end() ) {
    gct::blocking_timer frame_rate;
    ++walk;
    const auto global_data = gct::gltf::dynamic_uniforms_t()
      .set_projection_matrix( projection )
      .set_camera_matrix( walk.get_lookat() )
      .set_eye_pos( glm::vec4( walk.get_camera_pos(), 1.0 ) )
      .set_light_pos( glm::vec4( walk.get_light_pos(), 1.0 ) )
      .set_light_energy( walk.get_light_energy() )
      .set_light_size( light_size )
      .set_ambient( res.ambient_level )
      .set_inversed_voxel( glm::inverse( projection * walk.get_lookat() ) );
    const auto camera0_data = gct::gltf::dynamic_uniforms_t()
      .set_projection_matrix( projection )
      .set_camera_matrix( walk.get_camera()[ 0 ].get_lookat() )
      .set_eye_pos( glm::vec4( walk.get_camera()[ 0 ].get_camera_pos(), 1.0 ) )
      .set_light_pos( glm::vec4( walk.get_light_pos(), 1.0 ) )
      .set_light_energy( walk.get_light_energy() )
      .set_light_size( light_size )
      .set_ambient( res.ambient_level )
      .set_inversed_voxel( glm::inverse( projection * walk.get_camera()[ 0 ].get_lookat() ) );
    // frustum culling
    if( walk.get_current_camera() == 0 ) {
      if( walk.camera_moved() ) {
        visible_tree.clear();
        gct::frustum_culling(
          projection,
          walk.get_camera()[ 0 ].get_lookat(),
          full_kd,
          visible_tree,
          8u
        );
        gct::scene_graph::mark_node_visibility( visible_tree, visible_set );
        if( last_visible_il->get_draw_list().empty() ) {
          visible.clear();
          visible_tree.get( visible );
          last_visible_il->get_draw_list() = visible;
        }
      }
    }
    // depth
    if( walk.camera_moved() ) {
      {
        auto rec = command_buffer->begin();
        rec.copy( global_data, global_uniform );
        rec.transfer_to_graphics_barrier( { global_uniform->get_buffer() }, {} );
        {
          auto render_pass_token = rec.begin_render_pass(
            depth_gbuffer.get_render_pass_begin_info( 0 ),
            vk::SubpassContents::eInline
          );
          rec->setViewport( 0, 1, &depth_gbuffer.get_viewport() );
          rec->setScissor( 0, 1, &depth_gbuffer.get_scissor() );
          rec->setCullMode( vk::CullModeFlagBits::eBack );
          rec->setDepthCompareOp( vk::CompareOp::eLessOrEqual );
          rec.bind_descriptor_set(
            vk::PipelineBindPoint::eGraphics,
            1u,
            sg->get_resource()->pipeline_layout,
            global_descriptor_set
          );
          (*last_visible_il)( rec, *depth_csg );
        }
      }
      command_buffer->execute_and_wait();
    }
    // occlusion query
    if( walk.get_current_camera() == 0 ) {
      if( walk.camera_moved() ) {
        gct::scene_graph::build_invisible_set( visible_tree, visible_set, invisible_set );
        gct::scene_graph::calculate_poccl(
          visible_tree,
          visible_set,
          invisible_set,
          walk.get_camera()[ 0 ].get_camera_pos(),
          res.width,
          res.height,
          0.6981317007977318f
        );
        std::vector< gct::kdtree< gct::scene_graph::resource_pair, gct::scene_graph::nohc_node_data > > candidate;
        const unsigned int n = visible_tree.size();
        gct::scene_graph::get_visibility_unknown_node( { visible_tree }, candidate, visible );
        while( !candidate.empty() ) {
          std::vector< gct::kdtree< gct::scene_graph::resource_pair, gct::scene_graph::nohc_node_data > > not_occluded;
          gct::scene_graph::nohc_(
            candidate,
            *view_il,
            nohc_param,
            not_occluded,
            visible,
            occlusion_query,
            walk.get_camera()[ 0 ].get_camera_pos(),
            res.width,
            res.height,
            0.6981317007977318f,
            znear,
            n
          );
          if( !occlusion_query.empty() ) {
            {
              {
                auto rec = command_buffer->begin();
                rec.copy( camera0_data, global_uniform );
                rec.transfer_to_graphics_barrier( { global_uniform->get_buffer() }, {} );
                rec.barrier( {}, { depth_gbuffer.get_depth_views()[ 0 ]->get_factory() } );
                rec->setCullMode( vk::CullModeFlagBits::eBack );
                rec->setDepthCompareOp( vk::CompareOp::eLessOrEqual );
                occlusion_query( rec, projection * walk.get_camera()[ 0 ].get_lookat() );
              }
              command_buffer->execute_and_wait();
            }
            occlusion_query.get_result( not_occluded );
          }
          candidate = std::move( not_occluded );
        }
        view_il->get_draw_list() = std::move( visible );
        visible.clear();
      }
    }
    // rendering
    {
      {
        auto rec = command_buffer->begin();
        // aabb
        if( walk.camera_moved() || walk.get_grid_changed() || frame_counter <= 2u ) {
          draw_aabb.reset();
          if( walk.get_grid() ) {
            std::unordered_set< unsigned int > visible_set;
            for( std::uint32_t i = 0u; i != view_il->get_draw_list().size(); ++i ) {
              visible_set.insert( *view_il->get_draw_list()[ i ].inst ); 
            }
            for( std::uint32_t i = 0u; i != il->get_draw_list().size(); ++i ) {
              if( visible_set.find( *il->get_draw_list()[ i ].inst ) == visible_set.end() ) {
                const auto inst = il->get_resource()->inst.get( il->get_draw_list()[ i ].inst );
                draw_aabb.push( inst->initial_world_aabb );
              }
            }
          }
          if( walk.get_grid() || walk.get_grid_changed() || frame_counter <= 2u ) {
            rec.copy( global_data, global_uniform );
            rec.transfer_to_graphics_barrier( { global_uniform->get_buffer() }, {} );
            rec.barrier( {}, { depth_gbuffer.get_depth_views()[ 0 ]->get_factory() } );
            rec->setCullMode( vk::CullModeFlagBits::eBack );
            rec->setDepthCompareOp( vk::CompareOp::eAlways );
            rec.convert_image(
              draw_aabb.get_image_view()->get_factory(),
              vk::ImageLayout::eColorAttachmentOptimal
            );
            draw_aabb( rec, projection * walk.get_lookat() );
            rec.convert_image(
              draw_aabb.get_image_view()->get_factory(),
              vk::ImageLayout::eGeneral
            );
          }
        }
        tone.set( rec, 0 );
        if( walk.light_moved() ) {
          const auto shadow_data = gct::gltf::dynamic_uniforms_t()
            .set_eye_pos( glm::vec4( walk.get_camera_pos(), 1.0 ) )
            .set_light_pos( glm::vec4( walk.get_light_pos(), 1.0 ) )
            .set_light_energy( walk.get_light_energy() )
            .set_light_size( light_size )
            .set_ambient( res.ambient_level );
          shadow_mat( rec, walk.get_light_pos(), std::min( 0.1f*scale, 0.5f ), 2.f*scale, 0 );
          rec.copy( shadow_data, shadow_uniform );
          rec.transfer_to_graphics_barrier( { shadow_uniform->get_buffer() }, {} );
          {
            auto render_pass_token = rec.begin_render_pass(
              shadow_gbuffer.get_render_pass_begin_info( 0 ),
              vk::SubpassContents::eInline
            );
            rec->setViewport( 0, 1, &shadow_gbuffer.get_viewport() );
            rec->setScissor( 0, 1, &shadow_gbuffer.get_scissor() );
            rec->setCullMode( vk::CullModeFlagBits::eBack );
            rec->setDepthCompareOp( vk::CompareOp::eLessOrEqual );
            rec.bind_descriptor_set(
              vk::PipelineBindPoint::eGraphics,
              1u,
              sg->get_resource()->pipeline_layout,
              shadow_descriptor_set
            );
            (*il)( rec, *shadow_csg );
          }
          rec.convert_image(
            shadow_gbuffer.get_image( 0 ),
            vk::ImageLayout::eShaderReadOnlyOptimal
          );
          rec.barrier(
            vk::AccessFlagBits::eColorAttachmentWrite,
            vk::AccessFlagBits::eShaderRead,
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            vk::PipelineStageFlagBits::eFragmentShader,
            vk::DependencyFlagBits( 0 ),
            {},
            {
              shadow_gbuffer.get_image( 0 )
            }
          );
        }
        if( walk.light_moved() || walk.camera_moved() ) {
          rec.copy( global_data, global_uniform );
          rec.transfer_to_graphics_barrier( { global_uniform->get_buffer() }, {} );
          {
            auto render_pass_token = rec.begin_render_pass(
              gbuffer.get_render_pass_begin_info( 0 ),
              vk::SubpassContents::eInline
            );
            rec->setViewport( 0, 1, &gbuffer.get_viewport() );
            rec->setScissor( 0, 1, &gbuffer.get_scissor() );
            rec->setCullMode( vk::CullModeFlagBits::eBack );
            rec->setDepthCompareOp( vk::CompareOp::eLessOrEqual );
            rec.bind_descriptor_set(
              vk::PipelineBindPoint::eGraphics,
              1u,
              sg->get_resource()->pipeline_layout,
              global_descriptor_set
            );
            (*view_il)( rec, *geometry_csg );
          }
          sg->rotate_visibility( rec );
          rec.convert_image(
            gbuffer.get_image( 0 ),
            vk::ImageLayout::eGeneral
          );
        }
        if( walk.light_moved() || walk.camera_moved() || frame_counter <= 2u ) {
          lighting( rec, 0, res.width, res.height, 1u );
      
          rec.compute_barrier(
            {},
            { diffuse->get_factory() }
          );
      
          hbao( rec, 0 );
        
          rec.compute_barrier(
            {},
            {
              hbao.get_output()[ 0 ]->get_factory(),
              diffuse->get_factory(),
              specular->get_factory()
            }
          );

          const glm::mat4 world_to_screen = projection * walk.get_lookat();
          const glm::mat4 screen_to_world = glm::inverse( world_to_screen );
          skyview_froxel(
            rec,
            gct::skyview_froxel_param()
              .set_world_to_screen( world_to_screen )
              .set_screen_to_world( screen_to_world )
              .set_sigma_ma( skyview_param.sigma_ma )
              .set_sigma_oa( skyview_param.sigma_oa )
              .set_sigma_rs( skyview_param.sigma_rs )
              .set_sigma_ms( skyview_param.sigma_ms )
              .set_eye_pos( glm::vec4( walk.get_camera_pos(), 1.0 ) )
              .set_light_pos( glm::vec4( walk.get_light_pos(), 1.0 ) )
              .set_ground_radius( skyview_param.ground_radius )
              .set_top_radius( skyview_param.top_radius )
              .set_g( skyview_param.g )
              .set_altitude( skyview_param.ground_radius + skyview_param.altitude )
              .set_light_energy( walk.get_light_energy() )
          );
          rec.compute_barrier(
            {},
            {
              skyview_froxel.get_output()->get_factory()
            }
          );
        }
        bloom_gauss( rec, 0 );
        rec.compute_barrier( {}, { mixed_out->get_factory() } );
        tone.get( rec, 0 );
      }
      command_buffer->execute_and_wait();
    }
    if( walk.get_current_camera() == 0 ) {
      if( walk.light_moved() || walk.camera_moved() ) {
        last_visible = view_il->get_last_visible_list();
        gct::scene_graph::build_visible_set( last_visible, visible_set );
        last_visible_il->get_draw_list() = std::move( last_visible );
        last_visible.clear();
      }
    }
    auto &sync = framebuffers[ current_frame ];
    if( !sync.initial ) {
      sync.command_buffer->wait_for_executed();
    }
    else sync.initial = false;
    auto image_index = res.swapchain->acquire_next_image( sync.image_acquired );
    auto &fb = framebuffers[ image_index ];
    {
      auto rec = sync.command_buffer->begin();
      gamma( rec, image_index );
      rec.convert_image(
        res.swapchain_images[ image_index ],
        vk::ImageLayout::ePresentSrcKHR
      );
    }
    sync.command_buffer->execute(
      gct::submit_info_t()
        .add_wait_for( sync.image_acquired, vk::PipelineStageFlagBits::eColorAttachmentOutput )
        .add_signal_to( sync.draw_complete )
    );
    res.queue->present(
      gct::present_info_t()
        .add_wait_for( sync.draw_complete )
        .add_swapchain( res.swapchain, image_index )
    );

    glfwPollEvents();
    ++current_frame;
    ++frame_counter;
    walk.reset_flags();
    current_frame %= framebuffers.size();
  }
  (*res.queue)->waitIdle();
  walk.save( res.walk_state_filename );
}

