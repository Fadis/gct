#include <iostream>
#include <unordered_set>
#include <utility>
#include <nlohmann/json.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
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
#include <gct/sampler_create_info.hpp>
#include <gct/image_view_create_info.hpp>
#include <gct/submit_info.hpp>
#include <gct/fence.hpp>
#include <gct/wait_for_sync.hpp>
#include <gct/present_info.hpp>
#include <gct/gltf.hpp>
#include <gct/vertex_attributes.hpp>
#include <gct/render_pass_begin_info.hpp>
#include <gct/primitive.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_pool.hpp>
#include <gct/framebuffer.hpp>
#include <gct/render_pass.hpp>

struct fb_resources_t {
  std::shared_ptr< gct::image_t > color;
  std::shared_ptr< gct::framebuffer_t > framebuffer;
  std::shared_ptr< gct::semaphore_t > image_acquired;
  std::shared_ptr< gct::semaphore_t > draw_complete;
  std::shared_ptr< gct::semaphore_t > image_ownership;
  std::shared_ptr< gct::bound_command_buffer_t > command_buffer;
  gct::render_pass_begin_info_t render_pass_begin_info;
  std::shared_ptr< gct::descriptor_set_t > descriptor_set;
  std::shared_ptr< gct::buffer_t > uniform_staging;
  std::shared_ptr< gct::buffer_t > uniform;
};

