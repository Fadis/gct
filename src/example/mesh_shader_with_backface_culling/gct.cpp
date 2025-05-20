#include <chrono>
#include <iostream>
#include <algorithm>
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
#include <gct/hbao2.hpp>
#include <gct/gauss.hpp>
#include <gct/scene_graph.hpp>
#include <gct/gltf2.hpp>
#include <gct/gltf2_create_info.hpp>
#include <gct/image_pool.hpp>
#include <gct/compiled_scene_graph.hpp>
#include <gct/instance_list.hpp>
#include <gct/kdtree.hpp>
#include <gct/skyview.hpp>
#include <gct/skyview_froxel2.hpp>
#include <gct/skyview_froxel2_create_info.hpp>
#include <gct/skyview_froxel2_param.hpp>
#include <gct/lrlf_dof.hpp>
#include <gct/lens_flare.hpp>
#include <gct/af_state.hpp>
#include <gct/image_io.hpp>

struct fb_resources_t {
  std::shared_ptr< gct::semaphore_t > image_acquired;
  std::shared_ptr< gct::semaphore_t > draw_complete;
  std::shared_ptr< gct::semaphore_t > render_complete;
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
  LIBGCT_SETTER( gbuffer )
  LIBGCT_SETTER( depth )
  LIBGCT_SETTER( tone_value )
  LIBGCT_SETTER( tone_scale )
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
  std::int32_t gbuffer;
  std::int32_t depth;
  std::uint32_t tone_value;
  float tone_scale;
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
      VK_NV_REPRESENTATIVE_FRAGMENT_TEST_EXTENSION_NAME,
      VK_KHR_MULTIVIEW_EXTENSION_NAME,
      VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME,
      VK_KHR_MAINTENANCE_4_EXTENSION_NAME,
      VK_EXT_MESH_SHADER_EXTENSION_NAME//,
      //VK_KHR_SHADER_NON_SEMANTIC_INFO_EXTENSION_NAME
    },
    gct::descriptor_pool_create_info_t()
      .set_basic(
        vk::DescriptorPoolCreateInfo()
          .setFlags(
            vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet|
            vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind
          )
          .setMaxSets( 65536 * 10 )
      )
      .set_descriptor_pool_size( vk::DescriptorType::eUniformBuffer, 16 )
      .set_descriptor_pool_size( vk::DescriptorType::eStorageBuffer, 65536*5 )
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

  const unsigned int shadow_map_size = 1024u;
  const auto shadow_map_desc = sg->get_resource()->image->allocate(
    gct::image_allocate_info()
      .set_create_info(
        gct::image_create_info_t()
          .set_basic(
            gct::basic_2d_image( shadow_map_size, shadow_map_size )
              .setFormat( vk::Format::eR32Sfloat )
              .setFlags( vk::ImageCreateFlagBits::eCubeCompatible )
              .setUsage(
                vk::ImageUsageFlagBits::eColorAttachment|
                vk::ImageUsageFlagBits::eSampled|
                vk::ImageUsageFlagBits::eStorage|
                vk::ImageUsageFlagBits::eTransferSrc|
                vk::ImageUsageFlagBits::eTransferDst
              )
              .setArrayLayers( 6u )
          )
      )
      .set_range(
        gct::subview_range()
          .set_layer_count( 6u )
      )
      .set_layout(
        vk::ImageLayout::eGeneral
      )
  );
  const auto linear_sampler_desc = sg->get_resource()->sampler->allocate_linear();
  
  auto linear_sampler = sg->get_resource()->sampler->get( linear_sampler_desc );

  gct::gbuffer shadow_gbuffer(
    gct::gbuffer_create_info()
      .set_allocator( res.allocator )
      .set_external_color( { sg->get_resource()->image->get( shadow_map_desc.linear ) } )
      .set_swapchain_image_count( 1u )
      .set_color_buffer_count( 1 )
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
      .set_clear_color( gct::color::web::white )
  );

  gct::cubemap_images2 cubemap_images( shadow_gbuffer.get_image_views() );
  
  const auto shadow_map_texture_desc = sg->get_resource()->texture->allocate(
     linear_sampler,
     cubemap_images.get_cube_image_views()[ 0 ]
  );

  gct::gbuffer depth_gbuffer(
    gct::gbuffer_create_info()
      .set_allocator( res.allocator )
      .set_width( res.width )
      .set_height( res.height )
      .set_layer( 1u )
      .set_swapchain_image_count( 1u )
      .set_color_buffer_count( 0u )
      .set_format( vk::Format::eR16G16B16A16Sfloat ) 
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
  );

  constexpr std::size_t gbuf_count = 0u;
  gct::gbuffer gbuffer(
    gct::gbuffer_create_info()
      .set_allocator( res.allocator )
      .set_width( res.width )
      .set_height( res.height )
      .set_layer( 0u )
      .set_swapchain_image_count( 1u )
      .set_color_buffer_count( gbuf_count )
      .set_format( vk::Format::eR16G16B16A16Sfloat ) 
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
  );

  constexpr std::size_t egbuf_count = 8u * 4u + 1u;
  const auto extended_gbuffer_desc = sg->get_resource()->image->allocate(
    gct::image_allocate_info()
      .set_create_info(
        gct::image_create_info_t()
          .set_basic(
            gct::basic_2d_image( res.width, res.height )
              .setFormat( vk::Format::eR16G16B16A16Sfloat )
              .setUsage(
                vk::ImageUsageFlagBits::eStorage |
                vk::ImageUsageFlagBits::eTransferDst |
                vk::ImageUsageFlagBits::eTransferSrc
              )
              .setArrayLayers( egbuf_count )
          )
      )
      .set_range(
        gct::subview_range()
          .set_layer_count( egbuf_count )
      )
      .set_layout(
        vk::ImageLayout::eGeneral
      )
  );

  const auto extended_gbuffer = sg->get_resource()->image->get( extended_gbuffer_desc.linear );
 
  const auto extended_depth_desc = sg->get_resource()->image->allocate(
    gct::image_allocate_info()
      .set_create_info(
        gct::image_create_info_t()
          .set_basic(
            gct::basic_2d_image( res.width, res.height )
              .setFormat( vk::Format::eR32Sfloat )
              .setUsage(
                vk::ImageUsageFlagBits::eStorage |
                vk::ImageUsageFlagBits::eTransferDst |
                vk::ImageUsageFlagBits::eTransferSrc
              )
              .setArrayLayers( 4u )
          )
      )
      .set_range(
        gct::subview_range()
          .set_layer_count( 4u )
      )
      .set_layout(
        vk::ImageLayout::eGeneral
      )
  );
  
  const auto extended_depth = sg->get_resource()->image->get( extended_depth_desc.linear );

  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( extended_gbuffer, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( extended_depth, vk::ImageLayout::eGeneral );
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
      .set_color_buffer_count( 0u )
      .set_format( vk::Format::eR16G16B16A16Sfloat ) 
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
      .set_external_depth( depth_gbuffer.get_depth_views() )
  );

  gct::gltf::gltf2 doc(
    gct::gltf::gltf2_create_info()
      .set_filename( res.model_filename )
      .set_graph( sg )
      .set_root( sg->get_root_node() )
      .set_aspect_ratio( float( res.width )/float( res.height ) )
  );

  /*const auto geometry_csg = std::make_shared< gct::scene_graph::compiled_scene_graph >(
    gct::scene_graph::compiled_scene_graph_create_info()
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/geometry" )
      .set_render_pass( gbuffer.get_render_pass() )
      .set_dynamic_cull_mode( true ),
    *sg
  );*/
  
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
    { "condition", sg->get_resource()->last_visibility->get_buffer() }
  });
  
  const auto tone_buffer =
    res.allocator->create_mappable_buffer(
      16u,
      vk::BufferUsageFlagBits::eStorageBuffer|
      vk::BufferUsageFlagBits::eTransferSrc|
      vk::BufferUsageFlagBits::eTransferDst
    );

  const auto rgba16ici =
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( res.width, res.height )
          .setFormat( vk::Format::eR16G16B16A16Sfloat )
          .setUsage( vk::ImageUsageFlagBits::eStorage )
      );
 
  const auto rgba16ici2 =
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( res.width, res.height )
          .setFormat( vk::Format::eR16G16B16A16Sfloat )
          .setUsage( vk::ImageUsageFlagBits::eStorage )
          .setArrayLayers( 2 )
      );
  
  const auto r16ici =
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( res.width, res.height )
          .setFormat( vk::Format::eR16Sfloat )
          .setUsage( vk::ImageUsageFlagBits::eStorage )
      );
  
  const auto r16ici2 =
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( res.width, res.height )
          .setFormat( vk::Format::eR16Sfloat )
          .setUsage( vk::ImageUsageFlagBits::eStorage )
          .setArrayLayers( 2 )
      );

  const auto rgba16ici4 =
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( res.width, res.height )
          .setFormat( vk::Format::eR16G16B16A16Sfloat )
          .setUsage( vk::ImageUsageFlagBits::eStorage )
          .setArrayLayers( 4u )
      );
  
  const auto rgba16ici10 =
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( res.width, res.height )
          .setFormat( vk::Format::eR16G16B16A16Sfloat )
          .setUsage( vk::ImageUsageFlagBits::eStorage )
          .setArrayLayers( 10u )
      );

  std::vector< fb_resources_t > framebuffers;
  for( std::size_t i = 0u; i != res.swapchain_images.size(); ++i ) {
    framebuffers.emplace_back(
      fb_resources_t{
        res.device->get_semaphore(),
        res.device->get_semaphore(),
        res.device->get_semaphore(),
        res.device->get_semaphore(),
        res.queue->get_command_pool()->allocate()
      }
    );
  }

  const auto sb_image_desc = sg->get_resource()->image->allocate(
    gct::image_load_info()
      .set_filename( CMAKE_CURRENT_SOURCE_DIR "/flare_950.png" )
      .set_enable_linear( true )
  );
  const auto sb_sampler_desc = sg->get_resource()->sampler->allocate(
    gct::get_basic_linear_sampler_create_info()
  );

  const auto sb_texture_desc = sg->get_resource()->texture->allocate(
    sb_sampler_desc,
    sb_image_desc
  );
  
  gct::lens_flare sb(
    gct::lens_flare_create_info()
      .set_allocator_set( res.allocator_set )
      .set_width( res.width/ 4 )
      .set_height( res.height/ 4 )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/sb/" )
      .set_texture( sb_texture_desc.linear )
      .set_matrix_count( 1 )
      .set_format( vk::Format::eR16G16B16A16Sfloat )
      .set_lens_size( 0.025 / 2.8 )
      .set_scene_graph( sg->get_resource() )
      .add_resource( { "global_uniforms", global_uniform } )
      .add_resource( { "gbuffer", extended_gbuffer, vk::ImageLayout::eGeneral } )
  );

  const auto sb_rendered_desc = sg->get_resource()->image->allocate( sb.get_image_view() );

  const auto flare_image_desc = sg->get_resource()->image->allocate(
    gct::image_load_info()
      .set_filename( CMAKE_CURRENT_SOURCE_DIR "/flare_200.png" )
      .set_enable_linear( true )
  );
  const auto flare_sampler_desc = sg->get_resource()->sampler->allocate(
    gct::get_basic_linear_sampler_create_info()
  );

  const auto flare_texture_desc = sg->get_resource()->texture->allocate(
    flare_sampler_desc,
    flare_image_desc
  );
  
  gct::lens_flare flare(
    gct::lens_flare_create_info()
      .set_allocator_set( res.allocator_set )
      .set_width( res.width / 8 )
      .set_height( res.height / 8 )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/flare/" )
      .set_texture( flare_texture_desc.linear )
      .set_matrix_count( 36 )
      .set_format( vk::Format::eR16G16B16A16Sfloat )
      .set_lens_size( 0.025 / 2.8 )
      .set_scene_graph( sg->get_resource() )
      .add_resource( { "global_uniforms", global_uniform } )
      .add_resource( { "gbuffer", extended_gbuffer, vk::ImageLayout::eGeneral } )
  );

  const auto flare_rendered_desc = sg->get_resource()->image->allocate( flare.get_image_view() );

  std::shared_ptr< gct::mappable_buffer_t > af_state_buffer =
    res.allocator->create_mappable_buffer(
      sizeof( gct::af_state ),
      vk::BufferUsageFlagBits::eStorageBuffer
    );
 
  const auto unproject_desc = sg->get_resource()->matrix->allocate();

  gct::hbao2 hbao(
    gct::hbao2_create_info()
      .set_allocator( res.allocator )
      .set_pipeline_cache( res.pipeline_cache )
      .set_descriptor_pool( res.descriptor_pool )
      .set_unproject( unproject_desc )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/ao/" )
      .set_scene_graph( sg->get_resource() )
      .add_resource( { "global_uniforms", global_uniform } ) 
  );

  gct::skyview skyview(
    gct::skyview_create_info()
      .set_allocator( res.allocator )
      .set_pipeline_cache( res.pipeline_cache )
      .set_descriptor_pool( res.descriptor_pool )
      .set_format( vk::Format::eR16G16B16A16Sfloat )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/skyview/" )
  );
  const auto transmittance_desc = sg->get_resource()->image->allocate( skyview.get_transmittance() );
  const auto skyview_desc = sg->get_resource()->image->allocate( skyview.get_output() );

  const auto sigma = gct::get_default_skyview_sigma();
  const auto sigma_desc = sg->get_resource()->matrix->allocate( sigma );
  const auto world_to_screen_desc = sg->get_resource()->matrix->allocate();
  const auto unproject_to_world_desc = sg->get_resource()->matrix->allocate();
  
  gct::skyview_froxel2 skyview_froxel2(
    gct::skyview_froxel2_create_info()
      .set_allocator_set( res.allocator_set )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/skyview/" )
      .set_sigma( sigma_desc )
      .set_world_to_screen( world_to_screen_desc )
      .set_unproject_to_world( unproject_to_world_desc )
      .set_scene_graph( sg->get_resource() )
      .add_resource( { "global_uniforms", global_uniform } )
      .set_node_name( "skyview_froxel" )
  );
 
  gct::gauss coc_gauss(
    gct::gauss_create_info()
      .set_allocator_set( res.allocator_set )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/coc_gauss/" )
      .set_scene_graph( sg->get_resource() )
      .add_resource( { "global_uniforms", global_uniform } )
      .set_node_name( "coc_gauss" )
  );
  
  gct::lrlf_dof dof(
    gct::lrlf_dof_create_info()
      .set_allocator_set( res.allocator_set )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/dof/" )
      .set_scene_graph( sg->get_resource() )
      .add_resource( { "global_uniforms", global_uniform } )
      .set_node_name( "dof" )
  );
  
  gct::gauss bloom_gauss(
    gct::gauss_create_info()
      .set_allocator_set( res.allocator_set )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/gauss/" )
      .set_scene_graph( sg->get_resource() )
      .set_node_name( "bloom_gauss" )
  );

  gct::graphics geometry(
    gct::graphics_create_info()
      .set_pipeline_create_info(
        gct::graphics_pipeline_create_info_t()
          .set_gbuffer( gbuffer )
      )
      .set_swapchain_image_count( 1u )
      .add_shader( CMAKE_CURRENT_BINARY_DIR "/geometry/geometry.task.spv" )
      .add_shader( CMAKE_CURRENT_BINARY_DIR "/geometry/geometry.mesh.spv" )
      .add_shader( CMAKE_CURRENT_BINARY_DIR "/geometry/geometry.frag.spv" )
      .set_scene_graph( sg->get_resource() )
      .add_resource( { "global_uniforms", global_uniform } )
  );

  gct::shader_graph::builder builder( sg->get_resource() );
  
  const auto lighting_desc = builder.call(
    builder.get_image_io_create_info(
      std::make_shared< gct::compute >(
        gct::compute_create_info()
          .set_allocator_set( res.allocator_set )
          .set_shader( CMAKE_CURRENT_BINARY_DIR "/lighting/lighting.comp.spv" )
          .set_scene_graph( sg->get_resource() )
          .add_resource( { "global_uniforms", global_uniform } )
      ),
      gct::image_io_plan()
        .add_input( "src" )
        .add_input( "depth" )
        .add_output( "dest", "src", { 1.f, -4.f }, vk::Format::eR16G16B16A16Sfloat )
        .set_dim( "src", { 1.f, -4.f } )
        .set_node_name( "lighting" )
    )
    .set_push_constant( "unproject", *unproject_to_world_desc )
  )(
    gct::shader_graph::vertex::combined_result_type()
      .add( "src", extended_gbuffer_desc.linear )
      .add( "depth", extended_depth_desc.linear )
  );

  const auto np_desc = builder.call(
    std::make_shared< gct::compute >(
      gct::compute_create_info()
        .set_allocator_set( res.allocator_set )
        .set_shader( CMAKE_CURRENT_BINARY_DIR "/nearest_position/nearest_position.comp.spv" )
        .set_scene_graph( sg->get_resource() )
    ),
    gct::image_io_plan()
      .add_input( "src" )
      .add_input( "depth" )
      .add_output( "dest", "src", glm::vec2( 1.f, -1.f ), vk::Format::eR32Sfloat )
      .set_dim( "src", glm::vec2( 1.f, -1.f ) )
      .set_node_name( "nearest_position" )
  )(
    gct::shader_graph::vertex::combined_result_type()
      .add( "src", extended_gbuffer_desc.linear )
      .add( "depth", extended_depth_desc.linear )
  );

  const auto ao_out_desc = hbao(
    builder,
    np_desc
  );
  const auto skyview_froxel_out_desc = skyview_froxel2(
    builder,
    extended_gbuffer_desc.linear,
    extended_depth_desc.linear,
    transmittance_desc
  );

  const auto mix_ao_desc = builder.call(
    std::make_shared< gct::compute >(
      gct::compute_create_info()
        .set_allocator_set( res.allocator_set )
        .set_shader( CMAKE_CURRENT_BINARY_DIR "/mix_ao/mix_ao.comp.spv" )
        .set_scene_graph( sg->get_resource() )
        .add_resource( { "global_uniforms", global_uniform } )
        .add_resource( { "af_state", af_state_buffer } )
    ),
    gct::image_io_plan()
      .add_input( "gbuffer" )
      .add_input( "depth" )
      .add_input( "occlusion" )
      .add_input( "scattering" )
      .add_input( "lighting_image" )
      .add_output( "coc_image", "gbuffer", glm::vec2( 1.0f, -2.0f ), vk::Format::eR16G16B16A16Sfloat )
      .add_output( "dest_image", "gbuffer", glm::vec2( 1.0f, -2.0f ), vk::Format::eR16G16B16A16Sfloat )
      .set_dim( "gbuffer", glm::vec2( 1.0f, -1.0f ) )
      .set_node_name( "mix_ao" )
  )(
    gct::shader_graph::vertex::combined_result_type()
      .add( "gbuffer", extended_gbuffer_desc.linear )
      .add( "depth", extended_depth_desc.linear )
      .add( "occlusion", ao_out_desc[ "dest" ] )
      .add( "scattering", skyview_froxel_out_desc[ "dest" ] )
      .add( "lighting_image", lighting_desc )
  );

  const auto filtered_coc = coc_gauss( builder, mix_ao_desc[ "coc_image" ]  );
  
  const auto dof_desc = dof(
    builder,
    mix_ao_desc[ "dest_image" ],
    filtered_coc
  );

  const auto merge_desc = builder.call(
    std::make_shared< gct::compute >(
      gct::compute_create_info()
        .set_allocator_set( res.allocator_set )
        .set_shader( CMAKE_CURRENT_BINARY_DIR "/merge/merge.comp.spv" )
        .set_scene_graph( sg->get_resource() )
        .add_resource( { "global_uniforms", global_uniform } )
        .add_resource( { "af_state", af_state_buffer } )
        .add_resource( { "tone", tone_buffer->get_buffer() } )
        .add_resource( { "sb", sb.get_image_view(), vk::ImageLayout::eGeneral } )
    ),
    gct::image_io_plan()
      .add_input( "src" )
      .add_sampled( "star", linear_sampler_desc )
      .add_sampled( "flare", linear_sampler_desc )
      .add_sampled( "skyview", linear_sampler_desc )
      .add_output( "dest", "src", glm::vec2( 1.f, -1.f ) )
      .add_output( "bloom", "src", glm::vec2( 1.f, -1.f ) )
      .set_dim( "src", glm::vec2( 1.f, -1.f ) )
      .set_node_name( "merge" )
  )(
    gct::shader_graph::vertex::combined_result_type()
      .add( "src", dof_desc )
      .add( "skyview", skyview_desc )
      .add( "star", sb_rendered_desc )
      .add( "flare", flare_rendered_desc )
  );
  
  const auto filtered_bloom = bloom_gauss( builder, merge_desc[ "bloom" ] );

  builder.output( merge_desc[ "dest" ] );
  builder.output( filtered_bloom );
  const auto compiled = builder();
  const auto merged_view = compiled.get_view( merge_desc[ "dest" ] );
  const auto bloom_view = compiled.get_view( filtered_bloom );
  
  const auto skyview_param =
    gct::skyview_parameter()
      .set_convert_to_xyz( false )
      .set_sigma_ma( sigma[ 0 ] )
      .set_sigma_oa( sigma[ 1 ] )
      .set_sigma_rs( sigma[ 2 ] )
      .set_sigma_ms( sigma[ 3 ] );
  
  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto rec = command_buffer->begin();
      skyview( rec, skyview_param );
    }
    command_buffer->execute_and_wait();
  }
  
  auto update_af = gct::compute(
    gct::compute_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/distance/distance.comp.spv" )
      .set_swapchain_image_count( 1u )
      .add_resource( { "gbuffer", extended_gbuffer, vk::ImageLayout::eGeneral } )
      .add_resource( { "depth", extended_depth, vk::ImageLayout::eGeneral } )
      .add_resource( { "af_state", af_state_buffer } )
  );

  update_af.set_push_constant( "focus_pos", glm::ivec2( res.width/2, res.height/2 ) );
  
  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto rec = command_buffer->begin();
      rec.convert_image(
        merged_view->get_factory(), vk::ImageLayout::eGeneral
      );
    }
    command_buffer->execute_and_wait();
  }
  const gct::tone_mapping tone(
    gct::tone_mapping_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/tone/tone.comp.spv" )
      .set_input( std::vector< std::shared_ptr< gct::image_view_t > >{ merged_view } )
      .set_output( tone_buffer )
  );

  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( merged_view, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( sb.get_image_view(), vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }
  
  const auto gamma = gct::image_filter(
    gct::image_filter_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/gamma/gamma.comp.spv" )
      .set_input( std::vector< std::shared_ptr< gct::image_view_t > >( res.swapchain_images.size(), merged_view ) )
      .set_output( res.swapchain_image_views )
      .add_resource( { "bloom_image", bloom_view, vk::ImageLayout::eGeneral } )
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
        .set_zfar( scale )
        .set_lens_size( 0.05f/2.8f )
        .set_visible_range( 1.0 );
      recorder.copy( state, af_state_buffer );
    }
    command_buffer->execute_and_wait();
  }

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
  sg->get_resource()->matrix->set( unproject_desc, glm::inverse(
    glm::mat4(
      0.5f, 0.f, 0.f, 0.0f,
      0.f, 0.5f, 0.f, 0.0f,
      0.f, 0.f, 1.0f, 0.0f,
      0.5f, 0.5f, 0.0f, 1.f
    ) *
    projection
  ) );
  const auto light_desc = sg->get_resource()->light->allocate(
    sg->get_root_node()->matrix,
    gct::punctual_light_parameter()
      .set_type( gct::punctual_light_type::point )
      .set_light_size( light_size )
      .set_local_position( glm::vec4( walk.get_light_pos(), 1.0 ) * glm::vec4( 1.0, -1.0, 1.0, 1.0 ) )
      .set_energy( glm::vec4( walk.get_light_energy(),walk.get_light_energy(),walk.get_light_energy(), 1.0 ) )
      .set_shadow_map( *shadow_map_texture_desc )
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
  float average = 0.f;
  while( !walk.end() ) {
    const auto begin_date = std::chrono::high_resolution_clock::now();
    auto &sync = framebuffers[ current_frame ];
    gct::blocking_timer frame_rate;
    ++walk;
    // depth
    {
      {
        auto rec = command_buffer->begin();
        if( res.force_geometry || walk.camera_moved() || walk.light_moved() ) {
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
            .set_light( *light_desc )
            .set_gbuffer( *extended_gbuffer_desc.linear )
            .set_depth( *extended_depth_desc.linear );

          sg->get_resource()->matrix->set( camera_desc, walk.get_lookat() );
          sg->get_resource()->matrix->set( world_to_screen_desc, projection * walk.get_lookat() );
          sg->get_resource()->matrix->set( screen_to_world_desc, glm::inverse( projection * walk.get_lookat() ) );
          sg->get_resource()->matrix->set( unproject_to_world_desc, glm::inverse(
            glm::mat4(
              0.5f, 0.f, 0.f, 0.0f,
              0.f, 0.5f, 0.f, 0.0f,
              0.f, 0.f, 1.0f, 0.0f,
              0.5f, 0.5f, 0.0f, 1.f
            ) *
            projection *
            walk.get_lookat()
          ) );
          sg->get_resource()->light->set(
            light_desc,
            gct::punctual_light_parameter()
              .set_type( gct::punctual_light_type::point )
              .set_light_size( light_size )
              .set_local_position( glm::vec4( walk.get_light_pos(), 1.0 ) * glm::vec4( 1.0, -1.0, 1.0, 1.0 ) )
              .set_energy( glm::vec4( walk.get_light_energy(),walk.get_light_energy(),walk.get_light_energy(), 1.0 ) )
              .set_shadow_map( *shadow_map_texture_desc )
          );
          (*sg)( rec );
          rec.copy( global_data, global_uniform );
          rec.transfer_to_graphics_barrier( global_uniform );
        }
        if( res.force_geometry || walk.camera_moved() ) {
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
        // occlusion query
        if( res.force_geometry || walk.camera_moved() ) {
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
        tone.set( rec, 0 );
        if( res.force_geometry || walk.light_moved() ) {
          const auto shadow_data = global_uniforms_t()
            .set_projection_matrix( *proj_desc )
            .set_camera_matrix( *camera_desc )
            .set_screen_to_world_matrix( *screen_to_world_desc )
            .set_eye_pos( glm::vec4( walk.get_camera_pos(), 1.0 ) )
            .set_light_count( res.light_count )
            .set_ambient( res.ambient_level )
            .set_frame_counter( frame_counter );
          shadow_mat( rec, walk.get_light_pos(), std::min( 0.1f*scale, 0.5f ), 2.f*scale, 0 );
          rec.copy( shadow_data, shadow_uniform );
          rec.transfer_to_graphics_barrier( shadow_uniform );
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
        if( res.force_geometry || walk.light_moved() || walk.camera_moved() ) {
          {
            rec.fill( extended_gbuffer->get_factory(), gct::color::special::transparent );
            rec.fill( extended_gbuffer->get_factory(), gct::color::special::transparent );
            rec.barrier(
              gct::syncable()
                .add( extended_gbuffer )
                .add( extended_depth )
            );
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
            (*il)(
              rec,
              geometry
            );
          }
          if( walk.get_current_camera() == 0 ) {
            sg->rotate_visibility( rec );
          }
          rec.barrier( extended_gbuffer );
          rec.barrier( extended_depth );
        }
          
        rec.convert_image( flare.get_image_view()->get_factory(), vk::ImageLayout::eColorAttachmentOptimal );
        flare( rec );
        rec.convert_image( flare.get_image_view()->get_factory(), vk::ImageLayout::eGeneral );
        rec.convert_image( sb.get_image_view()->get_factory(), vk::ImageLayout::eColorAttachmentOptimal );
        sb( rec );
        rec.convert_image( sb.get_image_view()->get_factory(), vk::ImageLayout::eGeneral );
        rec.barrier(
          gct::syncable()
            .add( flare.get_image_view() )
            .add( sb.get_image_view() )
        );
        
        glm::ivec2 focus( res.width/2, res.height/2 );
        
        update_af( rec, 0, 16, 16, 1u );
        rec.compute_barrier( af_state_buffer );

        compiled( rec );
        
        rec.compute_barrier(
          gct::syncable()
            .add( merged_view )
            .add( bloom_view )
        );
        
        tone.get( rec, 0 );
      }
      command_buffer->execute(
        gct::submit_info_t()
          .add_signal_to( sync.render_complete )
      );
    }
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
        .add_wait_for( sync.render_complete, vk::PipelineStageFlagBits::eAllCommands )
        .add_wait_for( sync.image_acquired, vk::PipelineStageFlagBits::eColorAttachmentOutput )
        .add_signal_to( sync.draw_complete )
    );
    res.queue->present(
      gct::present_info_t()
        .add_wait_for( sync.draw_complete )
        .add_swapchain( res.swapchain, image_index )
    );
    command_buffer->wait_for_executed();
    sync.command_buffer->wait_for_executed();
    const auto end_date = std::chrono::high_resolution_clock::now();
    average = ( average * std::min( frame_counter, 300u ) + std::chrono::duration_cast< std::chrono::microseconds >( end_date - begin_date ).count() )/( std::min( frame_counter, 300u ) + 1 );
    if( frame_counter % 300 == 0 ) {
      std::cout << "elapsed : " << average << std::endl;
    }
    glfwPollEvents();
    ++current_frame;
    ++frame_counter;
    walk.reset_flags();
    current_frame %= framebuffers.size();
   // break;
  }
  (*res.queue)->waitIdle();
  walk.save( res.walk_state_filename );
}

