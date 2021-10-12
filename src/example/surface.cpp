#include <iostream>
#include <nlohmann/json.hpp>
#include <gct/get_extensions.hpp>
#include <gct/instance.hpp>
#include <gct/glfw.hpp>
#include <gct/queue.hpp>
#include <gct/device.hpp>
#include <gct/allocator.hpp>
#include <gct/device_create_info.hpp>
#include <gct/image_create_info.hpp>
#include <gct/swapchain.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/pipeline_layout_create_info.hpp>
#include <gct/buffer_view_create_info.hpp>
#include <gct/submit_info.hpp>
#include <gct/shader_module_create_info.hpp>
#include <gct/shader_module.hpp>
#include <gct/compute_pipeline_create_info.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/fence.hpp>
#include <gct/write_descriptor_set.hpp>
#include <gct/present_info.hpp>
#include <gct/wait_for_sync.hpp>
#include <gct/gltf.hpp>

struct fb_resources_t {
  std::shared_ptr< gct::image_t > color;
  std::shared_ptr< gct::descriptor_set_t > descriptor_set;
  std::shared_ptr< gct::semaphore_t > image_acquired;
  std::shared_ptr< gct::semaphore_t > draw_complete;
  std::shared_ptr< gct::semaphore_t > image_ownership;
  std::shared_ptr< gct::fence_t > fence;
  std::shared_ptr< gct::bound_command_buffer_t > command_buffer;
};

struct spec_t {
  std::uint32_t local_x_size = 0u;
  std::uint32_t local_y_size = 0u;
  float value = 0.f;
};

