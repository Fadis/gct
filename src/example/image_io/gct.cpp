#include "gct/aabb.hpp"
#include "gct/image_io_create_info.hpp"
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
#include <gct/image_io.hpp>
#include <vulkan/vulkan_enums.hpp>

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
  LIBGCT_SETTER( gbuffer )
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
};

int main( int argc, const char *argv[] ) {
  const gct::common_sample_setup res(
    argc, argv,
    {
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
      .rebuild_chain(),
    false
  );

  const auto sg = std::make_shared< gct::scene_graph::scene_graph >(
    gct::scene_graph::scene_graph_create_info()
      .set_allocator_set( res.allocator_set )
      .add_master_shader( CMAKE_CURRENT_BINARY_DIR "/dummy" )
      .set_shader( CMAKE_CURRENT_BINARY_DIR )
      .set_enable_linear( true )
  );
  
  const auto shrink = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( res.allocator_set )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/shrink/shrink.comp.spv" )
      .set_scene_graph( sg->get_resource() )
  );

  const auto src_desc = sg->get_resource()->image->allocate(
    gct::image_load_info()
      .set_filename( "input.png" )
      .set_usage( vk::ImageUsageFlagBits::eStorage )
      .set_mipmap( false )
      .set_enable_linear( false )
      .set_layout( vk::ImageLayout::eGeneral )
  );
 
  const auto src = sg->get_resource()->image->get( src_desc.normalized );

  const auto dest_alloc_info =
    gct::image_allocate_info()
      .set_create_info(
        gct::image_create_info_t()
          .set_basic(
            gct::basic_2d_image(
              src->get_factory()->get_props().get_basic().extent.width / 2,
              src->get_factory()->get_props().get_basic().extent.height / 2
            )
              .setUsage(
                vk::ImageUsageFlagBits::eStorage|
                vk::ImageUsageFlagBits::eTransferSrc|
                vk::ImageUsageFlagBits::eTransferDst
              )
          )
      )
      .set_layout(
        vk::ImageLayout::eGeneral
      );

  const auto dest_desc = sg->get_resource()->image->allocate( dest_alloc_info );

  auto command_buffer = res.queue->get_command_pool()->allocate();
  {
    {
      auto recorder = command_buffer->begin();
      (*sg)( recorder );
    }
    command_buffer->execute_and_wait();
  }

  const auto io = gct::image_io(
    gct::image_io_create_info(
      shrink,
      sg->get_resource(),
      gct::image_io_plan()
        .add_input( "input_image" )
        .add_output(
          "output_image",
          dest_alloc_info
        )
        .set_dim(
          gct::image_io_dimension()
            .set_relative_to( "output_image" )
        )
    )
    .add_input( "input_image", src_desc.normalized )
    .add_output( "output_image", dest_desc.linear )
  );

  {
    {
      auto recorder = command_buffer->begin();
      io( recorder );
    }
    command_buffer->execute_and_wait();
  }

  sg->get_resource()->image->dump(
    dest_desc.linear,
    gct::image_dump_info()
      .set_filename( "output.png" )
  );
  
  {
    {
      auto recorder = command_buffer->begin();
      (*sg)( recorder );
    }
    command_buffer->execute_and_wait();
  }
}

