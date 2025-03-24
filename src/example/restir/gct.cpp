#include <chrono>
#include <cstdint>
#include <filesystem>
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
#include <gct/voxel_image.hpp>
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
#include <gct/rtao.hpp>
#include <gct/distance_field2.hpp>
#include <gct/scene_graph.hpp>
#include <gct/gltf2.hpp>
#include <gct/gltf2_create_info.hpp>
#include <gct/image_pool.hpp>
#include <gct/compiled_scene_graph.hpp>
#include <gct/instance_list.hpp>
#include <gct/kdtree.hpp>
#include <gct/skyview.hpp>
#include <gct/skyview_froxel.hpp>

struct fb_resources_t {
  std::shared_ptr< gct::semaphore_t > image_acquired;
  std::shared_ptr< gct::semaphore_t > draw_complete;
  std::shared_ptr< gct::semaphore_t > image_ownership;
  std::shared_ptr< gct::bound_command_buffer_t > command_buffer;
  bool initial = true;
};

struct dynamic_uniforms_t {
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
      VK_NV_REPRESENTATIVE_FRAGMENT_TEST_EXTENSION_NAME
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
      .set_format( vk::Format::eR16G16B16A16Sfloat ) 
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
      .set_format( vk::Format::eR16G16B16A16Sfloat ) 
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
      .set_external_depth( depth_gbuffer.get_depth_views() )
  );

  gct::gbuffer aabb_gbuffer(
    gct::gbuffer_create_info()
      .set_allocator( res.allocator )
      .set_width( res.width )
      .set_height( res.height )
      .set_layer( 0u )
      .set_swapchain_image_count( 1u )
      .set_color_buffer_count( 1u )
      .set_format( vk::Format::eR16G16B16A16Sfloat ) 
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
      .set_external_depth( depth_gbuffer.get_depth_views() )
  );

  const auto sg = std::make_shared< gct::scene_graph::scene_graph >(
    gct::scene_graph::scene_graph_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .add_master_shader( CMAKE_CURRENT_BINARY_DIR "/geometry" )
      .add_master_shader( CMAKE_CURRENT_BINARY_DIR "/aabb" )
      .add_master_shader( CMAKE_CURRENT_BINARY_DIR "/depth" )
      .add_master_shader( CMAKE_CURRENT_BINARY_DIR "/voxel" )
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
  
  const auto aabb_csg = std::make_shared< gct::scene_graph::compiled_aabb_scene_graph >(
    gct::scene_graph::compiled_aabb_scene_graph_create_info()
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/aabb" )
      .set_render_pass( aabb_gbuffer.get_render_pass() )
      .set_dynamic_cull_mode( true ),
    *sg
  );

  gct::distance_field2 distance_field(
    gct::distance_field2_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_voronoi_shader( CMAKE_CURRENT_BINARY_DIR "/voronoi/voronoi.comp.spv" )
      .set_distance_field_shader( CMAKE_CURRENT_BINARY_DIR "/distance_field/distance_field.comp.spv" )
  );

  const auto voxel_csg = std::make_shared< gct::scene_graph::compiled_scene_graph >(
    gct::scene_graph::compiled_scene_graph_create_info()
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/voxel" )
      .set_render_pass( distance_field.get_distance_field_image().get_render_pass() )
      .set_dynamic_cull_mode( true ),
    *sg
  );
  
  const auto il = std::make_shared< gct::scene_graph::instance_list >(
    gct::scene_graph::instance_list_create_info(),
    *sg
  );

  const auto scene_aabb = sg->get_root_node()->get_initial_world_aabb();
  if( !scene_aabb ) throw -1;

  const gct::voxel_matrix voxel_matrix( scene_aabb->min, scene_aabb->max );

  const std::array< gct::matrix_pool::matrix_descriptor, 3u > voxel_proj_desc{
    sg->get_resource()->matrix->allocate( voxel_matrix.get_input_projection( 0 ) ),
    sg->get_resource()->matrix->allocate( voxel_matrix.get_input_projection( 1 ) ),
    sg->get_resource()->matrix->allocate( voxel_matrix.get_input_projection( 2 ) )
  };
  const auto voxel_desc = sg->get_resource()->matrix->allocate( voxel_matrix.get_output_projection() );
  const auto inversed_voxel_desc = sg->get_resource()->matrix->allocate( glm::inverse( voxel_matrix.get_output_projection() ) );

  auto voxel_sampler = res.device->get_sampler(
    gct::get_basic_linear_sampler_create_info()
  );

  if( std::filesystem::exists( ".light" ) ) {
    std::ifstream persistent_light( ".light" );
    nlohmann::json light_param_in_json;
    persistent_light >> light_param_in_json;
    const auto light = gct::restore_light(
      *sg->get_resource()->light,
      sg->get_root_node()->matrix,
      light_param_in_json
    );
    sg->get_root_node()->light = light;
  }
  else {
    const auto [light,light_param] = gct::generate_random_light(
      *sg->get_resource()->light,
      sg->get_root_node()->matrix,
      gct::aabb3()
        .set_min( scene_aabb->min * 0.6f )
        .set_max( scene_aabb->max * 0.6f ),
      gct::aabb3()
        .set_min( glm::vec3( 0.1f, 0.1f, 0.1f ) )
        .set_max( glm::vec3( 1.0f, 1.0f, 1.0f ) ),
      80.f,
      80.f,
      1000u
    );
    sg->get_root_node()->light = light;
    {
      std::ofstream persistent_light( ".light" );
      persistent_light << nlohmann::json( light_param ).dump( 2 ) << std::endl;
    }
  }

  const auto l1 = std::find_if(
    sg->get_root_node()->light.begin(),
    sg->get_root_node()->light.end(),
    []( const auto &v ) { return *v == 1u; }
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
      CMAKE_CURRENT_BINARY_DIR "/aabb",
      CMAKE_CURRENT_BINARY_DIR "/depth",
      CMAKE_CURRENT_BINARY_DIR "/voxel",
    },
    1u
  );
  std::shared_ptr< gct::mappable_buffer_t > global_uniform;
  std::shared_ptr< gct::descriptor_set_t > global_descriptor_set;
  global_uniform =
    res.allocator->create_mappable_buffer(
      sizeof( dynamic_uniforms_t ),
      vk::BufferUsageFlagBits::eUniformBuffer
    );
  global_descriptor_set =
    res.descriptor_pool->allocate(
      global_descriptor_set_layout
    );
  global_descriptor_set->update({
    { "global_uniforms", global_uniform },
    { "condition", sg->get_resource()->last_visibility->get_buffer() },
    { "voxel", distance_field.get_working_image().get_image(), vk::ImageLayout::eGeneral }
  });

  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto rec = command_buffer->begin();
      auto global_data = dynamic_uniforms_t()
        .set_voxel_proj( glm::ivec4( *voxel_proj_desc[ 0 ], *voxel_proj_desc[ 1 ], *voxel_proj_desc[ 2 ], 0u ) )
        .set_voxel( *voxel_desc );
      rec.copy( global_data, global_uniform );
      rec.transfer_to_graphics_barrier( { global_uniform->get_buffer() }, {} );
      distance_field.clear( rec );
      {
        auto render_pass_token = rec.begin_render_pass(
          distance_field.get_distance_field_image().get_render_pass_begin_info(),
          vk::SubpassContents::eInline
        );
        rec->setViewport( 0, { distance_field.get_distance_field_image().get_viewport() } );
        rec->setScissor( 0, { distance_field.get_distance_field_image().get_scissor() } );
        rec->setCullMode( vk::CullModeFlagBits::eNone );
        rec->setDepthCompareOp( vk::CompareOp::eAlways );
        rec.bind_descriptor_set(
          vk::PipelineBindPoint::eGraphics,
          1u,
          sg->get_resource()->pipeline_layout,
          global_descriptor_set
        );
        (*il)( rec, *voxel_csg );
      }
      rec.graphics_to_compute_barrier( {}, { distance_field.get_working_image().get_image()->get_factory() } );
      distance_field( rec );
    }
    command_buffer->execute_and_wait();
  }

  const auto rgba32ici =
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( res.width, res.height )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage|
            vk::ImageUsageFlagBits::eTransferDst|
            vk::ImageUsageFlagBits::eTransferSrc
          )
      );
  
  const auto rgba16ici =
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( res.width, res.height )
          .setFormat( vk::Format::eR16G16B16A16Sfloat )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage|
            vk::ImageUsageFlagBits::eTransferDst|
            vk::ImageUsageFlagBits::eTransferSrc
          )
      );


  const auto reservoir_ici =
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( res.width, res.height )
          .setArrayLayers( 2u )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage|
            vk::ImageUsageFlagBits::eTransferDst|
            vk::ImageUsageFlagBits::eTransferSrc
          )
      );
  

  const auto r16ici =
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( res.width, res.height )
          .setFormat( vk::Format::eR16Sfloat )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage|
            vk::ImageUsageFlagBits::eTransferDst|
            vk::ImageUsageFlagBits::eTransferSrc
          )
      );

  const auto r32uici =
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( res.width, res.height )
          .setFormat( vk::Format::eR32Uint )
          .setUsage( vk::ImageUsageFlagBits::eStorage )
      );

  const auto rg32uici =
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( res.width, res.height )
          .setFormat( vk::Format::eR32G32Uint )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage|
            vk::ImageUsageFlagBits::eTransferDst|
            vk::ImageUsageFlagBits::eTransferSrc
          )
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

  const auto previous_reservoir = res.allocator->create_image_view(
    reservoir_ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  
  const auto reservoir = res.allocator->create_image_view(
    reservoir_ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );

  const auto diffuse = res.allocator->create_image_view(
    rgba16ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  const auto specular = res.allocator->create_image_view(
    rgba16ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  const auto direct = res.allocator->create_image_view(
    rgba16ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );

  const auto previous_diffuse = res.allocator->create_image_view(
    rgba16ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  const auto previous_specular = res.allocator->create_image_view(
    rgba16ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );

  const auto temporary_diffuse = res.allocator->create_image_view(
    rgba16ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  const auto temporary_specular = res.allocator->create_image_view(
    rgba16ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );

  const auto previous_history = res.allocator->create_image_view(
    rg32uici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  const auto history = res.allocator->create_image_view(
    rg32uici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );

  const auto ao = res.allocator->create_image_view(
    r16ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );

  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( direct, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( diffuse, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( specular, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( history, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( previous_diffuse, vk::ImageLayout::eGeneral );
      recorder.fill( previous_diffuse->get_factory(), gct::color::web::black );
      recorder.set_image_layout( previous_specular, vk::ImageLayout::eGeneral );
      recorder.fill( previous_specular->get_factory(), gct::color::web::black );
      recorder.set_image_layout( previous_history, vk::ImageLayout::eGeneral );
      recorder.fill( previous_history->get_factory(), vk::ClearColorValue().setUint32( { 0u, 0u, 0u, 0u } ) );
      recorder.set_image_layout( ao, vk::ImageLayout::eGeneral );
      recorder.fill( ao->get_factory(), gct::color::web::black );
      recorder.set_image_layout( previous_reservoir, vk::ImageLayout::eGeneral );
      recorder.fill( previous_reservoir->get_factory(), gct::color::special::transparent );
      recorder.set_image_layout( reservoir, vk::ImageLayout::eGeneral );
      recorder.fill( reservoir->get_factory(), gct::color::special::transparent );
      recorder.set_image_layout( temporary_diffuse, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( temporary_specular, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }

  const auto update_reservoir = gct::compute(
    gct::compute_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/wrs/wrs.comp.spv" )
      .set_swapchain_image_count( 1u )
      .add_resource( { "gbuffer", gbuffer.get_image_views(), vk::ImageLayout::eGeneral } )
      .add_resource( { "previous_reservoir", previous_reservoir } )
      .add_resource( { "reservoir", reservoir } )
      .add_resource( { "global_uniforms", global_uniform } )
      .add_resource( { "light", sg->get_resource()->light->get_buffer() } )
      .add_resource( { "active_light", sg->get_resource()->light->get_active_light_buffer() } )
      .add_resource( { "matrix_pool", sg->get_resource()->matrix->get_buffer() } )
      .add_resource( { "distance_field", voxel_sampler, distance_field.get_distance_field_image().get_image(), vk::ImageLayout::eGeneral } ) 
  );

  const auto direct_light = gct::compute(
    gct::compute_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/lighting/direct_light.comp.spv" )
      .set_swapchain_image_count( 1u )
      .add_resource( { "gbuffer", gbuffer.get_image_views(), vk::ImageLayout::eGeneral } )
      .add_resource( { "direct", direct } )
      .add_resource( { "global_uniforms", global_uniform } )
      .add_resource( { "light", sg->get_resource()->light->get_buffer() } )
      .add_resource( { "active_light", sg->get_resource()->light->get_active_light_buffer() } )
      .add_resource( { "matrix_pool", sg->get_resource()->matrix->get_buffer() } )
  );

  const auto lighting = gct::compute(
    gct::compute_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/lighting/lighting.comp.spv" )
      .set_swapchain_image_count( 1u )
      .add_resource( { "gbuffer", gbuffer.get_image_views(), vk::ImageLayout::eGeneral } )
      .add_resource( { "reservoir", reservoir } )
      .add_resource( { "diffuse_image", diffuse } )
      .add_resource( { "specular_image", specular } )
      .add_resource( { "history", history } )
      .add_resource( { "previous_diffuse_image", previous_diffuse } )
      .add_resource( { "previous_specular_image", previous_specular } )
      .add_resource( { "previous_history", previous_history } )
      .add_resource( { "global_uniforms", global_uniform } )
      .add_resource( { "light", sg->get_resource()->light->get_buffer() } )
      .add_resource( { "active_light", sg->get_resource()->light->get_active_light_buffer() } )
      .add_resource( { "matrix_pool", sg->get_resource()->matrix->get_buffer() } )
      .add_resource( { "distance_field", voxel_sampler, distance_field.get_distance_field_image().get_image(), vk::ImageLayout::eGeneral } ) 
  );

  const auto hgauss = gct::compute(
    gct::compute_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/lighting/hadaptive.comp.spv" )
      .set_swapchain_image_count( 1u )
      .add_resource( { "gbuffer", gbuffer.get_image_views(), vk::ImageLayout::eGeneral } )
      .add_resource( { "diffuse_src", previous_diffuse } )
      .add_resource( { "specular_src", previous_specular } )
      .add_resource( { "history", previous_history } )
      .add_resource( { "diffuse_dest", temporary_diffuse } )
      .add_resource( { "specular_dest", temporary_specular } )
  );
  
  const auto vgauss = gct::compute(
    gct::compute_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/lighting/vadaptive.comp.spv" )
      .set_swapchain_image_count( 1u )
      .add_resource( { "gbuffer", gbuffer.get_image_views(), vk::ImageLayout::eGeneral } )
      .add_resource( { "diffuse_src", temporary_diffuse } )
      .add_resource( { "specular_src", temporary_specular } )
      .add_resource( { "history", previous_history } )
      .add_resource( { "diffuse_dest", previous_diffuse } )
      .add_resource( { "specular_dest", previous_specular } )
  );

  gct::rtao rtao(
    gct::rtao_create_info()
      .set_allocator( res.allocator )
      .set_pipeline_cache( res.pipeline_cache )
      .set_descriptor_pool( res.descriptor_pool )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/ao/" )
      .set_output( ao )
      .add_resource( { "global_uniforms", global_uniform } )
      .add_resource( { "gbuffer", gbuffer.get_image_views(), vk::ImageLayout::eGeneral } )
      .add_resource( { "matrix_pool", sg->get_resource()->matrix->get_buffer() } )
      .add_resource( { "distance_field", voxel_sampler, distance_field.get_distance_field_image().get_image(), vk::ImageLayout::eGeneral } ) 
  );

  gct::skyview skyview(
    gct::skyview_create_info()
      .set_allocator( res.allocator )
      .set_pipeline_cache( res.pipeline_cache )
      .set_descriptor_pool( res.descriptor_pool )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/skyview/" )
  );
  const auto skyview_param = gct::skyview_parameter()
    .set_sun_angle( -M_PI/3.0f );
  
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
    rgba16ici,
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
      .add_resource( { "occlusion", rtao.get_output(), vk::ImageLayout::eGeneral } )
      .add_resource( { "skyview", linear_sampler, skyview.get_output(), vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "scattering", skyview_froxel.get_output(), vk::ImageLayout::eGeneral } )
      .add_resource( { "specular_image", specular } )
      .add_resource( { "direct", direct } )
      .add_resource( { "dest_image", mixed_out } )
      .add_resource( { "tone", tone.get_buffer() } )
      .add_resource( { "dynamic_uniforms", global_uniform } )
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
      .set_input( std::vector< std::shared_ptr< gct::image_view_t > >( res.swapchain_images.size(), mixed_out ) )
      .set_output( res.swapchain_image_views )
      .add_resource( { "bloom_image", bloom_gauss.get_output(), vk::ImageLayout::eGeneral } )
  );

  const auto center = ( scene_aabb->min + scene_aabb->max ) / 2.f;
  const auto scale = std::abs( glm::length( scene_aabb->max - scene_aabb->min ) );

  const glm::mat4 projection = glm::perspective( 0.6981317007977318f, (float(res.width)/float(res.height)), std::min(0.1f*scale,0.5f), scale );

  const float light_size = 0.3;

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

  std::uint32_t current_frame = 0u;
  std::uint32_t frame_counter = 0u;
  bool update_optflow = false;
  std::minstd_rand rng;
  std::uniform_real_distribution jitter_dist( -0.0005, 0.0005 );
  while( !walk.end() ) {
    gct::blocking_timer frame_rate;
    ++walk;
    const auto global_data = dynamic_uniforms_t()
      .set_projection_matrix( *proj_desc )
      .set_camera_matrix( *camera_desc )
      .set_previous_projection_matrix( * previous_proj_desc )
      .set_previous_camera_matrix( *previous_camera_desc )
      .set_screen_to_world_matrix( *screen_to_world_desc )
      .set_voxel( *voxel_desc )
      .set_inversed_voxel( *inversed_voxel_desc )
      .set_eye_pos( glm::vec4( walk.get_camera_pos(), 1.0 ) )
      .set_light_count( res.light_count )
      .set_ambient( res.ambient_level )
      .set_frame_counter( frame_counter )
      .set_voxel_proj( glm::ivec4( *voxel_proj_desc[ 0 ], *voxel_proj_desc[ 1 ], *voxel_proj_desc[ 2 ], 0u ) );
    sg->get_resource()->matrix->set( proj_desc, projection );
    sg->get_resource()->matrix->set( camera_desc, walk.get_lookat() );
    sg->get_resource()->matrix->set( screen_to_world_desc, glm::inverse( projection * walk.get_lookat() ) );
    {
      {
        auto rec = command_buffer->begin();
        (*sg)( rec );
        rec.copy( global_data, global_uniform );
        rec.transfer_to_graphics_barrier( { global_uniform->get_buffer() }, {} );
        // depth
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
        // occlusion query
        if( walk.get_current_camera() == 0 ) {
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
        // distance field
        {
          distance_field.clear( rec );
          {
            auto render_pass_token = rec.begin_render_pass(
              distance_field.get_distance_field_image().get_render_pass_begin_info(),
              vk::SubpassContents::eInline
            );
            rec->setViewport( 0, { distance_field.get_distance_field_image().get_viewport() } );
            rec->setScissor( 0, { distance_field.get_distance_field_image().get_scissor() } );
            rec->setCullMode( vk::CullModeFlagBits::eNone );
            rec->setDepthCompareOp( vk::CompareOp::eAlways );
            rec.bind_descriptor_set(
              vk::PipelineBindPoint::eGraphics,
              1u,
              sg->get_resource()->pipeline_layout,
              global_descriptor_set
            );
            (*il)( rec, *voxel_csg, false );
          }
          rec.graphics_to_compute_barrier( {}, { distance_field.get_working_image().get_image()->get_factory() } );
          distance_field( rec );
        }
        tone.set( rec, 0 );
        {
          if( !update_optflow ) {
            update_optflow = true;
          }
          else {
            update_optflow = false;
          }
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
            (*il)( rec, *geometry_csg, true );
          }
          if( walk.get_current_camera() == 0 ) {
            sg->rotate_visibility( rec );
          }
          rec.convert_image(
            gbuffer.get_image( 0 ),
            vk::ImageLayout::eGeneral
          );
        }
        rec.barrier(
          {},
          {
            reservoir->get_factory()
          }
        );
        hgauss( rec, 0, res.width, res.height, 1u );
        rec.barrier(
          {},
          {
            temporary_diffuse->get_factory(),
            temporary_specular->get_factory()
          }
        );
        vgauss( rec, 0, res.width, res.height, 1u );
        rec.barrier(
          {},
          {
            previous_diffuse->get_factory(),
            previous_specular->get_factory()
          }
        );
        update_reservoir( rec, 0, res.width, res.height, 1u );
        rec.copy( reservoir->get_factory(), previous_reservoir->get_factory() );
        rec.fill( direct->get_factory(), gct::color::web::black );
        rec.barrier( {}, { direct->get_factory() } );
        direct_light( rec, 0, 1000u, 1u, 1u );
        lighting( rec, 0, res.width, res.height, 1u );
        rtao( rec );
        rec.barrier(
          {},
          {
            direct->get_factory(),
            diffuse->get_factory(),
            specular->get_factory()
          }
        );
        rec.copy( diffuse->get_factory(), previous_diffuse->get_factory() );
        rec.copy( specular->get_factory(), previous_specular->get_factory() );
        rec.copy( history->get_factory(), previous_history->get_factory() );
        
        if( walk.light_moved() || walk.camera_moved() || frame_counter <= 2u ) {
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
          rec.barrier(
            {},
            { gbuffer.get_image( 0 ) }
          );
          rec.fill(
            gbuffer.get_image( 0 ),
            gct::color::web::black,
            vk::ImageSubresourceRange()
              .setBaseArrayLayer( 5u )
              .setLayerCount( 1u )
          );
        }
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

