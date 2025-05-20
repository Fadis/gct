#include <chrono>
#include <iostream>
#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <gct/get_extensions.hpp>
#include <gct/instance.hpp>
#include <gct/queue.hpp>
#include <gct/device.hpp>
#include <gct/allocator.hpp>
#include <gct/device_create_info.hpp>
#include <gct/image_create_info.hpp>
#include <gct/image.hpp>
#include <gct/swapchain.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/pipeline_layout_create_info.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/graphics.hpp>
#include <gct/image_filter.hpp>
#include <gct/glfw.hpp>
#include <gct/buffer_view_create_info.hpp>
#include <gct/render_pass_begin_info.hpp>
#include <gct/submit_info.hpp>
#include <gct/shader_module.hpp>
#include <gct/fence.hpp>
#include <gct/wait_for_sync.hpp>
#include <gct/present_info.hpp>
#include <gct/timer.hpp>
#include <gct/gbuffer.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_pool.hpp>
#include <gct/framebuffer.hpp>
#include <gct/render_pass.hpp>
#include <gct/named_resource.hpp>
#include <gct/common_sample_setup.hpp>

struct fb_resources_t {
  std::shared_ptr< gct::semaphore_t > image_acquired;
  std::shared_ptr< gct::semaphore_t > draw_complete;
  std::shared_ptr< gct::semaphore_t > render_complete;
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
          .setMaxSets( 65536 * 7 )
      )
      .set_descriptor_pool_size( vk::DescriptorType::eUniformBuffer, 16 )
      .set_descriptor_pool_size( vk::DescriptorType::eStorageBuffer, 65536*2 )
      .set_descriptor_pool_size( vk::DescriptorType::eCombinedImageSampler, 65536*2 )
      .set_descriptor_pool_size( vk::DescriptorType::eStorageImage, 65536*2 )
      .rebuild_chain(),
      true,
      false,
      false
  );

  gct::gbuffer output(
    gct::gbuffer_create_info()
      .set_allocator( res.allocator )
      .set_width( res.width )
      .set_height( res.height )
      .set_layer( 1u )
      .set_swapchain_image_count( 1u )
      .set_color_buffer_count( 1u )
      .set_format( vk::Format::eR16G16B16A16Sfloat ) 
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
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
  
  gct::graphics geometry(
    gct::graphics_create_info()
      .set_allocator_set( res.allocator_set )
      .set_pipeline_create_info(
        gct::graphics_pipeline_create_info_t()
          .set_gbuffer( output )
      )
      .set_swapchain_image_count( 1u )
      .add_shader( CMAKE_CURRENT_BINARY_DIR "/geometry/geometry.task.spv" )
      .add_shader( CMAKE_CURRENT_BINARY_DIR "/geometry/geometry.mesh.spv" )
      .add_shader( CMAKE_CURRENT_BINARY_DIR "/geometry/geometry.frag.spv" )
  );
  
  auto command_buffer = res.queue->get_command_pool()->allocate();
  
  const auto gamma = gct::image_filter(
    gct::image_filter_create_info()
      .set_allocator_set( res.allocator_set )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/gamma/gamma.comp.spv" )
      .set_input( std::vector< std::shared_ptr< gct::image_view_t > >( res.swapchain_images.size(), output.get_image_view( 0 ) ) )
      .set_output( res.swapchain_image_views )
  );

  std::uint32_t frame_counter = 0u;
  bool update_optflow = false;
  float average = 0.f;
  while( 1 ) {
    const auto begin_date = std::chrono::high_resolution_clock::now();
    auto &sync = framebuffers[ frame_counter % framebuffers.size() ];
    gct::blocking_timer frame_rate;
    {
      {
        auto rec = command_buffer->begin();
        auto render_pass_token = rec.begin_render_pass(
          output.get_render_pass_begin_info( 0 ),
          vk::SubpassContents::eInline
        );
        rec->setViewport( 0, 1, &output.get_viewport() );
        rec->setScissor( 0, 1, &output.get_scissor() );
        rec->setCullMode( vk::CullModeFlagBits::eBack );
        rec->setDepthCompareOp( vk::CompareOp::eLessOrEqual );
        geometry( rec, 0, 1, 1, 1 );
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
    ++frame_counter;
  }
  (*res.queue)->waitIdle();
}

