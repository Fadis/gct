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
#include <gct/cubemap_matrix_generator2.hpp>
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
#include <gct/get_library_path.hpp>
#include <gct/color_attachment_name.hpp>
#include <gct/gbuffer_format.hpp>

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
      VK_EXT_MESH_SHADER_EXTENSION_NAME
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

  const auto sg = std::make_shared< gct::scene_graph::scene_graph >(
    gct::scene_graph::scene_graph_create_info()
      .set_allocator_set( res.allocator_set )
      .add_master_shader( gct::get_system_shader_path() / "scene_graph" / "dummy" )
      .add_master_shader( gct::get_system_shader_path() / "occlusion_culling" / "roc" / "1.0" )
      .set_enable_linear( true )
  );

  gct::cubemap_matrix_generator2 shadow_mat2(
    gct::cubemap_matrix_generator2_create_info()
      .set_allocator_set( res.allocator_set )
      .set_scene_graph( sg->get_resource() )
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
      .set_layer( 6u )
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
      .set_clear_color( gct::color::web::white )
  );

  gct::cubemap_images2 cubemap_images( shadow_gbuffer.get_image_views() );
 
  auto linear_repeat_sampler_desc= sg->get_resource()->sampler->allocate(
    gct::sampler_create_info_t()
      .set_basic(
        vk::SamplerCreateInfo()
          .setMagFilter( vk::Filter::eLinear )
          .setMinFilter( vk::Filter::eLinear )
          .setMipmapMode( vk::SamplerMipmapMode::eLinear )
          .setAddressModeU( vk::SamplerAddressMode::eRepeat )
          .setAddressModeV( vk::SamplerAddressMode::eRepeat )
          .setAddressModeW( vk::SamplerAddressMode::eRepeat )
          .setAnisotropyEnable( false )
          .setCompareEnable( false )
          .setMipLodBias( 0.f )
          .setMinLod( 0.f )
          .setMaxLod( VK_LOD_CLAMP_NONE )
          .setBorderColor( vk::BorderColor::eFloatTransparentBlack )
          .setUnnormalizedCoordinates( false )
      )
  );


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

  const auto random_desc = sg->get_resource()->image->allocate(
    gct::image_allocate_info()
      .set_create_info(
        gct::image_create_info_t()
          .set_basic(
            gct::basic_2d_image( 128u, 128u )
              .setFormat( vk::Format::eR16Sfloat )
              .setUsage(
                vk::ImageUsageFlagBits::eSampled|
                vk::ImageUsageFlagBits::eStorage|
                vk::ImageUsageFlagBits::eTransferSrc|
                vk::ImageUsageFlagBits::eTransferDst
              )
          )
      )
      .set_layout(
        vk::ImageLayout::eGeneral
      )
  );
  const auto random = sg->get_resource()->image->get( random_desc.linear );
 
  const auto depth_desc = sg->get_resource()->image->allocate(
    depth_gbuffer.get_depth_views()[ 0 ]
  );

  const auto fur_shell_desc = sg->get_resource()->image->allocate(
    gct::image_allocate_info()
      .set_create_info(
        gct::image_create_info_t()
          .set_basic(
            gct::basic_2d_image( 128u, 128u )
              .setFormat( vk::Format::eR16Sfloat )
              .setUsage(
                vk::ImageUsageFlagBits::eSampled|
                vk::ImageUsageFlagBits::eStorage|
                vk::ImageUsageFlagBits::eTransferSrc|
                vk::ImageUsageFlagBits::eTransferDst
              )
          )
      )
      .set_layout(
        vk::ImageLayout::eGeneral
      )
  );
  const auto fur_shell = sg->get_resource()->image->get( fur_shell_desc.linear );
  const auto fur_shell_texture_desc = sg->get_resource()->texture->allocate(
    linear_repeat_sampler_desc, fur_shell_desc.linear
  );

  const auto fur_fin_desc = sg->get_resource()->image->allocate(
    gct::image_allocate_info()
      .set_create_info(
        gct::image_create_info_t()
          .set_basic(
            gct::basic_2d_image( 128u, 128u )
              .setFormat( vk::Format::eR16Sfloat )
              .setUsage(
                vk::ImageUsageFlagBits::eSampled|
                vk::ImageUsageFlagBits::eStorage|
                vk::ImageUsageFlagBits::eTransferSrc|
                vk::ImageUsageFlagBits::eTransferDst
              )
          )
      )
      .set_layout(
        vk::ImageLayout::eGeneral
      )
  );
  const auto fur_fin = sg->get_resource()->image->get( fur_fin_desc.linear );
  const auto fur_fin_texture_desc = sg->get_resource()->texture->allocate(
    linear_repeat_sampler_desc, fur_fin_desc.linear
  );

  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto rec = command_buffer->begin();
      rec.convert_image(
        random->get_factory(), vk::ImageLayout::eGeneral
      );
      rec.convert_image(
        fur_shell->get_factory(), vk::ImageLayout::eGeneral
      );
      rec.convert_image(
        fur_fin->get_factory(), vk::ImageLayout::eGeneral
      );
    }
    command_buffer->execute_and_wait();
  }

  auto generate_random = gct::compute(
    gct::compute_create_info()
      .set_allocator_set( res.allocator_set )
      .set_shader( gct::get_system_shader_path() / "generate_random" / "1.0" / "generate.comp.spv" )
      .set_scene_graph( sg->get_resource() )
  );
  generate_random.set_push_constant( "dest", *random_desc.linear );


  auto generate_fur_shell = gct::compute(
    gct::compute_create_info()
      .set_allocator_set( res.allocator_set )
      .set_shader( gct::get_system_shader_path() / "box_filter" / "3x3" / "1.0" / "filter.comp.spv" )
      .set_scene_graph( sg->get_resource() )
  );
  generate_fur_shell.set_push_constant( "src", *random_desc.linear );
  generate_fur_shell.set_push_constant( "dest", *fur_shell_desc.linear );
  
  auto generate_fur_fin = gct::compute(
    gct::compute_create_info()
      .set_allocator_set( res.allocator_set )
      .set_shader( gct::get_system_shader_path() / "generate_fur_texture" / "1.0" / "fin.comp.spv" )
      .set_scene_graph( sg->get_resource() )
  );
  generate_fur_fin.set_push_constant( "src", *fur_shell_desc.linear );
  generate_fur_fin.set_push_constant( "dest", *fur_fin_desc.linear );
  generate_fur_fin.set_push_constant( "shell_texture_clamp_max", 0.8f );
  generate_fur_fin.set_push_constant( "shell_texture_clamp_min", 0.7f );
  
  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto rec = command_buffer->begin();
      generate_random( rec, 0u, 128u, 128u, 1u );
      rec.barrier(
        gct::syncable()
          .add( random )
      );
      generate_fur_shell( rec, 0u, 128u, 128u, 1u );
      rec.barrier(
        gct::syncable()
          .add( fur_shell )
      );
      generate_fur_fin( rec, 0u, 128u, 128u, 1u );
      rec.barrier(
        gct::syncable()
          .add( fur_fin )
      );
      rec.dump_image(
        res.allocator,
        fur_fin->get_factory(),
        "fin.png",
        0u,
        0u
      );
      (*sg)( rec );
    }
    command_buffer->execute_and_wait();
  }

  const auto ppll_state_desc = sg->get_resource()->ppll_state->allocate();

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

  const auto aabb_csg = std::make_shared< gct::scene_graph::compiled_aabb_scene_graph >(
    gct::scene_graph::compiled_aabb_scene_graph_create_info()
      .set_shader( gct::get_system_shader_path() / "occlusion_culling" / "roc" / "1.0" )
      .set_render_pass( aabb_gbuffer.get_render_pass() )
      .set_dynamic_cull_mode( true ),
    *sg
  );

  std::vector< std::shared_ptr< gct::gltf::gltf2 > > doc;

  if( res.model_filename_list.size() != 2u ) {
    std::abort();
  }

  {
    doc.push_back( std::make_shared< gct::gltf::gltf2 >(
      gct::gltf::gltf2_create_info()
        .set_filename( res.model_filename_list[ 0 ] )
        .set_graph( sg )
        .set_root( sg->get_root_node() )
        .set_aspect_ratio( float( res.width )/float( res.height ) )
    ) );
  }
  {
    doc.push_back( std::make_shared< gct::gltf::gltf2 >(
      gct::gltf::gltf2_create_info()
        .set_filename( res.model_filename_list[ 1 ] )
        .set_graph( sg )
        .set_root( sg->get_root_node() )
        .set_aspect_ratio( float( res.width )/float( res.height ) )
        .set_enable_vertex_to_primitive( true )
        .set_enable_same_position( true )
        .set_enable_adjacency( true )
    ) );
  }

  std::vector< std::shared_ptr< gct::scene_graph::instance_list > > il;

  for( auto &d: doc ) {
    il.push_back( std::make_shared< gct::scene_graph::instance_list >(
      gct::scene_graph::instance_list_create_info()
        .set_parallel_mode3( true ),
      *sg,
      d->get_descriptor()
    ) );
  }

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
      gct::get_system_shader_path() / "scene_graph" / "dummy",
      gct::get_system_shader_path() / "occlusion_culling" / "roc" / "1.0"
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
  
  std::shared_ptr< gct::mappable_buffer_t > af_state_buffer =
    res.allocator->create_mappable_buffer(
      sizeof( gct::af_state ),
      vk::BufferUsageFlagBits::eStorageBuffer
    );
 
  const auto unproject_desc = sg->get_resource()->matrix->allocate();

  gct::hbao2 hbao(
    gct::hbao2_create_info()
      .set_allocator_set( res.allocator_set )
      .set_unproject( unproject_desc )
      .set_scene_graph( sg->get_resource() )
  );

  gct::skyview skyview(
    gct::skyview_create_info()
      .set_allocator_set( res.allocator_set )
      .set_format( vk::Format::eR16G16B16A16Sfloat )
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
      .set_shader( gct::get_system_shader_path() / "skyview" / "kplus" / "1.0" )
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
      .set_shader( gct::get_system_shader_path() / "gauss" / "coc" )
      .set_scene_graph( sg->get_resource() )
      .add_resource( { "global_uniforms", global_uniform } )
      .set_node_name( "coc_gauss" )
  );
  
  gct::lrlf_dof dof(
    gct::lrlf_dof_create_info()
      .set_allocator_set( res.allocator_set )
      .set_scene_graph( sg->get_resource() )
      .add_resource( { "global_uniforms", global_uniform } )
      .set_node_name( "dof" )
  );
  
  gct::gauss bloom_gauss(
    gct::gauss_create_info()
      .set_allocator_set( res.allocator_set )
      .set_shader( gct::get_system_shader_path() / "gauss" / "12_32" )
      .set_scene_graph( sg->get_resource() )
      .set_node_name( "bloom_gauss" )
  );
  
  const auto bg_gbuffer_format =
      gct::gbuffer_format::albedo_alpha |
      gct::gbuffer_format::normal |
      gct::gbuffer_format::emissive_occlusion |
      gct::gbuffer_format::metallic_roughness_id;
 
  const auto fur_gbuffer_format =
      gct::gbuffer_format::albedo_alpha |
      gct::gbuffer_format::tangent |
      gct::gbuffer_format::emissive_occlusion |
      gct::gbuffer_format::metallic_roughness_id;

  std::shared_ptr< gct::shader_graph::compiled > generate_gbuffer;
  gct::image_pool::image_descriptor bg_gbuffer_desc;
  gct::image_pool::image_descriptor bg_depth_desc;
  gct::image_pool::image_descriptor fur_color_desc;
  {
    gct::shader_graph::builder builder( sg->get_resource() );
        
    auto standard = std::make_shared< gct::graphics >(
      gct::graphics_create_info()
        .set_swapchain_image_count( 1u )
        .add_shader( gct::get_system_shader_path() / "generate_k+buffer" / "standard" / "2.0" )
        .use_dynamic_rendering(
          vk::Format::eR16G16B16A16Sfloat,
          vk::Format::eD32Sfloat
        )
        .enable_depth_test()
        .set_scene_graph( sg->get_resource() )
        .add_resource( { "global_uniforms", global_uniform } )
    );
    
    const auto bg0_desc = builder.call(
      builder.get_image_io_create_info(
        standard,
        gct::image_io_plan()
          .add_output(
            "gbuffer", res.width, res.height, 1u,
            4u * 8u + 1u,
            vk::Format::eR16G16B16A16Sfloat
          )
          .add_output( "position", res.width, res.height, 1u, 4u, vk::Format::eR32Sfloat )
          .add_output( "depth", res.width, res.height, vk::Format::eD32Sfloat )
          .set_dim( il[ 0 ]->get_shape() )
          .set_node_name( "bg0" )
      )
      .set_push_constant( "gbuffer_format", bg_gbuffer_format )
    )();
    const auto bg1_desc = builder.call(
      builder.get_image_io_create_info(
        standard,
        gct::image_io_plan()
          .add_inout( "gbuffer" )
          .add_inout( "position" )
          .add_inout( "depth" )
          .set_dim( il[ 1 ]->get_shape() )
          .set_node_name( "bg1" )
      )
      .set_push_constant( "gbuffer_format", bg_gbuffer_format )
    )(
      bg0_desc
    );
  
    const auto shell_desc = builder.call(
      builder.get_image_io_create_info(
        std::make_shared< gct::graphics >(
          gct::graphics_create_info()
            .set_swapchain_image_count( 1u )
            .add_shader( gct::get_system_shader_path() / "forward_rendering" / "shell" )
            .use_dynamic_rendering(
              vk::Format::eR16G16B16A16Sfloat,
              vk::Format::eD32Sfloat
            )
            .disable_depth_write()
            .use_color_blend( gct::common_color_blend_mode::add )
            .set_scene_graph( sg->get_resource() )
            .add_resource( { "global_uniforms", global_uniform } )
        ),
        gct::image_io_plan()
          .add_output( "output_color", res.width, res.height , 1u, 1u, vk::Format::eR16G16B16A16Sfloat )
          .add_inout( "depth" )
          .set_dim( il[ 1 ]->get_shape() )
          .set_loop( 15u )
          .set_node_name( "shell" )
      )
      .set_push_constant( "shell_thickness", 0.1f )
      .set_push_constant( "light", 0u )
    )(
      gct::shader_graph::vertex::combined_result_type()
        .add( "depth", bg1_desc[ "depth" ] )
    );


    builder.output( shell_desc[ "output_color" ] );
    builder.output( bg1_desc[ "gbuffer" ] );
    builder.output( bg1_desc[ "position" ] );
    generate_gbuffer = std::make_shared< gct::shader_graph::compiled >( builder() );
    bg_gbuffer_desc = generate_gbuffer->get_image_descriptor( bg1_desc[ "gbuffer" ] ); 
    bg_depth_desc = generate_gbuffer->get_image_descriptor( bg1_desc[ "position" ] ); 
    fur_color_desc = generate_gbuffer->get_image_descriptor( shell_desc[ "output_color" ] ); 
    std::cout << builder.get_log() << std::endl;
  }
  const auto bg_gbuffer = sg->get_resource()->image->get( bg_gbuffer_desc );
  const auto bg_depth = sg->get_resource()->image->get( bg_depth_desc );
  const auto fur_color = sg->get_resource()->image->get( fur_color_desc );

  std::shared_ptr< gct::mappable_buffer_t > shadow_uniform;
  shadow_uniform =
    res.allocator->create_mappable_buffer(
      sizeof( global_uniforms_t ),
      vk::BufferUsageFlagBits::eUniformBuffer
    );

  std::shared_ptr< gct::descriptor_set_t > shadow_descriptor_set;
  shadow_descriptor_set =
    res.descriptor_pool->allocate(
      global_descriptor_set_layout
    );
  shadow_descriptor_set->update({
    { "global_uniforms", shadow_uniform }
  });

  gct::graphics shadow(
    gct::graphics_create_info()
      .set_pipeline_create_info(
        gct::graphics_pipeline_create_info_t()
          .set_gbuffer( shadow_gbuffer )
      )
      .add_shader( gct::get_system_shader_path() / "generate_shadow" / "standard" )
      .set_scene_graph( sg->get_resource() )
      .add_resource( { "global_uniforms", shadow_uniform } )
  );

  gct::graphics depth(
    gct::graphics_create_info()
      .set_pipeline_create_info(
        gct::graphics_pipeline_create_info_t()
          .set_gbuffer( depth_gbuffer )
      )
      .add_shader( gct::get_system_shader_path() / "generate_depth" / "standard" )
      .set_scene_graph( sg->get_resource() )
      .add_resource( { "global_uniforms", shadow_uniform } )
  );

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

  std::shared_ptr< gct::shader_graph::compiled > compiled;
  std::shared_ptr< gct::image_view_t > merged_view;
  std::shared_ptr< gct::image_view_t > bloom_view;
  {
    gct::shader_graph::builder builder( sg->get_resource() );

    const auto bg_lighting_desc = builder.call(
      builder.get_image_io_create_info(
        std::make_shared< gct::compute >(
          gct::compute_create_info()
            .set_allocator_set( res.allocator_set )
            .set_shader( gct::get_system_shader_path() / "lighting" / "kplus" / "2.0" / "lighting.comp.spv" )
            .set_scene_graph( sg->get_resource() )
            .add_resource( { "global_uniforms", global_uniform } )
        ),
        gct::image_io_plan()
          .add_input( "gbuffer" )
          .add_input( "position" )
          .add_output( "dest", "gbuffer", { 1.f, -4.f }, vk::Format::eR16G16B16A16Sfloat )
          .set_dim( "gbuffer", { 1.f, -4.f } )
          .set_node_name( "bg_lighting" )
      )
      .set_push_constant( "unproject", *unproject_to_world_desc )
      .set_push_constant( "light", 0u )
      .set_push_constant( "gbuffer_format", bg_gbuffer_format )
    )(
      gct::shader_graph::vertex::combined_result_type()
        .add( "gbuffer", bg_gbuffer_desc )
        .add( "position", bg_depth_desc )
    );
    
    const auto np_desc = builder.call(
      builder.get_image_io_create_info(
        std::make_shared< gct::compute >(
          gct::compute_create_info()
            .set_allocator_set( res.allocator_set )
            .set_shader( gct::get_system_shader_path() / "nearest_position" / "kplus" / "2.0" / "nearest_position.comp.spv" )
            .set_scene_graph( sg->get_resource() )
        ),
        gct::image_io_plan()
          .add_input( "gbuffer" )
          .add_input( "position" )
          .add_output( "dest", "gbuffer", glm::vec2( 1.f, -1.f ), vk::Format::eR32Sfloat )
          .set_dim( "gbuffer", glm::vec2( 1.f, -1.f ) )
          .set_node_name( "nearest_position" )
      )
      .set_push_constant( "gbuffer_format", bg_gbuffer_format )
    )(
      gct::shader_graph::vertex::combined_result_type()
        .add( "gbuffer", bg_gbuffer_desc )
        .add( "position", bg_depth_desc )
    );
 
    const auto ao_out_desc = hbao(
      builder,
      np_desc
    );
    const auto skyview_froxel_out_desc = skyview_froxel2(
      builder,
      bg_gbuffer_desc,
      bg_depth_desc,
      transmittance_desc,
      bg_gbuffer_format
    );
 
    const auto mix_ao_desc = builder.call(
      builder.get_image_io_create_info(
        std::make_shared< gct::compute >(
          gct::compute_create_info()
            .set_allocator_set( res.allocator_set )
            .set_shader( CMAKE_CURRENT_BINARY_DIR "/mix_ao/mix_ao.comp.spv" )
            .set_scene_graph( sg->get_resource() )
            .add_resource( { "global_uniforms", global_uniform } )
            .add_resource( { "af_state", af_state_buffer } )
        ),
        gct::image_io_plan()
          .add_input( "bg_gbuffer" )
          .add_input( "bg_position" )
          .add_input( "occlusion" )
          .add_input( "scattering" )
          .add_input( "bg_lighting_image" )
          .add_input( "fur_color" )
          .add_output( "coc_image", "bg_gbuffer", glm::vec2( 1.0f, -2.0f ), vk::Format::eR16G16B16A16Sfloat )
          .add_output( "dest_image", "bg_gbuffer", glm::vec2( 1.0f, -2.0f ), vk::Format::eR16G16B16A16Sfloat )
          .set_dim( "bg_gbuffer", glm::vec2( 1.0f, -1.0f ) )
          .set_node_name( "mix_ao" )
      )
      .set_push_constant( "bg_gbuffer_format", bg_gbuffer_format )
    )(
      gct::shader_graph::vertex::combined_result_type()
        .add( "bg_gbuffer", bg_gbuffer_desc )
        .add( "bg_position", bg_depth_desc )
        .add( "occlusion", ao_out_desc[ "dest" ] )
        .add( "scattering", skyview_froxel_out_desc[ "dest" ] )
        .add( "bg_lighting_image", bg_lighting_desc )
        .add( "fur_color", fur_color_desc )
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
          .set_shader( gct::get_system_shader_path() / "merge" / "2.0" / "merge.comp.spv" )
          .set_scene_graph( sg->get_resource() )
          .add_resource( { "global_uniforms", global_uniform } )
          .add_resource( { "af_state", af_state_buffer } )
          .add_resource( { "tone", tone_buffer->get_buffer() } )
      ),
      gct::image_io_plan()
        .add_input( "src" )
        .add_sampled( "skyview", linear_sampler_desc )
        .add_output( "dest", "src", glm::vec2( 1.f, -1.f ) )
        .add_output( "bloom", "src", glm::vec2( 1.f, -1.f ) )
        .set_dim( "src", glm::vec2( 1.f, -1.f ) )
        .set_node_name( "merge" )
    )(
      gct::shader_graph::vertex::combined_result_type()
        .add( "src", dof_desc )
        .add( "skyview", skyview_desc )
    );
 
    const auto starburst_desc = builder.call(
      builder.get_image_io_create_info(
        std::make_shared< gct::graphics >(
          gct::graphics_create_info()
            .set_swapchain_image_count( 1u )
            .add_shader( gct::get_system_shader_path() / "starburst" / "2.0" )
            .use_dynamic_rendering(
              vk::Format::eR16G16B16A16Sfloat
            )
            .disable_depth_write()
            .disable_depth_test()
            .use_color_blend( gct::common_color_blend_mode::add )
            .set_scene_graph( sg->get_resource() )
            .add_resource( { "global_uniforms", global_uniform } )
            .add_resource( { "af_state", af_state_buffer } )
        ),
        gct::image_io_plan()
          .add_inout( "output_color" )
          .set_dim( 1u, 1u, 1u )
          .set_node_name( "starburst" )
      )
      .set_push_constant( "light", 0u )
      .set_push_constant( "texture_id", *sb_texture_desc.linear )
      .set_push_constant(
        "sensor_size", glm::vec2( 0.036f, 0.036f*9.0f/16.0f )
      )
    )(
      gct::shader_graph::vertex::combined_result_type()
        .add( "output_color", merge_desc[ "dest" ] )
    );
    
    const auto flare_desc = builder.call(
      builder.get_image_io_create_info(
        std::make_shared< gct::graphics >(
          gct::graphics_create_info()
            .set_swapchain_image_count( 1u )
            .add_shader( gct::get_system_shader_path() / "lens_flare" / "prtlf" / "2.0" )
            .use_dynamic_rendering(
              vk::Format::eR16G16B16A16Sfloat
            )
            .disable_depth_write()
            .disable_depth_test()
            .use_color_blend( gct::common_color_blend_mode::add )
            .set_scene_graph( sg->get_resource() )
            .add_resource( { "global_uniforms", global_uniform } )
            .add_resource( { "af_state", af_state_buffer } )
        ),
        gct::image_io_plan()
          .add_inout( "output_color" )
          .set_dim( 36u, 1u, 1u )
          .set_node_name( "starburst" )
      )
      .set_push_constant( "light", 0u )
      .set_push_constant( "texture_id", *flare_texture_desc.linear )
      .set_push_constant(
        "sensor_size", glm::vec2( 0.036f, 0.036f*9.0f/16.0f )
      )
    )(
      gct::shader_graph::vertex::combined_result_type()
        .add( "output_color", starburst_desc )
    );
 
 
 
    const auto filtered_bloom = bloom_gauss( builder, merge_desc[ "bloom" ] );
 
    builder.output( flare_desc[ "output_color" ] );
    builder.output( filtered_bloom );
    compiled = std::make_shared< gct::shader_graph::compiled >( builder() );
    merged_view = compiled->get_view( flare_desc[ "output_color" ] );
    bloom_view = compiled->get_view( filtered_bloom );
    std::cout << builder.get_log() << std::endl;
  }

  auto update_af = gct::compute(
    gct::compute_create_info()
      .set_allocator_set( res.allocator_set )
      .set_shader( gct::get_system_shader_path() / "tone_mapping" / "1.0" / "distance.comp.spv" )
      .set_swapchain_image_count( 1u )
      .set_scene_graph( sg->get_resource() )
      .add_resource( { "af_state", af_state_buffer } )
  );
  update_af.set_push_constant( "focus_pos", glm::ivec2( res.width/2, res.height/2 ) );
  update_af.set_push_constant( "gbuffer", *bg_gbuffer_desc );
  update_af.set_push_constant( "position", *bg_depth_desc );
  update_af.set_push_constant( "gbuffer_format", bg_gbuffer_format );
  
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
      .set_allocator_set( res.allocator_set )
      .set_shader( gct::get_system_shader_path() / "tone_mapping" / "1.0" / "tone.comp.spv" )
      .set_input( std::vector< std::shared_ptr< gct::image_view_t > >{ merged_view } )
      .set_output( tone_buffer )
  );

  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( merged_view, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }

  auto generate_meshlet_info = gct::compute(
    gct::compute_create_info()
      .set_allocator_set( res.allocator_set )
      .set_shader( gct::get_system_shader_path() / "meshlet_normal" / "1.0" / "meshlet_normal.comp.spv" )
      .set_swapchain_image_count( 1u )
      .set_scene_graph( sg->get_resource() )
  );
  
  auto same_position = gct::compute(
    gct::compute_create_info()
      .set_allocator_set( res.allocator_set )
      .set_shader( gct::get_system_shader_path() / "vertex_to_primitive" / "same_position.comp.spv" )
      .set_swapchain_image_count( 1u )
      .set_scene_graph( sg->get_resource() )
      .add_resource( { "global_uniforms", global_uniform } )
  );
  
  auto vertex_to_primitive = gct::compute(
    gct::compute_create_info()
      .set_allocator_set( res.allocator_set )
      .set_shader( gct::get_system_shader_path() / "vertex_to_primitive" / "vertex_to_primitive.comp.spv" )
      .set_swapchain_image_count( 1u )
      .set_scene_graph( sg->get_resource() )
      .add_resource( { "global_uniforms", global_uniform } )
  );
  
  auto generate_adjacency = gct::compute(
    gct::compute_create_info()
      .set_allocator_set( res.allocator_set )
      .set_shader( gct::get_system_shader_path() / "generate_adjacency" / "1.0" / "generate_adjacency.comp.spv" )
      .set_swapchain_image_count( 1u )
      .set_scene_graph( sg->get_resource() )
      .add_resource( { "global_uniforms", global_uniform } )
  );

  const auto &il1_dl = il[ 1 ]->get_draw_list();
  if( il1_dl.size() != 1u ) throw -1;
  const auto il1_prim = sg->get_resource()->prim.get( il1_dl[ 0 ].prim );
  std::cout << "unique vertex count : " << il1_prim->unique_vertex_count << std::endl;
  std::cout << "vertex count : " << il1_prim->count << std::endl;

  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      for( auto &i: il ) {
        i->setup_resource_pair_buffer( recorder );
        generate_meshlet_info( recorder, 0u, i->get_mesh_count(), i->get_max_primitive_count(), 1u );
      }
      il[ 1 ]->setup_resource_pair_buffer( recorder );
      same_position( recorder, 0, il1_prim->unique_vertex_count, il1_prim->unique_vertex_count, 1u );
      recorder.barrier( sg->get_resource()->vertex_to_primitive->get_buffer() );
      vertex_to_primitive( recorder, 0, il1_prim->count / 3, 1u, 1u );
      recorder.barrier( sg->get_resource()->vertex_to_primitive->get_buffer() );
      generate_adjacency( recorder, 0, il1_prim->count / 3, 1u, 1u );
    }
    command_buffer->execute_and_wait();
  }

  const auto gamma = gct::image_filter(
    gct::image_filter_create_info()
      .set_allocator_set( res.allocator_set )
      .set_shader( gct::get_system_shader_path() / "gamma" / "1.0" / "gamma.comp.spv" )
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

  const auto shadow_camera = sg->get_resource()->matrix->allocate( 6u );
  const auto shadow_projection = sg->get_resource()->matrix->allocate();

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
            .set_frame_counter( frame_counter );
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
          sg->get_resource()->ppll_state->clear( ppll_state_desc );
          (*sg)( rec );
          rec.copy( global_data, global_uniform );
          rec.transfer_to_graphics_barrier( global_uniform );
        }
        if( res.force_geometry || walk.camera_moved() ) {
          {
            auto render_pass_token = rec.begin_render_pass(
              depth_gbuffer.get_render_pass_begin_info( 0 ),
              vk::SubpassContents::eInline
            );
            rec->setViewport( 0, 1, &depth_gbuffer.get_viewport() );
            rec->setScissor( 0, 1, &depth_gbuffer.get_scissor() );
            rec->setCullMode( vk::CullModeFlagBits::eBack );
            rec->setDepthCompareOp( vk::CompareOp::eLessOrEqual );
            il[ 0 ]->setup_resource_pair_buffer( rec );
            (*il[ 0 ])(
              rec,
              depth
            );
            il[ 1 ]->setup_resource_pair_buffer( rec );
            (*il[ 1 ])(
              rec,
              depth
            );
          }
        }
        if( res.force_geometry || walk.camera_moved() ) {
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
            il[ 0 ]->setup_resource_pair_buffer( rec );
            (*il[ 0 ])( rec, *aabb_csg );
            il[ 1 ]->setup_resource_pair_buffer( rec );
            (*il[ 1 ])( rec, *aabb_csg );
          }
          rec.convert_image(
            aabb_gbuffer.get_image( 0 ),
            vk::ImageLayout::eGeneral
          );
        }
        tone.set( rec, 0 );
        if( res.force_geometry || walk.light_moved() ) {
          const auto shadow_data = global_uniforms_t()
            .set_projection_matrix( *shadow_projection )
            .set_camera_matrix( *shadow_camera )
            .set_screen_to_world_matrix( *screen_to_world_desc )
            .set_eye_pos( glm::vec4( walk.get_light_pos(), 1.0 ) )
            .set_light_count( res.light_count )
            .set_ambient( res.ambient_level )
            .set_frame_counter( frame_counter );
          shadow_mat2(
            rec,
            walk.get_light_pos(),
            std::min( 0.1f*scale, 0.5f ),
            2.f*scale,
            shadow_camera,
            shadow_projection
          );
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
            il[ 0 ]->setup_resource_pair_buffer( rec );
            (*il[ 0 ])(
              rec,
              shadow,
              6u
            );
            il[ 1 ]->setup_resource_pair_buffer( rec );
            (*il[ 1 ])(
              rec,
              shadow,
              6u
            );
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
            rec.fill( bg_gbuffer->get_factory(), gct::color::special::transparent );
            rec.fill( bg_depth->get_factory(), gct::color::special::transparent );
            rec.barrier(
              gct::syncable()
                .add( bg_gbuffer )
                .add( bg_depth )
            );
            (*generate_gbuffer)( rec );
          }
          if( walk.get_current_camera() == 0 ) {
            sg->rotate_visibility( rec );
          }
          rec.barrier( bg_gbuffer );
          rec.barrier( bg_depth );
        }
          
        glm::ivec2 focus( res.width/2, res.height/2 );
        
        update_af( rec, 0, 16, 16, 1u );
        rec.compute_barrier( af_state_buffer );

        (*compiled)( rec );
        
        rec.compute_barrier(
          gct::syncable()
            .add( merged_view )
            .add( bloom_view )
        );
       
        tone.get( rec, 0 );

        /*if( frame_counter == 1 ) {
          rec.dump_image(
            res.allocator,
            fur_view->get_factory(),
            "fur.png",
            0u
          );
        }*/

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
    average = ( average * std::min( frame_counter, 60u ) + std::chrono::duration_cast< std::chrono::microseconds >( end_date - begin_date ).count() )/( std::min( frame_counter, 60u ) + 1 );
    if( frame_counter % 60 == 0 ) {
      std::cout << "elapsed : " << average << std::endl;
    }
    glfwPollEvents();
    ++current_frame;
    ++frame_counter;
    walk.reset_flags();
    current_frame %= framebuffers.size();
  }
  (*res.queue)->waitIdle();
  walk.save( res.walk_state_filename );
}

