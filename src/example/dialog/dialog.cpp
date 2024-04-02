#include <iostream>
#include <utility>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <gct/get_extensions.hpp>
#include <gct/instance.hpp>
#include <gct/surface.hpp>
#include <gct/queue.hpp>
#include <gct/device.hpp>
#include <gct/allocator.hpp>
#include <gct/device_create_info.hpp>
#include <gct/buffer.hpp>
#include <gct/image_create_info.hpp>
#include <gct/image.hpp>
#include <gct/swapchain.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/buffer_view_create_info.hpp>
#include <gct/submit_info.hpp>
#include <gct/wait_for_sync.hpp>
#include <gct/present_info.hpp>
#include <gct/gltf.hpp>
#include <gct/vertex_attributes.hpp>
#include <gct/primitive.hpp>
#include <gct/vulkanhpp.hpp>
#include <gct/display_surface_create_info.hpp>
#include <gct/canvas.hpp>
#include <gct/html_color.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_pool.hpp>
#include <gct/framebuffer.hpp>
#include <gct/render_pass.hpp>
#include <gct/eotf.hpp>

struct fb_resources_t {
  std::shared_ptr< gct::image_t > color;
  std::shared_ptr< gct::semaphore_t > image_acquired;
  std::shared_ptr< gct::semaphore_t > draw_complete;
  std::shared_ptr< gct::semaphore_t > image_ownership;
  std::shared_ptr< gct::bound_command_buffer_t > command_buffer;
  bool initial = true;
};