struct uniform_t {
  LIBGCT_SETTER( projection_matrix )
  LIBGCT_SETTER( camera_matrix )
  LIBGCT_SETTER( world_matrix )
  LIBGCT_SETTER( eye_pos )
  LIBGCT_SETTER( light_pos )
  LIBGCT_SETTER( light_energy )
  glm::mat4 projection_matrix;
  glm::mat4 camera_matrix;
  glm::mat4 world_matrix;
  glm::vec4 eye_pos;
  glm::vec4 light_pos;
  float light_energy;
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
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME,
    VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME,
    VK_KHR_MAINTENANCE1_EXTENSION_NAME,
    VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
  } );

  std::uint32_t width = 1024u;
  std::uint32_t height = 1024u;

  gct::glfw_window window( width, height, "ball", false );
  bool close_app = false;
  bool iconified = false;
  window.set_on_closed( [&]( auto & ) { close_app = true; } );
  window.set_on_key( [&]( auto &, int key, int, int action, int ) {
    if( action == GLFW_PRESS ) {
      if( key == GLFW_KEY_Q ) close_app = true;
    }
  } );
  window.set_on_iconified(
    [&]( auto&, int i ) {
      iconified = i;
    }
  );

  gct::glfw::get().poll();
  auto surface = window.get_surface( *groups[ 0 ].devices[ 0 ] );
 
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
      .set_descriptor_pool_size( vk::DescriptorType::eCombinedImageSampler, 4 )
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

  const auto vs = device->get_shader_module( "./src/example/textured/shader.vert.spv" );
  const auto fs = device->get_shader_module( "./src/example/textured/shader.frag.spv" );
 
  const auto descriptor_set_layout = device->get_descriptor_set_layout(
    gct::descriptor_set_layout_create_info_t()
      .add_binding(
        vs->get_props().get_reflection()
      )
      .add_binding(
        fs->get_props().get_reflection()
      )
  );
  std::cout << nlohmann::json( descriptor_set_layout->get_props() ) << std::endl;

  auto base_color_sampler = device->get_sampler(
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

  const auto pipeline_layout = device->get_pipeline_layout(
    gct::pipeline_layout_create_info_t()
      .add_descriptor_set_layout( descriptor_set_layout )
  );

  auto [vistat,vamap,stride] = get_vertex_attributes(
    *device,
    vs->get_props().get_reflection()
  );

  const auto [input_assembly,host_vertex_buffer,vertex_count] = gct::primitive::create_sphere( vamap, stride, 12u, 6u );
  //const auto [input_assembly,host_vertex_buffer,vertex_count] = gct::primitive::create_cube( vamap, stride );

  std::shared_ptr< gct::buffer_t > vertex_buffer;
  std::shared_ptr< gct::image_t > base_color_image;
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
      base_color_image = recorder.load_image(
        allocator,
        "../images/globe_color.png",
        vk::ImageUsageFlagBits::eSampled,
        true, true
      );
      recorder.barrier(
        vk::AccessFlagBits::eTransferWrite,
        vk::AccessFlagBits::eVertexAttributeRead,
        vk::PipelineStageFlagBits::eTransfer,
        vk::PipelineStageFlagBits::eVertexInput,
        vk::DependencyFlagBits( 0 ),
        { vertex_buffer },
        { base_color_image }
      );
    }
    command_buffer->execute(
      gct::submit_info_t()
    );
    command_buffer->wait_for_executed();
  }
  auto base_color_image_view = base_color_image->get_view(
    gct::image_view_create_info_t()
      .set_basic(
        vk::ImageViewCreateInfo()
          .setSubresourceRange(
            vk::ImageSubresourceRange()
              .setAspectMask( vk::ImageAspectFlagBits::eColor )
              .setBaseMipLevel( 0 )
              .setLevelCount( base_color_image->get_props().get_basic().mipLevels )
              .setBaseArrayLayer( 0 )
              .setLayerCount( base_color_image->get_props().get_basic().arrayLayers )
          )
          .setViewType( gct::to_image_view_type( base_color_image->get_props().get_basic().imageType, base_color_image->get_props().get_basic().arrayLayers ) )
          .setFormat( vk::Format::eR8G8B8A8Srgb )
      )
      .rebuild_chain()
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
    auto descriptor_set = descriptor_pool->allocate( descriptor_set_layout );
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
    updates.push_back(
      gct::write_descriptor_set_t()
        .set_basic(
          (*descriptor_set)[ "base_color" ]
        )
        .add_image(
          gct::descriptor_image_info_t()
            .set_sampler( base_color_sampler )
            .set_image_view( base_color_image_view )
            .set_basic(
              vk::DescriptorImageInfo()
                .setImageLayout(
                  base_color_image->get_layout().get_uniform_layout()
                )
            )
        )
    );
    descriptor_set->update( updates );
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
          .rebuild_chain(),
        descriptor_set,
        uniform_staging,
        uniform
      }
    );
  }

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
          .setPolygonMode( vk::PolygonMode::eFill )
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

  auto camera_pos = glm::vec3{ 0.f, -3.f, 6.0f };
  float camera_angle = 0;//M_PI;
  glm::vec3 camera_direction( std::sin( camera_angle ), 0, -std::cos( camera_angle ) );
  auto uniforms = uniform_t()
    .set_projection_matrix(
      glm::perspective( 0.39959648408210363f, (float(width)/float(height)), 0.1f, 150.f )
    )
    .set_camera_matrix(
      glm::lookAt(
        camera_pos,
        glm::vec3( 0.f, 0.f, 0.f ),
        glm::vec3{ 0.f, camera_pos[ 1 ] + 100.f, 0.f }
      )
    )
    .set_world_matrix(
      glm::mat4( 1.0 )
    )
    .set_eye_pos( glm::vec4{ camera_pos[ 0 ], camera_pos[ 1 ], camera_pos[ 2 ], 1.0 } )
    .set_light_pos( glm::vec4( 2.0, -2.0, 2.0, 1.0 ) )
    .set_light_energy( 8.0 );

  uint32_t current_frame = 0u;
  float angle = 0.f;
  while( !close_app ) {
    const auto begin_time = std::chrono::high_resolution_clock::now();
    angle += 1.f / 60.f;
    uniforms
      .set_world_matrix(
        glm::mat4(
          std::cos( angle ), 0.f, -std::sin( angle ), 0.f,
          0.f, 1.f, 0.f, 0.f,
          std::sin( angle ), 0.f, std::cos( angle ), 0.f,
          0.f, 0.f, 0.f, 1.f
        )
      );

    if( !iconified ) {
      auto &sync = framebuffers[ current_frame ];
      sync.command_buffer->wait_for_executed();
      auto image_index = swapchain->acquire_next_image( sync.image_acquired );
      auto &fb = framebuffers[ image_index ];
      {
        auto recorder = sync.command_buffer->begin();
        recorder.copy(
          uniforms,
          fb.uniform_staging,
          fb.uniform
        );
        recorder.barrier(
          vk::AccessFlagBits::eTransferRead,
          vk::AccessFlagBits::eShaderRead,
          vk::PipelineStageFlagBits::eTransfer,
          vk::PipelineStageFlagBits::eVertexShader,
          vk::DependencyFlagBits( 0 ),
          { fb.uniform },
          {}
        );
        auto render_pass_token = recorder.begin_render_pass(
          fb.render_pass_begin_info,
          vk::SubpassContents::eInline
        );
        recorder.bind_pipeline( pipeline );
        recorder.bind_descriptor_set(
          vk::PipelineBindPoint::eGraphics,
          pipeline_layout,
          fb.descriptor_set
        );
        recorder.bind_vertex_buffer( vertex_buffer );
        recorder->draw( vertex_count, 1, 0, 0 );
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
    }
    glfwPollEvents();
    gct::wait_for_sync( begin_time );
  }
  (*queue)->waitIdle();
}

