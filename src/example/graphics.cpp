#include <iostream>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/string_cast.hpp>
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
#include <gct/sampler_create_info.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/pipeline_layout_create_info.hpp>
#include <gct/buffer_view_create_info.hpp>
#include <gct/render_pass_begin_info.hpp>
#include <gct/submit_info.hpp>
#include <gct/fence.hpp>
#include <gct/wait_for_sync.hpp>
#include <gct/present_info.hpp>
#include <gct/gltf.hpp>
#include <gct/color.hpp>
#include <gct/timer.hpp>

struct fb_resources_t {
  std::shared_ptr< gct::image_t > color;
  std::shared_ptr< gct::framebuffer_t > framebuffer;
  std::shared_ptr< gct::semaphore_t > image_acquired;
  std::shared_ptr< gct::semaphore_t > draw_complete;
  std::shared_ptr< gct::semaphore_t > image_ownership;
  std::shared_ptr< gct::bound_command_buffer_t > command_buffer;
  gct::render_pass_begin_info_t render_pass_begin_info;
  bool initial = true;
};

int main() {
  gct::glfw::get();
  uint32_t iext_count = 0u;
  auto exts = glfwGetRequiredInstanceExtensions( &iext_count );
  std::vector< const char* > iext{};
  for( uint32_t i = 0u; i != iext_count; ++i )
    iext.push_back( exts[ i ] );
  const auto ilayers = gct::get_instance_layers();
  const auto iexts = gct::get_instance_extensions( std::vector< const char* >() );
  std::shared_ptr< gct::instance_t > instance(
    new gct::instance_t(
      gct::instance_create_info_t()
        .set_application_info(
          vk::ApplicationInfo()
            .setPApplicationName( "my_application" )
            .setApplicationVersion(  VK_MAKE_VERSION( 1, 0, 0 ) )
            .setApiVersion( VK_MAKE_VERSION( 1, 3, 0 ) )
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
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME,
    VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME,
    VK_KHR_MAINTENANCE1_EXTENSION_NAME,
    VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
  } );
  std::cout << nlohmann::json( groups[ 0 ] ).dump( 2 ) << std::endl;

  std::uint32_t width = 1920u*2;
  std::uint32_t height = 1080u*2;

  gct::glfw_window window( width, height, "window title", false );
  gct::glfw::get().poll();
  auto surface = window.get_surface( *groups[ 0 ].devices[ 0 ] );
  std::cout << nlohmann::json( *surface ).dump( 2 ) << std::endl;
 
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
  auto gcb = queue->get_command_pool()->allocate();

  auto swapchain = device->get_swapchain( surface );
  auto swapchain_images = swapchain->get_images();
  std::cout << "swapchain images : " << swapchain_images.size() << std::endl;

  // 11 * n
  auto descriptor_pool = device->get_descriptor_pool(
    gct::descriptor_pool_create_info_t()
      .set_basic(
        vk::DescriptorPoolCreateInfo()
          .setFlags( vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet )
          .setMaxSets( 250 )
      )
      .set_descriptor_pool_size( vk::DescriptorType::eUniformBuffer, 2 )
      .set_descriptor_pool_size( vk::DescriptorType::eCombinedImageSampler, 9 )
      .rebuild_chain()
  );

  auto pipeline_cache = device->get_pipeline_cache();

  auto render_pass = device->get_render_pass(
    gct::select_simple_surface_format( surface->get_caps().get_formats() ).basic.format,
    vk::Format::eD16Unorm
  );
  VmaAllocatorCreateInfo allocator_create_info{};
  //allocator_create_info.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
  auto allocator = device->get_allocator(
    allocator_create_info
  );
  
  
  std::vector< fb_resources_t > framebuffers;

  const auto dynamic_descriptor_set_layout = device->get_descriptor_set_layout(
    gct::descriptor_set_layout_create_info_t()
      .add_binding(
        vk::DescriptorSetLayoutBinding()
          .setBinding( 0 )
          .setDescriptorType( vk::DescriptorType::eUniformBuffer )
          .setDescriptorCount( 1u )
          .setStageFlags( vk::ShaderStageFlagBits::eVertex|vk::ShaderStageFlagBits::eFragment )
      )
  );
  std::vector< std::shared_ptr< gct::buffer_t > > staging_dynamic_uniform;
  std::vector< std::shared_ptr< gct::buffer_t > > dynamic_uniform;
  std::vector< std::shared_ptr< gct::descriptor_set_t > > dynamic_descriptor_set;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    staging_dynamic_uniform.emplace_back(
      allocator->create_buffer(
        sizeof( gct::gltf::dynamic_uniforms_t ),
        vk::BufferUsageFlagBits::eTransferSrc,
        VMA_MEMORY_USAGE_CPU_TO_GPU
      )
    );
    dynamic_uniform.emplace_back(
      allocator->create_buffer(
        sizeof( gct::gltf::dynamic_uniforms_t ),
        vk::BufferUsageFlagBits::eTransferDst|vk::BufferUsageFlagBits::eUniformBuffer,
        VMA_MEMORY_USAGE_GPU_ONLY
      )
    );
    dynamic_descriptor_set.push_back(
      descriptor_pool->allocate(
        dynamic_descriptor_set_layout
      )
    );
    std::vector< gct::write_descriptor_set_t > updates;
    updates.push_back(
      gct::write_descriptor_set_t()
        .set_basic(
          vk::WriteDescriptorSet()
            .setDstSet( **dynamic_descriptor_set.back() )
            .setDstBinding( 0u )
            .setDescriptorCount( 1u )
            .setDescriptorType( vk::DescriptorType::eUniformBuffer )
        )
        .add_buffer(
          gct::descriptor_buffer_info_t()
            .set_buffer( dynamic_uniform.back() )
            .set_basic(
              vk::DescriptorBufferInfo()
                .setOffset( 0 )
                .setRange( sizeof( gct::gltf::dynamic_uniforms_t ) )
            )
        )
    );
    dynamic_descriptor_set.back()->update( updates );
  }

  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    auto &image = swapchain_images[ i ];
    auto &render_pass_ = render_pass;
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
    auto framebuffer = render_pass_->get_framebuffer(
      gct::framebuffer_create_info_t()
        .add_attachment( color_view )
        .add_attachment( depth_view )
    );
    framebuffers.emplace_back(
      fb_resources_t{
        image,
        framebuffer,
        device->get_semaphore(),
        device->get_semaphore(),
        device->get_semaphore(),
        queue->get_command_pool()->allocate(),
        gct::render_pass_begin_info_t()
          .set_basic(
            vk::RenderPassBeginInfo()
              .setRenderPass( **render_pass )
              .setFramebuffer( **framebuffer )
              .setRenderArea( vk::Rect2D( vk::Offset2D(0, 0), vk::Extent2D((uint32_t)width, (uint32_t)height) ) )
          )
          .add_clear_value( vk::ClearColorValue( gct::color::web::wheat ) )
          .add_clear_value( vk::ClearDepthStencilValue( 1.f, 0 ) )
          .rebuild_chain()
      }
    );
  }

  auto environment_sampler = device->get_sampler(
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
          .setMaxLod( 9.f )
          .setBorderColor( vk::BorderColor::eFloatTransparentBlack )
          .setUnnormalizedCoordinates( false )
      )
  );

  std::shared_ptr< gct::image_t > environment_image;
  {
    auto command_buffer = queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      environment_image = recorder.load_image(
        allocator,
        "../images/environment.png",
        vk::ImageUsageFlagBits::eSampled,
        true, true
      );
      recorder.barrier(
        vk::AccessFlagBits::eTransferWrite,
        vk::AccessFlagBits::eVertexAttributeRead,
        vk::PipelineStageFlagBits::eTransfer,
        vk::PipelineStageFlagBits::eVertexInput,
        vk::DependencyFlagBits( 0 ),
        {},
        { environment_image }
      );
    }
    command_buffer->execute(
      gct::submit_info_t()
    );
    command_buffer->wait_for_executed();
  }
  
  auto environment_image_view = environment_image->get_view(
    gct::image_view_create_info_t()
      .set_basic(
        vk::ImageViewCreateInfo()
          .setSubresourceRange(
            vk::ImageSubresourceRange()
              .setAspectMask( vk::ImageAspectFlagBits::eColor )
          )
          .setViewType( gct::to_image_view_type( environment_image->get_props().get_basic().imageType ) )
          .setFormat( vk::Format::eR8G8B8A8Unorm )
      )
      .rebuild_chain()
  );

  const auto env_descriptor_set_layout = device->get_descriptor_set_layout(
    gct::descriptor_set_layout_create_info_t()
      .add_binding(
        vk::DescriptorSetLayoutBinding()
          .setBinding( 0 )
          .setDescriptorType( vk::DescriptorType::eCombinedImageSampler )
          .setDescriptorCount( 1u )
          .setStageFlags( vk::ShaderStageFlagBits::eFragment )
      )
  );

  auto env_descriptor_set = descriptor_pool->allocate( env_descriptor_set_layout );
  std::vector< gct::write_descriptor_set_t > updates;
  updates.push_back(
    gct::write_descriptor_set_t()
      .set_basic(
        vk::WriteDescriptorSet()
          .setDstSet( **env_descriptor_set )
          .setDstBinding( 0u )
          .setDescriptorCount( 1u )
          .setDescriptorType( vk::DescriptorType::eCombinedImageSampler )
      )
      .add_image(
        gct::descriptor_image_info_t()
          .set_sampler( environment_sampler )
          .set_image_view( environment_image_view )
          .set_basic(
            vk::DescriptorImageInfo()
              .setImageLayout(
                environment_image->get_layout().get_uniform_layout()
              )
          )
      )
  );
  env_descriptor_set->update( updates );


  gct::gltf::document_t doc;
  {
    auto rec = gcb->begin();
    //rec.load_image( allocator, "/home/fadis/gltf/BoomBox/glTF/BoomBox_baseColor.png", vk::ImageUsageFlagBits::eSampled, true, false );
    doc = gct::gltf::load_gltf(
      //"/home/fadis/pi_simple.gltf",
      //"/home/fadis/box.gltf",
      "/home/fadis/gltf/BoomBox/glTF/BoomBox.gltf",
      device,
      rec,
      allocator,
      descriptor_pool,
      { render_pass },
      {
        "/home/fadis/git/gct/shaders"
      },
      0,
      framebuffers.size(),
      0,
      float( width ) / float( height ),
      false,
      {
        dynamic_descriptor_set_layout,
        env_descriptor_set_layout
      }
    );
  }
  gcb->execute(
    gct::submit_info_t()
  );
  gcb->wait_for_executed();

  auto center = ( doc.node.min + doc.node.max ) / 2.f;
  auto scale = std::abs( glm::length( doc.node.max - doc.node.min ) );
  auto const viewport =
    vk::Viewport()
      .setWidth( width )
      .setHeight( height )
      .setMinDepth( 0.0f )
      .setMaxDepth( 1.0f );
  vk::Rect2D const scissor( vk::Offset2D(0, 0), vk::Extent2D( width, height ) );


  const glm::mat4 projection = glm::perspective( 0.39959648408210363f, (float(width)/float(height)), std::min(0.1f*scale,0.5f), 150.f*scale );
  gct::glfw_walk walk( center, scale );
  const auto point_lights = gct::gltf::get_point_lights(
    doc.node,
    doc.point_light
  );
  if( !point_lights.empty() ) {
    walk.set_light_energy( point_lights[ 0 ].intensity / ( 4 * M_PI ) / 100 );
    walk.set_light_pos( point_lights[ 0 ].location );
  }
  window.set_on_key(
    [&walk]( gct::glfw_window &p, int key, int scancode, int action, int mods ) {
      walk( p, key, scancode, action, mods );
    }
  );
  window.set_on_closed(
    [&walk]( auto & ) {
      walk.set_end();
    }
  );

  uint32_t current_frame = 0u;
  uint32_t last_image_index = framebuffers.size();
  while( !walk.end() ) {
    gct::blocking_timer frame_rate;
    ++walk;
    auto &sync = framebuffers[ current_frame ];
    if( !sync.initial ) {
      sync.command_buffer->wait_for_executed();
    }
    else sync.initial = false;
    auto image_index = swapchain->acquire_next_image( sync.image_acquired );
    auto &fb = framebuffers[ image_index ];
    {
      auto rec = sync.command_buffer->begin();
      //rec.convert_image( fb.color, fb.color->get_props().get_basic().initialLayout, vk::ImageLayout::ePresentSrcKHR );
      auto dynamic_data = gct::gltf::dynamic_uniforms_t()
        .set_projection_matrix( projection )
        .set_camera_matrix( walk.get_lookat() )
        .set_eye_pos( glm::vec4( walk.get_camera_pos(), 1.0 ) )
        .set_light_pos( glm::vec4( walk.get_light_pos(), 1.0 ) )
        .set_light_energy( walk.get_light_energy() );
      rec.copy(
        dynamic_data,
        staging_dynamic_uniform[ image_index ],
        dynamic_uniform[ image_index ]
      );
      rec.barrier(
        vk::AccessFlagBits::eTransferRead,
        vk::AccessFlagBits::eShaderRead,
        vk::PipelineStageFlagBits::eTransfer,
        vk::PipelineStageFlagBits::eVertexShader,
        vk::DependencyFlagBits( 0 ),
        { dynamic_uniform[ image_index ] },
        {}
      );

      auto render_pass_token = rec.begin_render_pass(
        fb.render_pass_begin_info,
        vk::SubpassContents::eInline
      );
      rec->setViewport( 0, 1, &viewport );
      rec->setScissor( 0, 1, &scissor );
      gct::gltf::draw_node(
        rec,
        doc.node,
        doc.mesh,
        doc.buffer,
        0u,
        {
          dynamic_descriptor_set[ image_index ],
          env_descriptor_set
        }
      );
    }
    sync.command_buffer->execute(
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
    ++current_frame;
    current_frame %= framebuffers.size();
  }
  (*queue)->waitIdle();
}

