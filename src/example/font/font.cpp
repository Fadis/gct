#include <iostream>
#include <unordered_set>
#include <utility>
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
#include <gct/image_create_info.hpp>
#include <gct/image.hpp>
#include <gct/swapchain.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/pipeline_layout_create_info.hpp>
#include <gct/pipeline_viewport_state_create_info.hpp>
#include <gct/pipeline_dynamic_state_create_info.hpp>
#include <gct/pipeline_input_assembly_state_create_info.hpp>
#include <gct/pipeline_vertex_input_state_create_info.hpp>
#include <gct/pipeline_multisample_state_create_info.hpp>
#include <gct/pipeline_tessellation_state_create_info.hpp>
#include <gct/pipeline_rasterization_state_create_info.hpp>
#include <gct/pipeline_depth_stencil_state_create_info.hpp>
#include <gct/pipeline_color_blend_state_create_info.hpp>
#include <gct/graphics_pipeline_create_info.hpp>
#include <gct/graphics_pipeline.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/buffer_view_create_info.hpp>
#include <gct/submit_info.hpp>
#include <gct/fence.hpp>
#include <gct/wait_for_sync.hpp>
#include <gct/present_info.hpp>
#include <gct/gltf.hpp>
#include <gct/vertex_attributes.hpp>
#include <gct/render_pass_begin_info.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_pool.hpp>
#include <gct/framebuffer.hpp>
#include <gct/render_pass.hpp>
#include <gct/primitive.hpp>
#include <gct/vulkanhpp.hpp>
#include <gct/display_surface_create_info.hpp>
#include <gct/font.hpp>
#include <gct/dpi.hpp>

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

struct uniform_t {
  LIBGCT_SETTER( world_matrix )
  glm::mat4 world_matrix;
  LIBGCT_SETTER( char_matrix )
  glm::mat2 char_matrix[ 1024 ];
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
    //VK_KHR_DISPLAY_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME,
    VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME,
    VK_KHR_MAINTENANCE1_EXTENSION_NAME,
    VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
  } );