int main() {
  gct::vulkanhpp::init();
  std::vector< const char* > iext{
    "VK_KHR_display",
    "VK_KHR_surface"
  };
  const auto ilayers = gct::get_instance_layers();
  const auto iexts = gct::get_instance_extensions( std::vector< const char* >() );
  std::shared_ptr< gct::instance_t > instance(
    new gct::instance_t(
      gct::instance_create_info_t()
        .set_application_info(
          vk::ApplicationInfo()
            .setPApplicationName( "my_application" )
            .setApplicationVersion(  VK_MAKE_VERSION( 1, 0, 0 ) )
            .setApiVersion( VK_MAKE_VERSION( 1, 2, 0 ) )
        )
        .add_layer(
          "VK_LAYER_KHRONOS_validation"
        )
        .add_extension(
          iext.begin(), iext.end()
        )
    )
  );
  
  auto groups = instance->get_physical_devices( {} );
  auto selected = groups[ 0 ].with_extensions( {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME,
    VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME,
    VK_KHR_MAINTENANCE1_EXTENSION_NAME,
    VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
  } );
  
  auto &displays = selected.devices[ 0 ]->get_props().get_display();
  if( displays.size() == 0u ) {
    std::abort();
  }


  const unsigned int width = 3840u;
  const unsigned int height = 2160u;

  auto display_mode = std::find_if(
    displays[ 0 ].modes.begin(),
    displays[ 0 ].modes.end(),
    []( const auto &v ) {
      return 
        v.basic.parameters.visibleRegion.width == width &&
        v.basic.parameters.visibleRegion.height == height;
    }
  );
  auto surface = selected.devices[ 0 ]->get_surface(
    gct::display_surface_create_info_t()
      .set_basic(
        vk::DisplaySurfaceCreateInfoKHR()
          .setDisplayMode( display_mode->basic.displayMode )
          .setImageExtent(
            vk::Extent2D()
              .setWidth( width )
              .setHeight( height )
          )
      )
  );
 
  std::vector< gct::queue_requirement_t > queue_requirements{
    gct::queue_requirement_t{
      vk::QueueFlagBits::eGraphics,
      0u,
      vk::Extent3D(),
#ifdef VK_EXT_GLOBAL_PRIORITY_EXTENSION_NAME
      vk::QueueGlobalPriorityEXT(),
#endif
      { **surface },
      vk::CommandPoolCreateFlagBits::eResetCommandBuffer
    }
  };
  auto device = selected.create_device(
    queue_requirements,
    gct::device_create_info_t()
  );
  auto queue = device->get_queue( 0u );

  auto swapchain = device->get_swapchain( surface );
  auto swapchain_images = swapchain->get_images();
  std::cout << "swapchain images : " << swapchain_images.size() << std::endl;

  auto descriptor_pool = device->get_descriptor_pool(
    gct::descriptor_pool_create_info_t()
      .set_basic(
        vk::DescriptorPoolCreateInfo()
          .setFlags( vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet )
          .setMaxSets( 10 )
      )
      .set_descriptor_pool_size( vk::DescriptorType::eUniformBuffer, 1 )
      .set_descriptor_pool_size( vk::DescriptorType::eCombinedImageSampler, 16 )
      .rebuild_chain()
  );

  auto pipeline_cache = device->get_pipeline_cache();

  VmaAllocatorCreateInfo allocator_create_info{};
  auto allocator = device->get_allocator(
    allocator_create_info
  );
  
  const auto wvs = device->get_shader_module( "./src/example/dialog/shader.vert.spv" );
  const auto wfs = device->get_shader_module( "./src/example/dialog/shader.frag.spv" );
  const auto dfs = device->get_shader_module( "./src/example/dialog/global.frag.spv" );

  auto [vistat,vamap,stride] = get_vertex_attributes(
    *device,
    wvs->get_props().get_reflection()
  );

  const auto [input_assembly,host_vertex_buffer,vertex_count] = gct::primitive::create_plane( vamap, stride, true );
  std::shared_ptr< gct::buffer_t > vertex_buffer;
  {
    auto command_buffer = queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      vertex_buffer = recorder.load_buffer(
        allocator,
        host_vertex_buffer.data(),
        sizeof( float ) * host_vertex_buffer.size(),
        vk::BufferUsageFlagBits::eVertexBuffer
      );
      recorder.barrier(
        vk::AccessFlagBits::eTransferWrite,
        vk::AccessFlagBits::eVertexAttributeRead,
        vk::PipelineStageFlagBits::eTransfer,
        vk::PipelineStageFlagBits::eVertexInput,
        vk::DependencyFlagBits( 0 ),
        { vertex_buffer },
        {}
      );
    }
    command_buffer->execute(
      gct::submit_info_t()
    );
    command_buffer->wait_for_executed();
  }


  std::shared_ptr< gct::image_t > icon;
  {
    auto command_buffer = queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      icon = recorder.load_image(
        allocator,
        "../images/info.png",
        vk::ImageUsageFlagBits::eSampled,
        true, gct::integer_attribute_t::srgb
      );
      recorder.barrier(
        vk::AccessFlagBits::eTransferWrite,
        vk::AccessFlagBits::eVertexAttributeRead,
        vk::PipelineStageFlagBits::eTransfer,
        vk::PipelineStageFlagBits::eVertexInput,
        vk::DependencyFlagBits( 0 ),
        {},
        { icon }
      );
    }
    command_buffer->execute(
      gct::submit_info_t()
    );
    command_buffer->wait_for_executed();
  }



  gct::canvas window(
    device,
    allocator,
    pipeline_cache,
    descriptor_pool,
    wvs,
    wfs,
    vk::Extent2D{ 640u, 300u },
    std::array< float, 4u >{ 0.f, 0.f, 0.f, 0.f },
    { gct::simple_shape{ vertex_buffer, vertex_count } },
    {},
    { 
      icon->get_view(
        gct::image_view_create_info_t()
          .set_basic(
            vk::ImageViewCreateInfo()
              .setSubresourceRange(
                vk::ImageSubresourceRange()
                  .setAspectMask( vk::ImageAspectFlagBits::eColor )
                  .setBaseMipLevel( 0 )
                  .setLevelCount( icon->get_props().get_basic().mipLevels )
                  .setBaseArrayLayer( 0 )
                  .setLayerCount( icon->get_props().get_basic().arrayLayers )
              )
              .setViewType( gct::to_image_view_type( icon->get_props().get_basic().imageType, icon->get_props().get_basic().arrayLayers ) )
              .setFormat( vk::Format::eR8G8B8A8Srgb )
          )
          .rebuild_chain()
      )
    }
  );
  window.add_rectangle(
    vk::Rect2D{ vk::Offset2D{ 0, 0 }, vk::Extent2D{ 640, 300 } },
    gct::srgb_oetf( gct::html_color( 0xf0f8ff ) ),
    0u, true, 0u, 0u, 0u
  );
  window.add_rectangle(
    vk::Rect2D{ vk::Offset2D{ 95, 50 }, vk::Extent2D{ 200, 70 } },
    gct::srgb_oetf( gct::html_color( 0xc0c0c0 ) ),
    1u, true, 0u, 0u, 1u
  );
  window.add_rectangle(
    vk::Rect2D{ vk::Offset2D{ 345, 50 }, vk::Extent2D{ 200, 70 } },
    gct::srgb_oetf( gct::html_color( 0x4169e1 ) ),
    1u, true, 0u, 0u, 2u
  );
  window.add_rectangle(
    vk::Rect2D{ vk::Offset2D{ 0, 0 }, vk::Extent2D{ 640, 300 } },
    gct::srgb_oetf( gct::html_color( 0x000000 ) ),
    1u, false, 1u, 0u, 0u
  );
  window.add_rectangle(
    vk::Rect2D{ vk::Offset2D{ 50, 150 }, vk::Extent2D{ 128, 128 } },
    gct::srgb_oetf( gct::html_color( 0x000000 ) ),
    1u, true, 0u, 1u, 0u
  );

  std::vector< std::shared_ptr< gct::canvas > > desktop;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    desktop.push_back(
      std::shared_ptr< gct::canvas >( new gct::canvas(
      device,
      allocator,
      pipeline_cache,
      descriptor_pool,
      wvs,
      dfs,
      vk::Extent2D{ width, height },
      gct::srgb_oetf( gct::html_color( 0x008080 ) ),
      { gct::simple_shape{ vertex_buffer, vertex_count } },
      {},
      { 
        window.get_color()->get_view( vk::ImageAspectFlagBits::eColor ),
      }
    ) ) );
    desktop.back()->add_rectangle(
      vk::Rect2D{ vk::Offset2D{ ( width - 640u ) / 2u, ( height - 300u ) / 2u }, vk::Extent2D{ 640, 300 } },
      gct::srgb_oetf( gct::html_color( 0x000000 ) ),
      1u, true, 0u, 1u, 0u
    );
  }

  std::vector< fb_resources_t > framebuffers;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    auto &image = swapchain_images[ i ];
    auto depth = allocator->create_image(
      gct::image_create_info_t()
        .set_basic(
          vk::ImageCreateInfo()
            .setImageType( vk::ImageType::e2D )
            .setFormat( vk::Format::eD16Unorm )
            .setExtent( image->get_props().get_basic().extent )
            .setUsage( vk::ImageUsageFlagBits::eDepthStencilAttachment )
        )
        .rebuild_chain(),
      VMA_MEMORY_USAGE_GPU_ONLY
    );
    auto depth_view = depth->get_view( vk::ImageAspectFlagBits::eDepth );
    auto color_view = image->get_view( vk::ImageAspectFlagBits::eColor );

    framebuffers.emplace_back(
      fb_resources_t{
        image,
        device->get_semaphore(),
        device->get_semaphore(),
        device->get_semaphore(),
        queue->get_command_pool()->allocate()
      }
    );
  }
  
  uint32_t current_frame = 0u;
  while( 1 ) {
    const auto begin_time = std::chrono::high_resolution_clock::now();
    auto &sync = framebuffers[ current_frame ];
    auto image_index = swapchain->acquire_next_image( sync.image_acquired );
    auto &fb = framebuffers[ image_index ];
    bool executed = false;
    if( desktop[ image_index ]->is_modified() ) {
      sync.command_buffer->wait_for_executed();
      {
        auto recorder = sync.command_buffer->begin();
        window( recorder );
        (*desktop[ image_index ])( recorder );
        recorder.blit(
          desktop[ image_index ]->get_color(),
          fb.color
        );
      }
      sync.command_buffer->execute(
        gct::submit_info_t()
          .add_wait_for( sync.image_acquired, vk::PipelineStageFlagBits::eColorAttachmentOutput )
          .add_signal_to( sync.draw_complete )
      );
      executed = true;
    }
    queue->present(
      gct::present_info_t()
        .add_wait_for( executed ? sync.draw_complete : sync.image_acquired )
        .add_swapchain( swapchain, image_index )
    );
    ++current_frame;
    current_frame %= framebuffers.size();
    gct::wait_for_sync( begin_time );
  }
  (*queue)->waitIdle();
}

