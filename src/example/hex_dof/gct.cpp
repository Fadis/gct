#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
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
#include <gct/fence.hpp>
#include <gct/wait_for_sync.hpp>
#include <gct/present_info.hpp>
#include <gct/gltf.hpp>
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
#include <gct/gltf2.hpp>
#include <gct/gltf2_create_info.hpp>
#include <gct/image_pool.hpp>
#include <gct/compiled_scene_graph.hpp>
#include <gct/instance_list.hpp>
#include <gct/kdtree.hpp>
#include <gct/skyview.hpp>
#include <gct/skyview_froxel.hpp>
#include <gct/af_state.hpp>

struct fb_resources_t {
  std::shared_ptr< gct::semaphore_t > image_acquired;
  std::shared_ptr< gct::semaphore_t > draw_complete;
  std::shared_ptr< gct::semaphore_t > image_ownership;
  std::shared_ptr< gct::bound_command_buffer_t > command_buffer;
  bool initial = true;
};

struct global_uniforms_t {
  LIBGCT_SETTER( eye_pos )
  LIBGCT_SETTER( voxel_proj )
  LIBGCT_SETTER( projection_matrix )
  LIBGCT_SETTER( camera_matrix )
  LIBGCT_SETTER( previous_projection_matrix )
  LIBGCT_SETTER( previous_camera_matrix )
  LIBGCT_SETTER( screen_to_world_matrix )
  LIBGCT_SETTER( voxel )
  LIBGCT_SETTER( inversed_voxel )
  LIBGCT_SETTER( frame_counter )
  LIBGCT_SETTER( light_count )
  LIBGCT_SETTER( ambient )
  LIBGCT_SETTER( light )
  glm::vec4 eye_pos;
  glm::ivec4 voxel_proj;
  std::int32_t projection_matrix;
  std::int32_t camera_matrix;
  std::int32_t previous_projection_matrix;
  std::int32_t previous_camera_matrix;
  std::int32_t screen_to_world_matrix;
  std::int32_t voxel;
  std::int32_t inversed_voxel;
  std::int32_t frame_counter;
  std::int32_t light_count;
  float ambient;
  std::int32_t light;
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
      VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
      VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME,
      VK_EXT_FRAGMENT_SHADER_INTERLOCK_EXTENSION_NAME,
      VK_NV_REPRESENTATIVE_FRAGMENT_TEST_EXTENSION_NAME
    },
    gct::descriptor_pool_create_info_t()
      .set_basic(
        vk::DescriptorPoolCreateInfo()
          .setFlags(
            vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet|
            vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind
          )
          .setMaxSets( 65536 * 7 )
      )
      .set_descriptor_pool_size( vk::DescriptorType::eUniformBuffer, 16 )
      .set_descriptor_pool_size( vk::DescriptorType::eStorageBuffer, 65536*2 )
      .set_descriptor_pool_size( vk::DescriptorType::eCombinedImageSampler, 65536*2 )
      .set_descriptor_pool_size( vk::DescriptorType::eStorageImage, 65536*2 )
      .rebuild_chain()
  );

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

  constexpr std::size_t gbuf_count = 2u;
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
      //.set_external_depth( depth_gbuffer.get_depth_views() )
  );

  constexpr std::size_t egbuf_count = 8u * 4u + 1u;
  const auto extended_gbuffer = res.allocator->create_image_view(
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( res.width, res.height )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage |
            vk::ImageUsageFlagBits::eTransferDst |
            vk::ImageUsageFlagBits::eTransferSrc
          )
          .setArrayLayers( egbuf_count )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  
  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( extended_gbuffer, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }


  gct::gbuffer aabb_gbuffer(
    gct::gbuffer_create_info()
      .set_allocator( res.allocator )
      .set_width( res.width )
      .set_height( res.height )
      .set_layer( 0u )
      .set_swapchain_image_count( 1u )
      .set_color_buffer_count( 1u )
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
      .add_master_shader( CMAKE_CURRENT_BINARY_DIR "/aabb" )
      .add_master_shader( CMAKE_CURRENT_BINARY_DIR "/depth" )
      .set_shader( CMAKE_CURRENT_BINARY_DIR )
      .set_enable_linear( true )
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

  const auto aabb_csg = std::make_shared< gct::scene_graph::compiled_aabb_scene_graph >(
    gct::scene_graph::compiled_aabb_scene_graph_create_info()
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/aabb" )
      .set_render_pass( aabb_gbuffer.get_render_pass() )
      .set_dynamic_cull_mode( true ),
    *sg
  );

  const auto il = std::make_shared< gct::scene_graph::instance_list >(
    gct::scene_graph::instance_list_create_info(),
    *sg
  );

  gct::kdtree< gct::scene_graph::resource_pair > full_kd;
  gct::scene_graph::append(
    *il->get_resource(),
    il->get_draw_list(),
    full_kd
  );

  auto command_buffer = res.queue->get_command_pool()->allocate();
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
      CMAKE_CURRENT_BINARY_DIR "/aabb",
      CMAKE_CURRENT_BINARY_DIR "/depth"
    },
    1u
  );
  std::shared_ptr< gct::mappable_buffer_t > global_uniform;
  std::shared_ptr< gct::descriptor_set_t > global_descriptor_set;
  global_uniform =
    res.allocator->create_mappable_buffer(
      sizeof( global_uniforms_t ),
      vk::BufferUsageFlagBits::eUniformBuffer
    );
  global_descriptor_set =
    res.descriptor_pool->allocate(
      global_descriptor_set_layout
    );
  global_descriptor_set->update({
    { "global_uniforms", global_uniform },
    { "shadow", cubemap_sampler, cubemap_images.get_cube_image_views()[ 0 ], vk::ImageLayout::eShaderReadOnlyOptimal },
    { "condition", sg->get_resource()->last_visibility->get_buffer() },
    { "extended_gbuffer", extended_gbuffer, vk::ImageLayout::eGeneral }
  });

  const auto rgba32ici =
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( res.width, res.height )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage|
            vk::ImageUsageFlagBits::eSampled
          )
      );
 
  const auto rgba32ici2 =
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( res.width, res.height )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage|
            vk::ImageUsageFlagBits::eSampled
          )
          .setArrayLayers( 2 )
      );
  
  const auto r32ici =
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( res.width, res.height )
          .setFormat( vk::Format::eR32Sfloat )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage|
            vk::ImageUsageFlagBits::eSampled
          )
      );
  
  const auto r32ici2 =
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( res.width, res.height )
          .setFormat( vk::Format::eR32Sfloat )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage|
            vk::ImageUsageFlagBits::eSampled
          )
          .setArrayLayers( 2 )
      );

  const auto rgba32ici4 =
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( res.width, res.height )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage|
            vk::ImageUsageFlagBits::eSampled
          )
          .setArrayLayers( 4u )
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
    rgba32ici4,
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  const auto specular = res.allocator->create_image_view(
    rgba32ici4,
    VMA_MEMORY_USAGE_GPU_ONLY
  );

  const auto near_dof = res.allocator->create_image_view(
    rgba32ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  
  const auto far_dof = res.allocator->create_image_view(
    rgba32ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );

  const auto near_dof0 = res.allocator->create_image_view(
    rgba32ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  
  const auto far_dof0 = res.allocator->create_image_view(
    rgba32ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );

  const auto near_dof1 = res.allocator->create_image_view(
    rgba32ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  
  const auto far_dof1 = res.allocator->create_image_view(
    rgba32ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );

  const auto near_dof2 = res.allocator->create_image_view(
    rgba32ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  
  const auto far_dof2 = res.allocator->create_image_view(
    rgba32ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );

  const auto near_coc = res.allocator->create_image_view(
    r32ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  
  const auto far_coc = res.allocator->create_image_view(
    r32ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );

  const auto near_max_coc = res.allocator->create_image_view(
    r32ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  
  const auto far_max_coc = res.allocator->create_image_view(
    r32ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );

  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( diffuse, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( specular, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( near_dof, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( far_dof, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( near_dof0, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( far_dof0, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( near_dof1, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( far_dof1, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( near_dof2, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( far_dof2, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( near_coc, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( far_coc, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( near_max_coc, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( far_max_coc, vk::ImageLayout::eGeneral );
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
      .add_resource( { "gbuffer", extended_gbuffer, vk::ImageLayout::eGeneral } )
      .add_resource( { "diffuse_image", diffuse } )
      .add_resource( { "specular_image", specular } )
      .add_resource( { "global_uniforms", global_uniform } )
      .add_resource( { "light_pool", sg->get_resource()->light->get_buffer() } )
  );

  gct::hbao hbao(
    gct::hbao_create_info()
      .set_allocator( res.allocator )
      .set_pipeline_cache( res.pipeline_cache )
      .set_descriptor_pool( res.descriptor_pool )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/ao/" )
      .set_input_name( "gbuffer" )
      .set_input( gbuffer.get_image_views() )
      .add_resource( { "global_uniforms", global_uniform } ) 
      .add_resource( { "matrix_pool", sg->get_resource()->matrix->get_buffer() } )
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
      .set_gbuffer( extended_gbuffer )
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
  

  auto unnormalized_sampler = res.device->get_sampler(
    gct::get_basic_unnormalized_sampler_create_info()
  );

  std::shared_ptr< gct::mappable_buffer_t > af_state_buffer =
    res.allocator->create_mappable_buffer(
      sizeof( gct::af_state ),
      vk::BufferUsageFlagBits::eStorageBuffer
    );
  
  const auto update_af = gct::compute(
    gct::compute_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/distance/distance.comp.spv" )
      .set_swapchain_image_count( 1u )
      .add_resource( { "gbuffer", extended_gbuffer, vk::ImageLayout::eGeneral } )
      .add_resource( { "af_state", af_state_buffer } )
  );
  

  const auto mix_ao = gct::compute(
    gct::compute_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/mix_ao/mix_ao.comp.spv" )
      .set_swapchain_image_count( 1u )
      .add_resource( { "gbuffer", extended_gbuffer, vk::ImageLayout::eGeneral } )
      .add_resource( { "occlusion", hbao.get_output(), vk::ImageLayout::eGeneral } )
      .add_resource( { "scattering", skyview_froxel.get_output(), vk::ImageLayout::eGeneral } )
      .add_resource( { "diffuse_image", diffuse } )
      .add_resource( { "specular_image", specular } )
      .add_resource( { "global_uniforms", global_uniform } )
      .add_resource( { "near_coc_image", near_coc, vk::ImageLayout::eGeneral } )
      .add_resource( { "far_coc_image", far_coc, vk::ImageLayout::eGeneral } )
      .add_resource( { "af_state", af_state_buffer } )
      .add_resource( { "near_dest_image", near_dof, vk::ImageLayout::eGeneral } )
      .add_resource( { "far_dest_image", far_dof, vk::ImageLayout::eGeneral } )
      .add_resource( { "light_pool", sg->get_resource()->light->get_buffer() } )
      .add_resource( { "matrix_pool", sg->get_resource()->matrix->get_buffer() } )
  );
  
  const auto dof_p0n = gct::compute(
    gct::compute_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/dof/p0n.comp.spv" )
      .set_swapchain_image_count( 1u )
      .add_resource( { "src_image", unnormalized_sampler, near_dof, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "dest_image0", near_dof0, vk::ImageLayout::eGeneral } )
      .add_resource( { "dest_image1", near_dof1, vk::ImageLayout::eGeneral } )
      .add_resource( { "coc", unnormalized_sampler, near_coc, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "max_coc", near_max_coc, vk::ImageLayout::eGeneral } )
      .add_resource( { "af_state", af_state_buffer } )
  );
  
  const auto dof_p0f = gct::compute(
    gct::compute_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/dof/p0f.comp.spv" )
      .set_swapchain_image_count( 1u )
      .add_resource( { "src_image", unnormalized_sampler, far_dof, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "dest_image0", far_dof0, vk::ImageLayout::eGeneral } )
      .add_resource( { "dest_image1", far_dof1, vk::ImageLayout::eGeneral } )
      .add_resource( { "coc", unnormalized_sampler, far_coc, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "max_coc", far_max_coc, vk::ImageLayout::eGeneral } )
      .add_resource( { "af_state", af_state_buffer } )
  );


  const auto dof_p1n = gct::compute(
    gct::compute_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/dof/p1n.comp.spv" )
      .set_swapchain_image_count( 1u )
      .add_resource( { "src_image0", unnormalized_sampler, near_dof0, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "src_image1", unnormalized_sampler, near_dof1, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "dest_image", near_dof2, vk::ImageLayout::eGeneral } )
      .add_resource( { "coc", unnormalized_sampler, near_max_coc, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "af_state", af_state_buffer } )
  );

  const auto dof_p1f = gct::compute(
    gct::compute_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/dof/p1f.comp.spv" )
      .set_swapchain_image_count( 1u )
      .add_resource( { "src_image0", unnormalized_sampler, far_dof0, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "src_image1", unnormalized_sampler, far_dof1, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "dest_image", far_dof2, vk::ImageLayout::eGeneral } )
      .add_resource( { "coc", unnormalized_sampler, far_max_coc, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "af_state", af_state_buffer } )
  );

  const auto bloom_gauss = gct::image_filter(
    gct::image_filter_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/merge/merge.comp.spv" )
      .set_input_name( "near_src_image" )
      .set_input( std::vector< std::shared_ptr< gct::image_view_t > >{ near_dof2 } )
      .set_output_create_info( rgba32ici )
      .set_output_name( "bloom_image" )
      .add_resource( { "far_src_image", far_dof2, vk::ImageLayout::eGeneral } )
      .add_resource( { "skyview", linear_sampler, skyview.get_output(), vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "dest_image", mixed_out } )
      .add_resource( { "tone", tone.get_buffer() } )
      .add_resource( { "global_uniforms", global_uniform } )
      .add_resource( { "matrix_pool", sg->get_resource()->matrix->get_buffer() } )
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
      .set_input( std::vector< std::shared_ptr< gct::image_view_t > >{ mixed_out, mixed_out } )
      .set_output( res.swapchain_image_views )
      .add_resource( { "bloom_image", bloom_gauss.get_output(), vk::ImageLayout::eGeneral } )
  );

  const auto scene_aabb = sg->get_root_node()->get_initial_world_aabb();
  if( !scene_aabb ) throw -1;

  const auto center = ( scene_aabb->min + scene_aabb->max ) / 2.f;
  const auto scale = std::abs( glm::length( scene_aabb->max - scene_aabb->min ) );

  const glm::mat4 projection = glm::perspective( 0.6981317007977318f, (float(res.width)/float(res.height)), std::min(0.1f*scale,0.5f), scale );
  const float light_size = 0.3;

  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      gct::af_state state = gct::af_state()
        .set_znear( std::min(0.1f*scale,0.5f) )
        .set_zfar( scale );
      recorder.copy( state, af_state_buffer );
    }
    command_buffer->execute_and_wait();
  }

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
  const auto proj_desc = sg->get_resource()->matrix->allocate( projection );
  const auto previous_proj_desc = sg->get_resource()->matrix->get_history( proj_desc );
  const auto camera_desc = sg->get_resource()->matrix->allocate( walk.get_lookat() );
  const auto previous_camera_desc = sg->get_resource()->matrix->get_history( camera_desc );
  const auto screen_to_world_desc = sg->get_resource()->matrix->allocate( glm::inverse( projection * walk.get_lookat() ) );
  const auto light_desc = sg->get_resource()->light->allocate(
    sg->get_root_node()->matrix,
    gct::punctual_light_parameter()
      .set_type( gct::punctual_light_type::point )
      .set_light_size( light_size )
      .set_local_position( glm::vec4( walk.get_light_pos(), 1.0 ) * glm::vec4( 1.0, -1.0, 1.0, 1.0 ) )
      .set_energy( glm::vec4( walk.get_light_energy(),walk.get_light_energy(),walk.get_light_energy(), 1.0 ) )
  );

  std::shared_ptr< gct::mappable_buffer_t > shadow_uniform;
  std::shared_ptr< gct::descriptor_set_t > shadow_descriptor_set;
  shadow_uniform =
    res.allocator->create_mappable_buffer(
      sizeof( global_uniforms_t ),
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

  std::uint32_t current_frame = 0u;
  std::uint32_t frame_counter = 0u;
  bool update_optflow = false;
  std::minstd_rand rng;
  std::uniform_real_distribution jitter_dist( -0.0005, 0.0005 );
  while( !walk.end() ) {
    gct::blocking_timer frame_rate;
    ++walk;
    const auto global_data = global_uniforms_t()
      .set_projection_matrix( *proj_desc )
      .set_camera_matrix( *camera_desc )
      .set_previous_projection_matrix( * previous_proj_desc )
      .set_previous_camera_matrix( *previous_camera_desc )
      .set_screen_to_world_matrix( *screen_to_world_desc )
      .set_eye_pos( glm::vec4( walk.get_camera_pos(), 1.0 ) )
      .set_light_count( res.light_count )
      .set_ambient( res.ambient_level )
      .set_frame_counter( frame_counter )
      .set_light( *light_desc );
    {
      sg->get_resource()->matrix->set( proj_desc, projection );
      sg->get_resource()->matrix->set( camera_desc, walk.get_lookat() );
      sg->get_resource()->matrix->set( screen_to_world_desc, glm::inverse( projection * walk.get_lookat() ) );
      sg->get_resource()->light->set(
        light_desc,
        gct::punctual_light_parameter()
          .set_type( gct::punctual_light_type::point )
          .set_light_size( light_size )
          .set_local_position( glm::vec4( walk.get_light_pos(), 1.0 ) * glm::vec4( 1.0, -1.0, 1.0, 1.0 ) )
          .set_energy( glm::vec4( walk.get_light_energy(),walk.get_light_energy(),walk.get_light_energy(), 1.0 ) )
      );
      {
        auto rec = command_buffer->begin();
        (*sg)( rec );
        rec.copy( global_data, global_uniform );
        rec.transfer_to_graphics_barrier( { global_uniform->get_buffer() }, {} );
      }
      command_buffer->execute_and_wait();
    }
    // depth
    if( walk.camera_moved() ) {
      {
        auto rec = command_buffer->begin();
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
          (*il)( rec, *depth_csg, true );
        }
      }
      command_buffer->execute_and_wait();
    }
    // occlusion query
    if( walk.get_current_camera() == 0 ) {
      if( walk.camera_moved() ) {
        {
          auto rec = command_buffer->begin();
          rec.copy( global_data, global_uniform );
          rec.transfer_to_graphics_barrier( { global_uniform->get_buffer() }, {} );
          il->setup_resource_pair_buffer( rec );
          {
            auto render_pass_token = rec.begin_render_pass(
              aabb_gbuffer.get_render_pass_begin_info( 0 ),
              vk::SubpassContents::eInline
            );
            rec->setViewport( 0, 1, &aabb_gbuffer.get_viewport() );
            rec->setScissor( 0, 1, &aabb_gbuffer.get_scissor() );
            rec->setCullMode( vk::CullModeFlagBits::eBack );
            rec->setDepthCompareOp( vk::CompareOp::eLessOrEqual );
            rec.bind_descriptor_set(
              vk::PipelineBindPoint::eGraphics,
              1u,
              sg->get_resource()->pipeline_layout,
              global_descriptor_set
            );
            (*il)( rec, *aabb_csg );
          }
          rec.convert_image(
            aabb_gbuffer.get_image( 0 ),
            vk::ImageLayout::eGeneral
          );
        }
        command_buffer->execute_and_wait();
      }
    }
    {
      {
        auto rec = command_buffer->begin();
        tone.set( rec, 0 );
        if( walk.light_moved() ) {
          const auto shadow_data = global_uniforms_t()
            .set_projection_matrix( *proj_desc )
            .set_camera_matrix( *camera_desc )
            //.set_previous_projection_matrix( * previous_proj_desc )
            //.set_previous_camera_matrix( *previous_camera_desc )
            .set_screen_to_world_matrix( *screen_to_world_desc )
            .set_eye_pos( glm::vec4( walk.get_camera_pos(), 1.0 ) )
            .set_light_count( res.light_count )
            .set_ambient( res.ambient_level )
            .set_frame_counter( frame_counter );
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
            rec.fill( extended_gbuffer->get_factory(), gct::color::special::transparent );
            rec.barrier( {}, { extended_gbuffer->get_factory() } );
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
            (*il)( rec, *geometry_csg, true );
          }
          if( walk.get_current_camera() == 0 ) {
            sg->rotate_visibility( rec );
          }
          rec.barrier( {}, { extended_gbuffer->get_factory() } );
          rec.convert_image(
            gbuffer.get_image( 0 ),
            vk::ImageLayout::eGeneral
          );
        }
        if( walk.light_moved() || walk.camera_moved() || frame_counter <= 2u ) {
          lighting( rec, 0, res.width, res.height, 4u );
      
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

        glm::ivec2 focus( res.width/2, res.height/2 );
        rec->pushConstants(
          **update_af.get_pipeline()->get_props().get_layout(),
          update_af.get_pipeline()->get_props().get_layout()->get_props().get_push_constant_range()[ 0 ].stageFlags,
          0u,
          sizeof( glm::ivec2 ),
          &focus
        );
        update_af( rec, 0, 16, 16, 1u );
        rec.compute_barrier( { af_state_buffer->get_buffer() }, {} );
        mix_ao( rec, 0, res.width, res.height, 1u );
        rec.convert_image( near_dof->get_factory(), vk::ImageLayout::eShaderReadOnlyOptimal );
        rec.convert_image( far_dof->get_factory(), vk::ImageLayout::eShaderReadOnlyOptimal );
        rec.convert_image( near_coc->get_factory(), vk::ImageLayout::eShaderReadOnlyOptimal );
        rec.convert_image( far_coc->get_factory(), vk::ImageLayout::eShaderReadOnlyOptimal );
        rec.convert_image( near_max_coc->get_factory(), vk::ImageLayout::eGeneral );
        rec.convert_image( far_max_coc->get_factory(), vk::ImageLayout::eGeneral );
        rec.convert_image( near_dof0->get_factory(), vk::ImageLayout::eGeneral );
        rec.convert_image( near_dof1->get_factory(), vk::ImageLayout::eGeneral );
        rec.convert_image( far_dof0->get_factory(), vk::ImageLayout::eGeneral );
        rec.convert_image( far_dof1->get_factory(), vk::ImageLayout::eGeneral );
        dof_p0n( rec, 0, res.width, res.height, 1u );
        dof_p0f( rec, 0, res.width, res.height, 1u );
        rec.convert_image( near_dof0->get_factory(), vk::ImageLayout::eShaderReadOnlyOptimal );
        rec.convert_image( near_dof1->get_factory(), vk::ImageLayout::eShaderReadOnlyOptimal );
        rec.convert_image( far_dof0->get_factory(), vk::ImageLayout::eShaderReadOnlyOptimal );
        rec.convert_image( far_dof1->get_factory(), vk::ImageLayout::eShaderReadOnlyOptimal );
        rec.convert_image( near_max_coc->get_factory(), vk::ImageLayout::eShaderReadOnlyOptimal );
        rec.convert_image( far_max_coc->get_factory(), vk::ImageLayout::eShaderReadOnlyOptimal );
        dof_p1n( rec, 0, res.width, res.height, 1u );
        dof_p1f( rec, 0, res.width, res.height, 1u );
        rec.convert_image( near_dof->get_factory(), vk::ImageLayout::eGeneral );
        rec.convert_image( far_dof->get_factory(), vk::ImageLayout::eGeneral );
        rec.convert_image( near_coc->get_factory(), vk::ImageLayout::eGeneral );
        rec.convert_image( far_coc->get_factory(), vk::ImageLayout::eGeneral );
        rec.compute_barrier( {}, {
          near_dof2->get_factory(),
          far_dof2->get_factory()
        } );
        bloom_gauss( rec, 0 );
        rec.compute_barrier( {}, { mixed_out->get_factory() } );
        tone.get( rec, 0 );
      }
      command_buffer->execute_and_wait();
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