//  std::cout << nlohmann::json( groups[ 0 ] ).dump( 2 ) << std::endl;
  
  auto &displays = selected.devices[ 0 ]->get_props().get_display();
  if( displays.size() == 0u ) {
    std::abort();
  }


  std::cout << "DPI" << nlohmann::json( gct::get_dpi( displays[ 0 ] ) ) << std::endl;
  auto [xpx,ypx] = gct::pt_to_px( gct::get_dpi( displays[ 0 ] ), 96 );
  std::cout << "96ptの文字は " << xpx << " x " << xpx << " ピクセル" << std::endl;
  auto [sxpx,sypx] = gct::pt_to_px( gct::get_dpi( displays[ 0 ] ), 7 );
  std::cout << "7ptの文字は " << sxpx << " x " << sxpx << " ピクセル" << std::endl;

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
  auto gcb = queue->get_command_pool()->allocate();

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
  
  const auto render_pass = device->get_render_pass(
    gct::select_simple_surface_format( surface->get_caps().get_formats() ).basic.format,
    vk::Format::eD16Unorm
  );

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

    auto framebuffer = render_pass->get_framebuffer(
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
          .add_clear_value( vk::ClearColorValue( std::array< float, 4u >{ 0.0f, 0.0f, 0.0f, 1.0f } ) )
          .add_clear_value( vk::ClearDepthStencilValue( 1.f, 0 ) )
          .rebuild_chain()
      }
    );
  }

  const auto vs = device->get_shader_module( "./src/example/font/shader.vert.spv" );
  const auto fs = device->get_shader_module( "./src/example/font/shader.frag.spv" );
 
  const auto descriptor_set_layout = device->get_descriptor_set_layout(
    gct::descriptor_set_layout_create_info_t()
      .add_binding(
        vs->get_props().get_reflection()
      )
      .add_binding(
        fs->get_props().get_reflection()
      )
  );
  
  const auto descriptor_set = descriptor_pool->allocate( descriptor_set_layout );

  auto uniform_host = uniform_t()
    .set_world_matrix(
      glm::translate(
        glm::scale(
          glm::mat4(
            1.f, 0.f, 0.f, 0.f,
            0.f, -1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f
          ),
          glm::vec3( float( height )/float( width ), 1.f, 1.f )
        ),
        glm::vec3( -0.5f, -0.5f, 0.0f )
      )
    );

  auto uniform_staging = allocator->create_buffer(
    gct::buffer_create_info_t()
      .set_basic(
        vk::BufferCreateInfo()
          .setSize( sizeof( uniform_t ) )
          .setUsage( vk::BufferUsageFlagBits::eTransferSrc )
      ),
    VMA_MEMORY_USAGE_CPU_TO_GPU
  );
  auto uniform = allocator->create_buffer(
    gct::buffer_create_info_t()
      .set_basic(
        vk::BufferCreateInfo()
          .setSize( sizeof( uniform_t ) )
          .setUsage( vk::BufferUsageFlagBits::eTransferDst|vk::BufferUsageFlagBits::eUniformBuffer )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY
  );

  std::vector< gct::write_descriptor_set_t > updates;
  updates.push_back(
    gct::write_descriptor_set_t()
      .set_basic(
        (*descriptor_set)[ "uniforms" ]
      )
      .add_buffer(
        gct::descriptor_buffer_info_t()
          .set_buffer( uniform )
          .set_basic(
            vk::DescriptorBufferInfo()
              .setOffset( 0 )
              .setRange( sizeof( uniform_t ) )
          )
      )
  );
  descriptor_set->update( updates );

  const auto pipeline_layout = device->get_pipeline_layout(
    gct::pipeline_layout_create_info_t()
      .add_descriptor_set_layout( descriptor_set_layout )
  );

  auto [vistat,vamap,stride] = get_vertex_attributes(
    *device,
    vs->get_props().get_reflection()
  );

  gct::font font;
  {
    auto command_buffer = queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.copy(
        uniform_host,
        uniform_staging,
        uniform
      );
      font = recorder.load_font(
        "font.gltf",
        allocator
      );
      recorder.barrier(
        vk::AccessFlagBits::eTransferWrite,
        vk::AccessFlagBits::eVertexAttributeRead,
        vk::PipelineStageFlagBits::eTransfer,
        vk::PipelineStageFlagBits::eVertexInput,
        vk::DependencyFlagBits( 0 ),
        { uniform, font.buffer },
        {}
      );
    }
    command_buffer->execute(
      gct::submit_info_t()
    );
    command_buffer->wait_for_executed();
  }
  const auto input_assembly = gct::pipeline_input_assembly_state_create_info_t()
    .set_basic(
      vk::PipelineInputAssemblyStateCreateInfo()
        .setTopology( vk::PrimitiveTopology::eTriangleList )
    );


  const auto viewport =
    gct::pipeline_viewport_state_create_info_t()
      /*.set_basic(
        vk::PipelineViewportStateCreateInfo()
      )*/
      .add_viewport(
        vk::Viewport()
          .setWidth( width )
          .setHeight( height )
          .setMinDepth( 0.0f )
          .setMaxDepth( 1.0f )
      )
      .add_scissor(
        vk::Rect2D()
          .setOffset( { 0, 0 } )
          .setExtent( { width, height } )
      )
      .rebuild_chain();

  const auto rasterization =
    gct::pipeline_rasterization_state_create_info_t()
      .set_basic(
        vk::PipelineRasterizationStateCreateInfo()
          .setDepthClampEnable( false )
          .setRasterizerDiscardEnable( false )
          .setPolygonMode( vk::PolygonMode::eLine )
          .setCullMode( vk::CullModeFlagBits::eNone )
          .setFrontFace( vk::FrontFace::eClockwise )
          .setDepthBiasEnable( false )
          .setLineWidth( 1.0f )
      );

  const auto multisample =
    gct::pipeline_multisample_state_create_info_t()
      .set_basic(
        vk::PipelineMultisampleStateCreateInfo()
      );

  const auto stencil_op = vk::StencilOpState()
    .setCompareOp( vk::CompareOp::eAlways )
    .setFailOp( vk::StencilOp::eKeep )
    .setPassOp( vk::StencilOp::eKeep );

  const auto depth_stencil =
    gct::pipeline_depth_stencil_state_create_info_t()
      .set_basic(
        vk::PipelineDepthStencilStateCreateInfo()
          .setDepthTestEnable( true )
          .setDepthWriteEnable( true )
          .setDepthCompareOp( vk::CompareOp::eLessOrEqual )
          .setDepthBoundsTestEnable( false )
          .setStencilTestEnable( false )
          .setFront( stencil_op )
          .setBack( stencil_op )
      );

  const auto color_blend =
    gct::pipeline_color_blend_state_create_info_t()
      .add_attachment(
        vk::PipelineColorBlendAttachmentState()
          .setBlendEnable( false )
          .setColorWriteMask(
            vk::ColorComponentFlagBits::eR |
            vk::ColorComponentFlagBits::eG |
            vk::ColorComponentFlagBits::eB |
            vk::ColorComponentFlagBits::eA
          )
      );

  auto pipeline = pipeline_cache->get_pipeline(
    gct::graphics_pipeline_create_info_t()
      .add_stage( vs )
      .add_stage( fs )
      .set_vertex_input( vistat )
      .set_input_assembly( input_assembly )
      .set_viewport( viewport )
      .set_rasterization( rasterization )
      .set_multisample( multisample )
      .set_depth_stencil( depth_stencil )
      .set_color_blend( color_blend )
      .set_dynamic(
        gct::pipeline_dynamic_state_create_info_t()
      )
      .set_layout( pipeline_layout )
      .set_render_pass( render_pass, 0 )
  );

  uint32_t current_frame = 0u;
  auto glyph = font.index.begin();
  while( 1 ) {
    const auto begin_time = std::chrono::high_resolution_clock::now();
    auto &sync = framebuffers[ current_frame ];
    sync.command_buffer->wait_for_executed();
    auto image_index = swapchain->acquire_next_image( sync.image_acquired );
    auto &fb = framebuffers[ image_index ];
    {
      auto recorder = sync.command_buffer->begin();
      auto render_pass_token = recorder.begin_render_pass(
        fb.render_pass_begin_info,
        vk::SubpassContents::eInline
      );
      recorder.bind_pipeline( pipeline );
      recorder.bind_descriptor_set(
        vk::PipelineBindPoint::eGraphics,
        pipeline_layout,
        descriptor_set
      );
      recorder.bind_vertex_buffer( font.buffer );
      recorder.bind_index_buffer( font.buffer, font.offset, vk::IndexType::eUint32 );
      recorder->drawIndexed( glyph->second.vertex_count, 1, glyph->second.vertex_offset, 0, 0 );
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
    ++current_frame;
    current_frame %= framebuffers.size();
    ++glyph;
    if( glyph == font.index.end() )
      glyph = font.index.begin();
    gct::wait_for_sync( begin_time );
  }
  (*queue)->waitIdle();

}

