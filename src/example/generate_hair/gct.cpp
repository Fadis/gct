#include <iostream>
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
      .rebuild_chain(),
    true,
    false,
    false
  );

  const auto sg = std::make_shared< gct::scene_graph::scene_graph >(
    gct::scene_graph::scene_graph_create_info()
      .set_allocator_set( res.allocator_set )
      .add_master_shader( gct::get_system_shader_path() / "scene_graph" / "dummy" )
      .set_enable_linear( true )
  );

  const auto ppll_state_desc = sg->get_resource()->ppll_state->allocate();

  const auto hair_gbuffer_format =
    gct::gbuffer_format::albedo_alpha;

  const unsigned int hair_tex_size = 512u;
  std::shared_ptr< gct::shader_graph::compiled > generate_hair;
  std::shared_ptr< gct::image_view_t > hair_view;
  {
    gct::shader_graph::builder builder( sg->get_resource() );
        
    
    const auto generate_hair_desc = builder.call(
      builder.get_image_io_create_info(
        std::make_shared< gct::graphics >(
          gct::graphics_create_info()
            .set_swapchain_image_count( 1u )
            .add_shader( gct::get_system_shader_path() / "generate_hair" / "straight" / "1.0" )
            .use_dynamic_rendering(
              vk::Format::eR16G16B16A16Sfloat
            )
            .set_scene_graph( sg->get_resource() )
            .set_line_width( 1.0f )
        ),
        gct::image_io_plan()
          .add_output( "gbuffer", hair_tex_size, hair_tex_size * 64u, vk::Format::eR16G16B16A16Sfloat )
          .add_output( "position", hair_tex_size, hair_tex_size * 64u, vk::Format::eR32G32B32A32Sfloat )
          .add_output( "start", hair_tex_size, hair_tex_size, vk::Format::eR32Uint )
          .add_output( "next", hair_tex_size, hair_tex_size * 64u, vk::Format::eR32Uint )
          .set_dim( 25u, 1u, 128u )
          .set_node_name( "generate_hair" )
      )
      .set_render_area(
        vk::Rect2D()
          .setExtent(
            vk::Extent2D()
              .setWidth( hair_tex_size )
              .setHeight( hair_tex_size )
          )
      )
      .set_push_constant( "straight_factor", 0.7f )
      .set_push_constant( "ppll_state_id", *ppll_state_desc )
      .set_push_constant( "gbuffer_format", hair_gbuffer_format )
    )();
    
    const auto mix_hair_desc = builder.call(
      builder.get_image_io_create_info(
        std::make_shared< gct::compute >(
          gct::compute_create_info()
            .set_allocator_set( res.allocator_set )
            .set_shader( gct::get_system_shader_path() / "generate_hair" / "straight" / "1.0" / "mix.comp.spv" )
            .set_scene_graph( sg->get_resource() )
        ),
        gct::image_io_plan()
          .add_input( "gbuffer" )
          .add_input( "position" )
          .add_input( "start" )
          .add_input( "next" )
          .add_output( "dest", "start", { 1.f, -1.f }, vk::Format::eR16G16B16A16Sfloat )
          .set_dim( "start", { 1.f, -1.f } )
          .set_node_name( "mix_hair" )
      )
      .set_push_constant( "ppll_state_id", *ppll_state_desc )
      .set_push_constant( "gbuffer_format", hair_gbuffer_format )
    )(
      gct::shader_graph::vertex::combined_result_type()
        .add( "gbuffer",  generate_hair_desc[ "gbuffer" ] )
        .add( "position",  generate_hair_desc[ "position" ] )
        .add( "start",  generate_hair_desc[ "start" ] )
        .add( "next",  generate_hair_desc[ "next" ] )
    );

    builder.output( mix_hair_desc[ "dest" ] );
    generate_hair = std::make_shared< gct::shader_graph::compiled >( builder() );
    hair_view = generate_hair->get_view( mix_hair_desc[ "dest" ] );
    std::cout << builder.get_log() << std::endl;
  }

  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto rec = command_buffer->begin();
      (*sg)( rec );
      (*generate_hair)( rec );
      rec.compute_barrier(
        gct::syncable()
          .add( hair_view )
      );
      rec.dump_image(
        res.allocator,
        hair_view->get_factory(),
        "hair.png",
        0u
      );

    }
    command_buffer->execute_and_wait();
  }
  
}

