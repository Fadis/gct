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
#include <gct/pipeline_cache.hpp>
#include <gct/pipeline_layout_create_info.hpp>
#include <gct/buffer_view_create_info.hpp>
#include <gct/submit_info.hpp>
#include <gct/fence.hpp>
#include <gct/wait_for_sync.hpp>
#include <gct/present_info.hpp>
#include <gct/gltf.hpp>

struct fb_resources_t {
  std::shared_ptr< gct::image_t > color;
  std::shared_ptr< gct::framebuffer_t > framebuffer;
  std::shared_ptr< gct::semaphore_t > image_acquired;
  std::shared_ptr< gct::semaphore_t > draw_complete;
  std::shared_ptr< gct::semaphore_t > image_ownership;
  std::shared_ptr< gct::bound_command_buffer_t > command_buffer;
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
      "test_gct",
      VK_MAKE_VERSION( 1, 0, 0 ),
      VK_MAKE_VERSION( 1, 2, 0 ),
      iext,
      /*std::vector< const char* >{
        VK_KHR_DISPLAY_EXTENSION_NAME,
        VK_KHR_SURFACE_EXTENSION_NAME
      },*/
      std::vector< const char* >{
        "VK_LAYER_KHRONOS_validation"
      }
    )
  );

  auto groups = instance->get_physical_devices( {} );
  auto selected = groups[ 0 ].with_extensions( {
    VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
    VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
    VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME,
    VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME,
    VK_KHR_MAINTENANCE1_EXTENSION_NAME
  } );
  std::cout << nlohmann::json( groups[ 0 ] ).dump( 2 ) << std::endl;

  std::uint32_t width = 1920u;
  std::uint32_t height = 1080u;

  gct::glfw_window window( width, height, "window title", false );
  window.set_on_closed( []( auto & ) { std::cout << "closed" << std::endl; } );
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
    },
    gct::queue_requirement_t{
      vk::QueueFlagBits::eTransfer,
      0u,
      vk::Extent3D(),
#ifdef VK_EXT_GLOBAL_PRIORITY_EXTENSION_NAME
      vk::QueueGlobalPriorityEXT(),
#endif
      {},
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

  auto descriptor_pool = device->get_descriptor_pool(
    gct::descriptor_pool_create_info_t()
      .set_basic(
        vk::DescriptorPoolCreateInfo()
          .setFlags( vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet )
          .setMaxSets( 400 )
      )
      .set_descriptor_pool_size( vk::DescriptorType::eUniformBuffer, 400 )
      .set_descriptor_pool_size( vk::DescriptorType::eCombinedImageSampler, 400 )
      .rebuild_chain()
  );

  auto pipeline_cache = device->get_pipeline_cache();

  std::vector< std::shared_ptr< gct::render_pass_t > > render_pass;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    render_pass.emplace_back( device->get_render_pass(
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
    ) );
  }
  auto allocator = device->get_allocator();
  
  std::vector< fb_resources_t > framebuffers;

  std::vector< std::shared_ptr< gct::buffer_t > > staging_dynamic_uniform;
  std::vector< std::shared_ptr< gct::buffer_t > > dynamic_uniform;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    staging_dynamic_uniform.emplace_back(
      allocator->create_buffer(
        gct::buffer_create_info_t()
          .set_basic(
            vk::BufferCreateInfo()
              .setSize( sizeof( gct::gltf::dynamic_uniforms_t ) )
              .setUsage( vk::BufferUsageFlagBits::eTransferSrc )
          ),
          VMA_MEMORY_USAGE_CPU_TO_GPU
       )
    );
    dynamic_uniform.emplace_back(
      allocator->create_buffer(
        gct::buffer_create_info_t()
          .set_basic(
            vk::BufferCreateInfo()
              .setSize( sizeof( gct::gltf::dynamic_uniforms_t ) )
              .setUsage( vk::BufferUsageFlagBits::eTransferDst|vk::BufferUsageFlagBits::eUniformBuffer )
          ),
          VMA_MEMORY_USAGE_GPU_ONLY
       )
    );
  }

  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    auto &image = swapchain_images[ i ];
    auto &render_pass_ = render_pass[ i ];
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
        std::move( framebuffer ),
        device->get_semaphore(),
        device->get_semaphore(),
        device->get_semaphore(),
        queue->get_command_pool()->allocate()
      }
    );
  }

  auto image = allocator->create_image(
    gct::image_create_info_t()
      .set_basic(
        vk::ImageCreateInfo()
          .setImageType( vk::ImageType::e2D )
          .setFormat( surface->get_caps().get_formats()[ 0 ].basic.format )
          .setExtent(
            vk::Extent3D()
              .setWidth( surface->get_caps().get_basic().currentExtent.width )
              .setHeight( surface->get_caps().get_basic().currentExtent.height )
              .setDepth( 1 )
          )
          .setMipLevels( 1 )
          .setArrayLayers( 1 )
          .setSamples( vk::SampleCountFlagBits::e1 )
          .setTiling( vk::ImageTiling::eOptimal )
          .setUsage( vk::ImageUsageFlagBits::eColorAttachment )
          .setSharingMode( vk::SharingMode::eExclusive )
          .setInitialLayout( vk::ImageLayout::eUndefined )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  auto image_view = image->get_view( vk::ImageAspectFlagBits::eColor );
  
  auto buffer = allocator->create_buffer(
    gct::buffer_create_info_t()
      .set_basic(
        vk::BufferCreateInfo()
          .setSize( 1024 )
          .setUsage( vk::BufferUsageFlagBits::eTransferSrc )
      ),
    VMA_MEMORY_USAGE_CPU_TO_GPU
  );
  for( auto &v: buffer->map< std::uint8_t >() )
    v = 1;
  auto buffer2 = allocator->create_buffer(
    gct::buffer_create_info_t()
      .set_basic(
        vk::BufferCreateInfo()
          .setSize( 1024 )
          .setUsage( vk::BufferUsageFlagBits::eUniformTexelBuffer )
      ),
    VMA_MEMORY_USAGE_CPU_TO_GPU
  );
  auto buffer_view = buffer2->get_view(
    gct::buffer_view_create_info_t()
      .set_basic(
        vk::BufferViewCreateInfo()
          .setOffset( 0 )
          .setRange( 100 )
          .setFormat( vk::Format::eR8G8B8A8Unorm )
      )
  );

  gct::gltf::document_t doc;
  {
    auto rec = gcb->begin();
    //rec.load_image( allocator, "/home/fadis/gltf/BoomBox/glTF/BoomBox_baseColor.png", vk::ImageUsageFlagBits::eSampled, true, false );
    doc = gct::gltf::load_gltf(
      "/home/fadis/gltf/Sponza/glTF/Sponza.gltf",
      device,
      rec,
      allocator,
      descriptor_pool,
      render_pass,
      "/home/fadis/git/gct/shaders",
      0,
      framebuffers.size(),
      0,
      dynamic_uniform,
      float( width ) / float( height )
    );
  }
  gcb->execute(
    gct::submit_info_t()
  );
  gcb->wait_for_executed();

  auto center = ( doc.node.min + doc.node.max ) / 2.f;
  auto scale = std::abs( glm::length( doc.node.max - doc.node.min ) );
  const std::array< vk::ClearValue, 2 > clear_values{
    vk::ClearColorValue( std::array< float, 4u >{ 1.0f, 0.0f, 1.0f, 1.0f } ),
    vk::ClearDepthStencilValue( 1.f, 0 )
  };
  auto const viewport =
    vk::Viewport()
      .setWidth( width )
      .setHeight( height )
      .setMinDepth( 0.0f )
      .setMaxDepth( 1.0f );
  vk::Rect2D const scissor( vk::Offset2D(0, 0), vk::Extent2D( width, height ) );


  auto lhrh = glm::mat4(-1,0,0,0,0,-1,0,0,0,0,1,0,0,0,0,1);
  const glm::mat4 projection = lhrh * glm::perspective( 0.39959648408210363f, (float(width)/float(height)), std::min(0.1f*scale,0.5f), 150.f*scale );
  auto camera_pos = center + glm::vec3{ 0.f, 0.f, 1.0f*scale };
  float camera_angle = 0;//M_PI;
  auto speed = 0.01f*scale;
  auto light_pos = glm::vec3{ 0.0f*scale, 1.2f*scale, 0.0f*scale };
  float light_energy = 5.0f;
  const auto point_lights = gct::gltf::get_point_lights(
    doc.node,
    doc.point_light
  );
  if( !point_lights.empty() ) {
    light_energy = point_lights[ 0 ].intensity / ( 4 * M_PI ) / 100;
    light_pos = point_lights[ 0 ].location;
  }
  std::unordered_set< int > pressed_keys;
  window.set_on_key( [&]( auto &, int key, int scancode, int action, int mods ) {
    if( action == GLFW_RELEASE )
      pressed_keys.erase( key );
    else if( action == GLFW_PRESS )
      pressed_keys.insert( key );
  } );

  uint32_t current_frame = 0u;
  uint32_t last_image_index = framebuffers.size();
  while( pressed_keys.find( GLFW_KEY_Q ) == pressed_keys.end() ) {
    const auto begin_time = std::chrono::high_resolution_clock::now();
    if( pressed_keys.find( GLFW_KEY_A ) != pressed_keys.end() )
      camera_angle += 0.01 * M_PI/2;
    if( pressed_keys.find( GLFW_KEY_D ) != pressed_keys.end() )
      camera_angle -= 0.01 * M_PI/2;
    glm::vec3 camera_direction( std::sin( camera_angle ), 0, -std::cos( camera_angle ) );
    if( pressed_keys.find( GLFW_KEY_W ) != pressed_keys.end() )
      camera_pos += camera_direction * glm::vec3( speed );
    if( pressed_keys.find( GLFW_KEY_S ) != pressed_keys.end() )
      camera_pos -= camera_direction * glm::vec3( speed );
    if( pressed_keys.find( GLFW_KEY_E ) != pressed_keys.end() )
      camera_pos[ 1 ] += speed;
    if( pressed_keys.find( GLFW_KEY_C ) != pressed_keys.end() )
      camera_pos[ 1 ] -= speed;
    if( pressed_keys.find( GLFW_KEY_J ) != pressed_keys.end() )
      light_energy += 0.05f;
    if( pressed_keys.find( GLFW_KEY_K ) != pressed_keys.end() )
      light_energy -= 0.05f;
    if( pressed_keys.find( GLFW_KEY_UP ) != pressed_keys.end() )
      light_pos[ 2 ] += speed;
    if( pressed_keys.find( GLFW_KEY_DOWN ) != pressed_keys.end() )
      light_pos[ 2 ] -= speed;
    if( pressed_keys.find( GLFW_KEY_LEFT ) != pressed_keys.end() )
      light_pos[ 0 ] -= speed;
    if( pressed_keys.find( GLFW_KEY_RIGHT ) != pressed_keys.end() )
      light_pos[ 0 ] += speed;

    glm::mat4 lookat = glm::lookAt(
      camera_pos,
      camera_pos + camera_direction,
      glm::vec3{ 0.f, camera_pos[ 1 ] + 100.f*scale, 0.f }
    );
    std::cout << glm::to_string( camera_pos ) << std::endl;
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
        .set_camera_matrix( lookat )
        .set_eye_pos( glm::vec4( camera_pos, 1.0 ) )
        .set_light_pos( glm::vec4( light_pos, 1.0 ) )
        .set_light_energy( light_energy );
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

      auto const pass_info = vk::RenderPassBeginInfo()
        .setRenderPass( **render_pass[ image_index ] )
        .setFramebuffer( **fb.framebuffer )
        .setRenderArea( vk::Rect2D( vk::Offset2D(0, 0), vk::Extent2D((uint32_t)width, (uint32_t)height) ) )
        .setClearValueCount( clear_values.size() )
        .setPClearValues( clear_values.data() );
       rec->beginRenderPass( &pass_info, vk::SubpassContents::eInline );
       rec->setViewport( 0, 1, &viewport );
       rec->setScissor( 0, 1, &scissor );
       gct::gltf::draw_node(
         rec,
         doc.node,
         doc.mesh,
         doc.buffer,
         image_index,
         0u
       );
       rec->endRenderPass();
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
    gct::wait_for_sync( begin_time );
  }
  (*queue)->waitIdle();
}