int main() {
  gct::glfw::get();
  uint32_t iext_count = 0u;
  auto exts = glfwGetRequiredInstanceExtensions( &iext_count );
  std::vector< const char* > iext{};
  for( uint32_t i = 0u; i != iext_count; ++i )
    iext.push_back( exts[ i ] );
  std::shared_ptr< gct::instance_t > instance(
    new gct::instance_t(
      "test_gct",
      VK_MAKE_VERSION( 1, 0, 0 ),
      VK_MAKE_VERSION( 1, 2, 0 ),
      iext,
      std::vector< const char* >{
        "VK_LAYER_KHRONOS_validation"
      }
    )
  );
  auto groups = instance->get_physical_devices( {} );
  auto selected = groups[ 0 ].with_extensions( {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
  } );
  
  gct::glfw_window window( 640, 480, "window title", false );
  bool closed = false;
  window.set_on_closed( [&]( auto & ) { std::cout << "closed" << std::endl; closed = true; } );
  gct::glfw::get().poll();
  auto surface = window.get_surface( *selected.devices[ 0 ] );
 
  auto device = selected.create_device(
    std::vector< gct::queue_requirement_t >{
      gct::queue_requirement_t{
        vk::QueueFlagBits::eCompute,
        0u,
        vk::Extent3D(),
#ifdef VK_EXT_GLOBAL_PRIORITY_EXTENSION_NAME
        vk::QueueGlobalPriorityEXT(),
#endif
        { **surface },
        vk::CommandPoolCreateFlagBits::eResetCommandBuffer
      }
    },
    gct::device_create_info_t()
  );
  auto queue = device->get_queue( 0u );
  
  auto swapchain = device->get_swapchain( surface );
  auto swapchain_images = swapchain->get_images();

  auto command_buffer = queue->get_command_pool()->allocate();

  auto descriptor_pool = device->get_descriptor_pool(
    gct::descriptor_pool_create_info_t()
      .set_basic(
        vk::DescriptorPoolCreateInfo()
          .setFlags( vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet )
          .setMaxSets( 10 )
      )
      .set_descriptor_pool_size( vk::DescriptorType::eStorageBuffer, 5 )
      .rebuild_chain()
  );
  auto descriptor_set_layout = device->get_descriptor_set_layout(
    gct::descriptor_set_layout_create_info_t()
      .add_binding(
        vk::DescriptorSetLayoutBinding()
          .setDescriptorType( vk::DescriptorType::eStorageBuffer )
          .setDescriptorCount( 1 )
          .setBinding( 0 )
          .setStageFlags( vk::ShaderStageFlagBits::eCompute )
          .setPImmutableSamplers( nullptr )
      )
      .rebuild_chain()
  );

  auto descriptor_set = descriptor_pool->allocate( *descriptor_set_layout );

  auto shader = device->get_shader_module(
    gct::shader_module_create_info_t()
      .load( "../shaders/grad.comp.spv" )
  );
  auto pipeline_layout = device->get_pipeline_layout(
    gct::pipeline_layout_create_info_t()
      .add_descriptor_set_layout( descriptor_set_layout )
      .add_push_constant_range(
        vk::PushConstantRange()
          .setStageFlags( vk::ShaderStageFlagBits::eCompute )
          .setOffset( 0 )
          .setSize( 32u )
      )
  );

  auto pipeline_cache = device->get_pipeline_cache();

  auto render_pass = device->get_render_pass(
    gct::render_pass_create_info_t()
      .add_attachment(
        vk::AttachmentDescription()
          .setFormat( surface->get_caps().get_formats()[ 0 ].basic.format )
          .setSamples( vk::SampleCountFlagBits::e1 )
          .setLoadOp( vk::AttachmentLoadOp::eClear )
          .setStoreOp( vk::AttachmentStoreOp::eStore )
          .setStencilLoadOp( vk::AttachmentLoadOp::eDontCare )
          .setStencilStoreOp( vk::AttachmentStoreOp::eDontCare )
          .setInitialLayout( vk::ImageLayout::eUndefined )
          .setFinalLayout( vk::ImageLayout::ePresentSrcKHR )
      )
      .add_attachment(
        vk::AttachmentDescription()
          .setFormat( vk::Format::eD16Unorm )
          .setSamples( vk::SampleCountFlagBits::e1 )
          .setLoadOp( vk::AttachmentLoadOp::eClear )
          .setStoreOp( vk::AttachmentStoreOp::eDontCare )
          .setStencilLoadOp( vk::AttachmentLoadOp::eDontCare )
          .setStencilStoreOp( vk::AttachmentStoreOp::eDontCare )
          .setInitialLayout( vk::ImageLayout::eUndefined )
          .setFinalLayout( vk::ImageLayout::eDepthStencilAttachmentOptimal )
      )
      .add_subpass(
        gct::subpass_description_t()
          .add_color_attachment( 0, vk::ImageLayout::eColorAttachmentOptimal )
          .set_depth_stencil_attachment( 1, vk::ImageLayout::eDepthStencilAttachmentOptimal )
          .rebuild_chain()
      )
      .rebuild_chain()
  );

  auto pipeline = pipeline_cache->get_compute_pipeline(
    gct::compute_pipeline_create_info_t()
      .set_stage(
        gct::pipeline_shader_stage_create_info_t()
          .set_shader_module( shader )
          .set_basic(
            vk::PipelineShaderStageCreateInfo()
              .setStage( vk::ShaderStageFlagBits::eCompute )
              .setPName( "main" )
          )
          .set_specialization_info(
            gct::specialization_info_t< spec_t >()
              .set_data(
                spec_t{ 8, 4, 0.5f }
              )
              .add_map< std::uint32_t >( 1, offsetof( spec_t, local_x_size ) )
              .add_map< std::uint32_t >( 2, offsetof( spec_t, local_y_size ) )
              .add_map< std::uint32_t >( 3, offsetof( spec_t, value ) )
          )
      )
      .set_layout( pipeline_layout )
  );

  auto allocator = device->get_allocator();
 
  std::vector< fb_resources_t > framebuffers;

  for( auto &i: swapchain_images ) {
    framebuffers.emplace_back(
      fb_resources_t{
        std::move( i ),
        descriptor_pool->allocate( *descriptor_set_layout ),
        device->get_semaphore(),
        device->get_semaphore(),
        device->get_semaphore(),
        device->get_fence(),
        queue->get_command_pool()->allocate()
      }
    );
  }


  auto device_local_buffer = allocator->create_buffer(
    gct::buffer_create_info_t()
      .set_basic(
        vk::BufferCreateInfo()
          .setSize( 640 * 480 * sizeof( glm::vec4 ) )
          .setUsage( vk::BufferUsageFlagBits::eStorageBuffer|vk::BufferUsageFlagBits::eTransferSrc|vk::BufferUsageFlagBits::eTransferDst )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  auto device_local_image = allocator->create_image(
    gct::image_create_info_t()
      .set_basic(
        vk::ImageCreateInfo()
          .setImageType( vk::ImageType::e2D )
          .setFormat( vk::Format::eR32G32B32A32Sfloat )
          .setExtent(
            vk::Extent3D()
              .setWidth( 640 )
              .setHeight( 480 )
          )
          .setUsage( vk::ImageUsageFlagBits::eTransferSrc|vk::ImageUsageFlagBits::eTransferDst )
          .setTiling( vk::ImageTiling::eLinear )
      )
      .rebuild_chain(),
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  
  descriptor_set->update(
    {
      gct::write_descriptor_set_t()
        .set_basic(
          vk::WriteDescriptorSet()
            .setDstBinding( 0 )
            .setDescriptorType( vk::DescriptorType::eStorageBuffer )
            .setDescriptorCount( 1 )
        )
        .set_buffer(
          gct::descriptor_buffer_info_t()
            .set_buffer( device_local_buffer )
            .set_basic(
              vk::DescriptorBufferInfo()
                .setOffset( 0 )
                .setRange( 1024 )
            )
        )
    }
  );

  {
    auto rec = command_buffer->begin();
    rec.bind_descriptor_set(
      vk::PipelineBindPoint::eCompute,
      pipeline_layout,
      descriptor_set
    );
    rec.bind_pipeline(
      vk::PipelineBindPoint::eCompute,
      pipeline
    );
    rec->dispatch( 640 / 8, 480 / 4, 1 );
    rec.copy( device_local_buffer, device_local_image, vk::ImageLayout::eTransferSrcOptimal );
  }
  command_buffer->execute(
    gct::submit_info_t()
  );
  command_buffer->wait_for_executed();

  unsigned int current_frame = 0u;
  unsigned int last_image_index = framebuffers.size();
  while( !closed ) {
    const auto begin_time = std::chrono::high_resolution_clock::now();
    auto &sync = framebuffers[ current_frame ];
    if( last_image_index != framebuffers.size() )
      framebuffers[ last_image_index ].command_buffer->wait_for_executed();
    //auto wait_for_fences_result = (*device)->waitForFences( 1, &**sync.fence, VK_TRUE, UINT64_MAX );
    //if( wait_for_fences_result != vk::Result::eSuccess )
    //  vk::throwResultException( wait_for_fences_result, "waitForFences failed" );
    //auto reset_fences_result = (*device)->resetFences( 1, &**sync.fence );
    auto image_index = swapchain->acquire_next_image( sync.image_acquired );
    auto &fb = framebuffers[ image_index ];
    {
      auto rec = fb.command_buffer->begin();
      rec.convert_image( fb.color, fb.color->get_props().get_basic().initialLayout, vk::ImageLayout::eSharedPresentSrcKHR );
      rec->blitImage(
        **device_local_image, vk::ImageLayout::eTransferSrcOptimal,
        **fb.color, vk::ImageLayout::eSharedPresentSrcKHR,
        {
          vk::ImageBlit()
            .setSrcSubresource(
              vk::ImageSubresourceLayers()
                .setAspectMask( vk::ImageAspectFlagBits::eColor )
                .setBaseArrayLayer( 0 )
                .setLayerCount( 1 )
                .setMipLevel( 0 )
            )
            .setSrcOffsets( {
              vk::Offset3D( 0, 0, 0 ),
              vk::Offset3D( 640, 480, 1 ),
            } )
            .setDstSubresource(
              vk::ImageSubresourceLayers()
                .setAspectMask( vk::ImageAspectFlagBits::eColor )
                .setBaseArrayLayer( 0 )
                .setLayerCount( 1 )
                .setMipLevel( 0 )
            )
            .setDstOffsets( {
              vk::Offset3D( 0, 0, 0 ),
              vk::Offset3D( 640, 480, 1 ),
            } )
        },
        vk::Filter::eLinear
      );
      //rec.convert_image( fb.color, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::ePresentSrcKHR );
    }
    fb.command_buffer->execute(
      gct::submit_info_t()
        .add_wait_for( sync.image_acquired, vk::PipelineStageFlagBits::eColorAttachmentOutput )
        .add_signal_to( sync.draw_complete )
    );
    queue->present(
      gct::present_info_t()
        .add_wait_for( sync.draw_complete )
        .add_swapchain( swapchain, image_index )
    );
    last_image_index = image_index;
    glfwPollEvents();
    gct::wait_for_sync( begin_time );
    std::cout << current_frame << std::endl;
    ++current_frame;
    if( current_frame == framebuffers.size() ) current_frame = 0u;
  }

}

