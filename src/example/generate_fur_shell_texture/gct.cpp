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
      .add_master_shader( gct::get_system_shader_path() / "occlusion_culling" / "roc" / "1.0" )
      .set_enable_linear( true )
  );

  const auto linear_sampler_desc = sg->get_resource()->sampler->allocate_linear();
  auto linear_sampler = sg->get_resource()->sampler->get( linear_sampler_desc );

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

  const auto fur_clamp_desc = sg->get_resource()->image->allocate(
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
  const auto fur_clamp = sg->get_resource()->image->get( fur_clamp_desc.linear );
  const auto fur_clamp_texture_desc = sg->get_resource()->texture->allocate(
    linear_repeat_sampler_desc, fur_clamp_desc.linear
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
        fur_clamp->get_factory(), vk::ImageLayout::eGeneral
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

  auto clamp = gct::compute(
    gct::compute_create_info()
      .set_allocator_set( res.allocator_set )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/clamp/generate.comp.spv" )
      .set_scene_graph( sg->get_resource() )
  );
  clamp.set_push_constant( "src", *fur_shell_desc.linear );
  clamp.set_push_constant( "dest", *fur_clamp_desc.linear );
  clamp.set_push_constant( "shell_texture_clamp_max", 0.90f );
  clamp.set_push_constant( "shell_texture_clamp_min", 0.82f );

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
      clamp( rec, 0u, 128u, 128u, 1u );
      rec.barrier(
        gct::syncable()
          .add( random )
          .add( fur_shell )
          .add( fur_clamp )
      );
      rec.dump_image(
        res.allocator,
        random->get_factory(),
        "random.png",
        0u
      );
      rec.dump_image(
        res.allocator,
        fur_shell->get_factory(),
        "shell.png",
        0u
      );
      rec.dump_image(
        res.allocator,
        fur_clamp->get_factory(),
        "clamp.png",
        0u,
        0u
      );
      (*sg)( rec );
    }
    command_buffer->execute_and_wait();
  }

  (*res.queue)->waitIdle();
}

