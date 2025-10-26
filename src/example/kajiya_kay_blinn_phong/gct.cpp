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
      VK_EXT_MESH_SHADER_EXTENSION_NAME,
      VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
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
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .add_master_shader( gct::get_system_shader_path() / "scene_graph" / "dummy" )
      .set_enable_linear( true )
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
    { "global_uniforms", global_uniform }
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

  auto linear_repeat_sampler_desc= sg->get_resource()->sampler->allocate(
    gct::sampler_create_info_t()
      .set_basic(
        vk::SamplerCreateInfo()
          .setMagFilter( vk::Filter::eLinear )
          .setMinFilter( vk::Filter::eLinear )
          .setMipmapMode( vk::SamplerMipmapMode::eLinear )
          .setAddressModeU( vk::SamplerAddressMode::eRepeat )
          .setAddressModeV( vk::SamplerAddressMode::eClampToEdge )
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
      (*sg)( rec );
    }
    command_buffer->execute_and_wait();
  }

  std::shared_ptr< gct::shader_graph::compiled > geometry;
  gct::image_pool::image_descriptor shaded_desc;
  {
    gct::shader_graph::builder builder( sg->get_resource() );
  
    const auto render = std::make_shared< gct::graphics >(
      gct::graphics_create_info()
        .set_swapchain_image_count( 1u )
        .add_shader( gct::get_system_shader_path() / "forward_rendering" / "standard" )
        .use_dynamic_rendering(
          vk::Format::eR16G16B16A16Sfloat,
          vk::Format::eD32Sfloat,
          vk::Format::eUndefined
        )
        .set_scene_graph( sg->get_resource() )
        .add_resource( { "global_uniforms", global_uniform } )
    );
 
    const auto lighting1_desc = builder.call(
      builder.get_image_io_create_info(
        render,
        gct::image_io_plan()
          .add_output( "output_color", res.width, res.height, vk::Format::eR16G16B16A16Sfloat )
          .add_output( "depth", res.width, res.height, vk::Format::eD32Sfloat )
          .set_dim( il[ 0 ]->get_shape() )
          .set_node_name( "lighting" )
      )
      .set_push_constant( "light", 0u )
    )();
 
    const auto lighting2_desc = builder.call(
      builder.get_image_io_create_info(
        render,
        gct::image_io_plan()
          .add_inout( "output_color" )
          .add_inout( "depth" )
          .set_dim( il[ 1 ]->get_shape() )
          .set_node_name( "bunny" )
      )
      .set_push_constant( "light", 0u )
    )(
      lighting1_desc
    );
 
    const auto shell2_desc = builder.call(
      builder.get_image_io_create_info(
        std::make_shared< gct::graphics >(
          gct::graphics_create_info()
            .set_swapchain_image_count( 1u )
            .add_shader( gct::get_system_shader_path() / "forward_rendering" / "kajiya_kay_blinn_phong" / "1.0" )
            .use_dynamic_rendering(
              vk::Format::eR16G16B16A16Sfloat,
              vk::Format::eD32Sfloat,
              vk::Format::eUndefined
            )
            .disable_depth_write()
            .use_color_blend( gct::common_color_blend_mode::rgb )
            .set_scene_graph( sg->get_resource() )
            .add_resource( { "global_uniforms", global_uniform } )
        ),
        gct::image_io_plan()
          .add_inout( "output_color" )
          .add_inout( "depth" )
          .set_dim( il[ 1 ]->get_shape() )
          .set_node_name( "shell" )
          .set_loop( 15u )
      )
      .set_push_constant( "light", 0u )
      .set_push_constant( "shell_thickness", 0.1f )
      .set_push_constant( "shell_texture", *fur_shell_texture_desc )
      .set_push_constant( "shell_texture_clamp_max", 0.60f )
      .set_push_constant( "shell_texture_clamp_min", 0.52f )
    )(
      lighting2_desc
    );
 
    builder.output( shell2_desc[ "output_color" ] );
    geometry = std::make_shared< gct::shader_graph::compiled >( builder() );
    shaded_desc = geometry->get_image_descriptor( shell2_desc[ "output_color" ] );
    std::cout << builder.get_log() << std::endl;
  }
  auto shaded_view = sg->get_resource()->image->get( shaded_desc );

  std::shared_ptr< gct::shader_graph::compiled > post_process;
  std::shared_ptr< gct::image_view_t > merged_view;
  std::shared_ptr< gct::image_view_t > bloom_view;
  {
    gct::shader_graph::builder builder( sg->get_resource() );

    const auto apply_tone_desc = builder.call(
      std::make_shared< gct::compute >(
        gct::compute_create_info()
          .set_allocator_set( res.allocator_set )
          .set_shader( gct::get_system_shader_path() / "tone_mapping" / "1.0" / "apply.comp.spv" )
          .set_scene_graph( sg->get_resource() )
          .add_resource( { "global_uniforms", global_uniform } )
          .add_resource( { "tone", tone_buffer->get_buffer() } )
      ),
      gct::image_io_plan()
        .add_input( "src" )
        .add_output( "dest", "src", glm::vec2( 1.f, -1.f ) )
        .add_output( "bloom", "src", glm::vec2( 1.f, -1.f ) )
        .set_dim( "src", glm::vec2( 1.f, -1.f ) )
        .set_node_name( "merge" )
    )(
      gct::shader_graph::vertex::combined_result_type()
        .add( "src", shaded_desc )
    );
    builder.output( apply_tone_desc[ "dest" ] );
    builder.output( apply_tone_desc[ "bloom" ] );
    post_process = std::make_shared< gct::shader_graph::compiled >( builder() );
    merged_view = post_process->get_view( apply_tone_desc[ "dest" ] );
    bloom_view = post_process->get_view( apply_tone_desc[ "bloom" ] );
    std::cout << builder.get_log() << std::endl;
  }
  
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
  const auto camera_desc = sg->get_resource()->matrix->allocate( walk.get_lookat() );

  const auto shadow_camera = sg->get_resource()->matrix->allocate( 6u );
  const auto shadow_projection = sg->get_resource()->matrix->allocate();

  const auto light_desc = sg->get_resource()->light->allocate(
    sg->get_root_node()->matrix,
    gct::punctual_light_parameter()
      .set_type( gct::punctual_light_type::point )
      .set_light_size( light_size )
      .set_local_position( glm::vec4( walk.get_light_pos(), 1.0 ) * glm::vec4( 1.0, -1.0, 1.0, 1.0 ) )
      .set_energy( glm::vec4( walk.get_light_energy(),walk.get_light_energy(),walk.get_light_energy(), 1.0 ) )
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
        tone.set( rec, 0 );
        if( res.force_geometry || walk.camera_moved() || walk.light_moved() ) {
          const auto global_data = global_uniforms_t()
            .set_projection_matrix( *proj_desc )
            .set_camera_matrix( *camera_desc )
            .set_eye_pos( glm::vec4( walk.get_camera_pos(), 1.0 ) )
            .set_light_count( res.light_count )
            .set_ambient( res.ambient_level )
            .set_frame_counter( frame_counter );
          sg->get_resource()->matrix->set( camera_desc, walk.get_lookat() );
          sg->get_resource()->light->set(
            light_desc,
            gct::punctual_light_parameter()
              .set_type( gct::punctual_light_type::point )
              .set_light_size( light_size )
              .set_local_position( glm::vec4( walk.get_light_pos(), 1.0 ) * glm::vec4( 1.0, -1.0, 1.0, 1.0 ) )
              .set_energy( glm::vec4( walk.get_light_energy(),walk.get_light_energy(),walk.get_light_energy(), 1.0 ) )
          );
          (*sg)( rec );
          rec.copy( global_data, global_uniform );
          rec.transfer_to_graphics_barrier( global_uniform );
        
          (*geometry)( rec );
       
          rec.barrier(
            gct::syncable()
              .add( shaded_view )
          );
        }

        (*post_process)( rec );
        
        tone.get( rec, 0 );

        rec.compute_barrier(
          gct::syncable()
            .add( merged_view )
            .add( bloom_view )
        );
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

