#include <iostream>
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

struct tone_state_t {
  std::uint32_t max;
  float scale;
};

struct shadow_matrices_t {
  glm::mat4 camera_projection[ 6 ];
  glm::mat4 camera[ 6 ];
};

int main( int argc, const char *argv[] ) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  desc.add_options()
    ( "help,h", "show this message" )
    ( "walk,w", po::value< std::string >()->default_value(".walk"), "walk state filename" )
    ( "model,m", po::value< std::string >(), "glTF filename" )
    ( "ambient,a", po::value< float >()->default_value( 0.1 ), "ambient light level" )
    ( "validation,v", po::bool_switch(), "enable validation layer" );
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) || !vm.count( "model" ) ) {
    std::cout << desc << std::endl;
    return 0;
  }
  const std::string walk_state_filename = vm[ "walk" ].as< std::string >();
  const std::string model_filename = vm[ "model" ].as< std::string >();
  const float ambient_level = std::min( std::max( vm[ "ambient" ].as< float >(), 0.f ), 1.f );

  gct::glfw::get();
  uint32_t iext_count = 0u;
  auto exts = glfwGetRequiredInstanceExtensions( &iext_count );
  std::vector< const char* > iext{};
  for( uint32_t i = 0u; i != iext_count; ++i )
    iext.push_back( exts[ i ] );
  iext.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
  const auto ilayers = gct::get_instance_layers();
  const auto iexts = gct::get_instance_extensions( std::vector< const char* >() );
  auto ici = gct::instance_create_info_t()
    .set_application_info(
      vk::ApplicationInfo()
        .setPApplicationName( "my_application" )
        .setApplicationVersion(  VK_MAKE_VERSION( 1, 0, 0 ) )
        .setApiVersion( VK_MAKE_VERSION( 1, 3, 0 ) )
    )
    .add_extension(
      iext.begin(), iext.end()
    );
  if( vm[ "validation" ].as< bool >() ) {
    ici.add_layer(
      "VK_LAYER_KHRONOS_validation"
    );
  }
  std::shared_ptr< gct::instance_t > instance(
    new gct::instance_t( ici )
  );
  instance->abort_on_validation_failure();

  auto groups = instance->get_physical_devices( {} );
  auto selected = groups[ 0 ].with_extensions( {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME,
    VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME,
    VK_KHR_MAINTENANCE1_EXTENSION_NAME,
    VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
  } );

  std::uint32_t width = 1920u*2;
  std::uint32_t height = 1080u*2;

  gct::glfw_window window( width, height, "window title", false );
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
  auto gcb = queue->get_command_pool()->allocate();

  auto swapchain = device->get_swapchain( surface );
  auto swapchain_images = swapchain->get_images();

  auto descriptor_pool = device->get_descriptor_pool(
    gct::descriptor_pool_create_info_t()
      .set_basic(
        vk::DescriptorPoolCreateInfo()
          .setFlags( vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet )
          .setMaxSets( 250 )
      )
      .set_descriptor_pool_size( vk::DescriptorType::eUniformBuffer, 2 )
      .set_descriptor_pool_size( vk::DescriptorType::eStorageBuffer, 1 )
      .set_descriptor_pool_size( vk::DescriptorType::eCombinedImageSampler, 9 )
      .set_descriptor_pool_size( vk::DescriptorType::eStorageImage, 8 )
      .rebuild_chain()
  );

  auto pipeline_cache = device->get_pipeline_cache();

  VmaAllocatorCreateInfo allocator_create_info{};
  const auto allocator = device->get_allocator(
    allocator_create_info
  );

  const auto [shadow_mat_descriptor_set_layout,shadow_mat_pipeline] = pipeline_cache->get_pipeline(
    CMAKE_CURRENT_BINARY_DIR "/shadow_mat/shadow_mat.comp.spv"
  );

  std::vector< std::shared_ptr< gct::buffer_t > > shadow_mat_storage;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    shadow_mat_storage.emplace_back(
      allocator->create_buffer(
        sizeof( shadow_matrices_t ),
        vk::BufferUsageFlagBits::eStorageBuffer,
        VMA_MEMORY_USAGE_GPU_ONLY
      )
    );
  }
  std::vector< std::shared_ptr< gct::descriptor_set_t > > shadow_mat_descriptor_set;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    shadow_mat_descriptor_set.push_back(
      descriptor_pool->allocate(
        shadow_mat_descriptor_set_layout
      )
    );
    shadow_mat_descriptor_set.back()->update({
      { "matrices", shadow_mat_storage[ i ] }
    });
  }
  
  const unsigned int shadow_map_size = 1024u;
  gct::gbuffer shadow_gbuffer(
    gct::gbuffer_create_info()
      .set_allocator( allocator )
      .set_width( shadow_map_size )
      .set_height( shadow_map_size )
      .set_layer( 6 )
      .set_swapchain_image_count( swapchain_images.size() )
      .set_color_buffer_count( 1 )
      .set_flags( vk::ImageCreateFlagBits::eCubeCompatible )
      .set_format( vk::Format::eR32G32Sfloat ) 
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
      .set_clear_color( gct::color::web::white )
  );

  gct::cubemap_images2 cubemap_images( shadow_gbuffer.get_image_views() );

  auto cubemap_sampler = device->get_sampler(
    gct::get_basic_linear_sampler_create_info()
  );

  constexpr std::size_t gbuf_count = 8u;
  gct::gbuffer gbuffer(
    allocator,
    width,
    height,
    swapchain_images.size(),
    gbuf_count
  );

  const auto dynamic_descriptor_set_layout = device->get_descriptor_set_layout(
    {
      CMAKE_CURRENT_BINARY_DIR "/geometry",
      CMAKE_CURRENT_BINARY_DIR "/shadow"
    },
    1u
  );
  std::vector< std::shared_ptr< gct::mappable_buffer_t > > dynamic_uniform;
  std::vector< std::shared_ptr< gct::descriptor_set_t > > dynamic_descriptor_set;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    dynamic_uniform.emplace_back(
      allocator->create_mappable_buffer(
        sizeof( gct::gltf::dynamic_uniforms_t ),
        vk::BufferUsageFlagBits::eUniformBuffer
      )
    );
    dynamic_descriptor_set.push_back(
      descriptor_pool->allocate(
        dynamic_descriptor_set_layout
      )
    );
    dynamic_descriptor_set.back()->update({
      { "dynamic_uniforms", dynamic_uniform.back() },
      { "shadow", cubemap_sampler, cubemap_images.get_cube_image_views()[ i ], vk::ImageLayout::eShaderReadOnlyOptimal }
    });
  }

  const auto rgba32ici =
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( width, height )
          .setUsage( vk::ImageUsageFlagBits::eStorage )
      );

  const auto [light_descriptor_set_layout,light_pipeline] = pipeline_cache->get_pipeline(
    CMAKE_CURRENT_BINARY_DIR "/lighting/lighting.comp.spv"
  );

  std::vector< std::shared_ptr< gct::image_view_t > > diffuse;
  std::vector< std::shared_ptr< gct::image_view_t > > specular;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    diffuse.push_back(
      allocator->create_image(
        rgba32ici,
        VMA_MEMORY_USAGE_GPU_ONLY
      )->get_view( vk::ImageAspectFlagBits::eColor )
    );
    specular.push_back(
      allocator->create_image(
        rgba32ici,
        VMA_MEMORY_USAGE_GPU_ONLY
      )->get_view( vk::ImageAspectFlagBits::eColor )
    );
  }
  {
    auto command_buffer = queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( gbuffer.get_image_views(), vk::ImageLayout::eGeneral );
      recorder.set_image_layout( diffuse, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( specular, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }
  std::vector< std::shared_ptr< gct::descriptor_set_t > > light_descriptor_set;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    light_descriptor_set.push_back(
      descriptor_pool->allocate(
        light_descriptor_set_layout
      )
    );
    light_descriptor_set.back()->update({
      { "gbuffer", gbuffer.get_image_view( i ) },
      { "diffuse_image", diffuse[ i ] },
      { "specular_image", specular[ i ] },
      { "dynamic_uniforms", dynamic_uniform[ i ] }
    });
  }

  std::vector< std::shared_ptr< gct::buffer_t > > tone;
  std::vector< std::shared_ptr< gct::buffer_t > > tone_staging;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    tone.push_back(
      allocator->create_buffer(
        sizeof( tone_state_t ),
        vk::BufferUsageFlagBits::eStorageBuffer|
        vk::BufferUsageFlagBits::eTransferDst|
        vk::BufferUsageFlagBits::eTransferSrc,
        VMA_MEMORY_USAGE_GPU_ONLY
      )
    );
    tone_staging.push_back(
      allocator->create_buffer(
        sizeof( tone_state_t ),
        vk::BufferUsageFlagBits::eTransferDst|
        vk::BufferUsageFlagBits::eTransferSrc,
        VMA_MEMORY_USAGE_CPU_TO_GPU
      )
    );
  }
  

  const auto [mix_ao_descriptor_set_layout,mix_ao_pipeline] = pipeline_cache->get_pipeline(
    CMAKE_CURRENT_BINARY_DIR "/mix_ao/mix_ao.comp.spv"
  );


  std::vector< std::shared_ptr< gct::image_view_t > > mixed_out;
  std::vector< std::shared_ptr< gct::image_view_t > > bloom_out;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    mixed_out.push_back(
      allocator->create_image(
        rgba32ici,
        VMA_MEMORY_USAGE_GPU_ONLY
      )->get_view( vk::ImageAspectFlagBits::eColor )
    );
    bloom_out.push_back(
      allocator->create_image(
        rgba32ici,
        VMA_MEMORY_USAGE_GPU_ONLY
      )->get_view( vk::ImageAspectFlagBits::eColor )
    );
  }
  {
    auto command_buffer = queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( mixed_out, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( bloom_out, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }

  std::vector< std::shared_ptr< gct::descriptor_set_t > > mix_ao_descriptor_set;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    mix_ao_descriptor_set.push_back(
      descriptor_pool->allocate(
        mix_ao_descriptor_set_layout
      )
    );
    mix_ao_descriptor_set.back()->update({
      { "gbuffer", gbuffer.get_image_view( i ) },
      { "diffuse_image", diffuse[ i ] },
      { "specular_image", specular[ i ] },
      { "dest_image", mixed_out[ i ] },
      { "bloom_image", bloom_out[ i ] },
      { "tone", tone[ i ] },
      { "dynamic_uniforms", dynamic_uniform[ i ] }
    });
  }

  const auto [bloom_hgauss_descriptor_set_layout,bloom_hgauss_pipeline] = pipeline_cache->get_pipeline(
    CMAKE_CURRENT_BINARY_DIR "/gauss/h12_32.comp.spv"
  );
  const auto [bloom_vgauss_descriptor_set_layout,bloom_vgauss_pipeline] = pipeline_cache->get_pipeline(
    CMAKE_CURRENT_BINARY_DIR "/gauss/v12_32.comp.spv"
  );

  std::vector< std::shared_ptr< gct::image_view_t > > bloom_gauss_temp;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    bloom_gauss_temp.push_back(
      allocator->create_image(
        rgba32ici,
        VMA_MEMORY_USAGE_GPU_ONLY
      )->get_view( vk::ImageAspectFlagBits::eColor )
    );
  }
  {
    auto command_buffer = queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( bloom_gauss_temp, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }
  std::vector< std::shared_ptr< gct::descriptor_set_t > > bloom_hgauss_descriptor_set;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    bloom_hgauss_descriptor_set.push_back(
      descriptor_pool->allocate(
        bloom_hgauss_descriptor_set_layout
      )
    );
    bloom_hgauss_descriptor_set.back()->update({
      { "src_image", bloom_out[ i ] },
      { "dest_image", bloom_gauss_temp[ i ] }
    });
  }
  std::vector< std::shared_ptr< gct::descriptor_set_t > > bloom_vgauss_descriptor_set;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    bloom_vgauss_descriptor_set.push_back(
      descriptor_pool->allocate(
        bloom_vgauss_descriptor_set_layout
      )
    );
    bloom_vgauss_descriptor_set.back()->update({
      { "src_image", bloom_gauss_temp[ i ] },
      { "dest_image", bloom_out[ i ] }
    });
  }

  const auto [tone_descriptor_set_layout,tone_pipeline] = pipeline_cache->get_pipeline(
    CMAKE_CURRENT_BINARY_DIR "/tone/tone.comp.spv"
  );

  std::vector< std::shared_ptr< gct::descriptor_set_t > > tone_descriptor_set;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    tone_descriptor_set.push_back(
      descriptor_pool->allocate(
        tone_descriptor_set_layout
      )
    );
    tone_descriptor_set.back()->update({
      { "src_image", mixed_out[ i ] },
      { "tone", tone[ i ] }
    });
  }

  const auto [gamma_descriptor_set_layout,gamma_pipeline] = pipeline_cache->get_pipeline(
    CMAKE_CURRENT_BINARY_DIR "/gamma/gamma.comp.spv"
  );
  {
    auto command_buffer = queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( swapchain_images, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }
  std::vector< std::shared_ptr< gct::image_view_t > > swapchain_image_views;
  for( const auto &image: swapchain_images ) {
    swapchain_image_views.push_back( image->get_view( vk::ImageAspectFlagBits::eColor ) );
  }

  std::vector< std::shared_ptr< gct::descriptor_set_t > > gamma_descriptor_set;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    gamma_descriptor_set.push_back(
      descriptor_pool->allocate(
        gamma_descriptor_set_layout
      )
    );
    gamma_descriptor_set.back()->update({
      { "src_image", mixed_out[ i ] },
      { "dest_image", swapchain_image_views[ i ]  },
      { "bloom_image", bloom_out[ i ] }
    });
  }

  gct::gltf::document_t doc;
  {
    auto rec = gcb->begin();
    doc = gct::gltf::load_gltf(
      model_filename,
      device,
      rec,
      allocator,
      descriptor_pool,
      {
        shadow_gbuffer.get_render_pass(),
        gbuffer.get_render_pass()
      },
      {
        CMAKE_CURRENT_BINARY_DIR "/shadow",
        CMAKE_CURRENT_BINARY_DIR "/geometry"
      },
      0,
      1,
      0,
      float( width ) / float( height ),
      false,
      {
        dynamic_descriptor_set_layout
      }
    );
  }
  gcb->execute_and_wait();

  auto center = ( doc.node.min + doc.node.max ) / 2.f;
  auto scale = std::abs( glm::length( doc.node.max - doc.node.min ) );

  auto const viewport =
    vk::Viewport()
      .setWidth( width )
      .setHeight( height )
      .setMinDepth( 0.0f )
      .setMaxDepth( 1.0f );
  vk::Rect2D const scissor( vk::Offset2D(0, 0), vk::Extent2D( width, height ) );
  auto const shadow_viewport =
    vk::Viewport()
      .setWidth( shadow_map_size )
      .setHeight( shadow_map_size )
      .setMinDepth( 0.0f )
      .setMaxDepth( 1.0f );
  vk::Rect2D const shadow_scissor( vk::Offset2D(0, 0), vk::Extent2D( shadow_map_size, shadow_map_size ) );


  const glm::mat4 projection = glm::perspective( 0.6981317007977318f, (float(width)/float(height)), std::min(0.1f*scale,0.5f), 150.f*scale );
  const float light_size = 0.3;

  gct::glfw_walk walk( center, scale, walk_state_filename );
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

  std::vector< std::shared_ptr< gct::mappable_buffer_t > > shadow_uniform;
  std::vector< std::shared_ptr< gct::descriptor_set_t > > shadow_descriptor_set;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    shadow_uniform.emplace_back(
      allocator->create_mappable_buffer(
        sizeof( gct::gltf::dynamic_uniforms_t ),
        vk::BufferUsageFlagBits::eUniformBuffer
      )
    );
    shadow_descriptor_set.push_back(
      descriptor_pool->allocate(
        dynamic_descriptor_set_layout
      )
    );
    shadow_descriptor_set.back()->update({
      { "dynamic_uniforms", shadow_uniform.back() },
      { "matrices", shadow_mat_storage[ i ] }
    });
  }
  
  gct::hysteresis< float > tone_scale(
    1.f, 10, 60
  );

  uint32_t current_frame = 0u;
  {
    ++walk;
    auto command_buffer = queue->get_command_pool()->allocate();
    unsigned int image_index = 0u;  
    {
      auto rec = command_buffer->begin();

      auto shadow_mat_pc = gct::cubemat_push_constant_t()
        .set_ioffset( glm::ivec4( 0, 0, 0, 0 ) )
        .set_offset( glm::vec4( walk.get_light_pos(), 1.f ) )
        .set_near( std::min( 0.1f*scale, 0.5f ) )
        .set_far( 2.f*scale );
      rec->pushConstants(
        **shadow_mat_pipeline->get_props().get_layout(),
        vk::ShaderStageFlagBits::eCompute,
        0u,
        sizeof( gct::cubemat_push_constant_t ),
        reinterpret_cast< void* >( &shadow_mat_pc )
      );
      rec.bind(
        shadow_mat_pipeline,
        { shadow_mat_descriptor_set[ image_index ] }
      );
      rec.dispatch_threads( 6, 1, 1 );

      rec.barrier(
        vk::AccessFlagBits::eShaderWrite,
        vk::AccessFlagBits::eShaderRead,
        vk::PipelineStageFlagBits::eComputeShader,
        vk::PipelineStageFlagBits::eGeometryShader,
        vk::DependencyFlagBits( 0 ),
        {
          shadow_mat_storage[ image_index ]
        },
        {}
      );

      {
        auto dynamic_data = gct::gltf::dynamic_uniforms_t()
          .set_eye_pos( glm::vec4( walk.get_camera_pos(), 1.0 ) )
          .set_light_pos( glm::vec4( walk.get_light_pos(), 1.0 ) )
          .set_light_energy( walk.get_light_energy() )
          .set_light_size( light_size )
          .set_ambient( ambient_level );
        rec.copy(
          dynamic_data,
          shadow_uniform[ image_index ]
        );
      }

      rec.barrier(
        vk::AccessFlagBits::eTransferWrite,
        vk::AccessFlagBits::eShaderRead,
        vk::PipelineStageFlagBits::eTransfer,
        vk::PipelineStageFlagBits::eVertexShader,
        vk::DependencyFlagBits( 0 ),
        {
          shadow_uniform[ image_index ]->get_buffer(),
        },
        {}
      );
      {
        auto render_pass_token = rec.begin_render_pass(
          shadow_gbuffer.get_render_pass_begin_info( image_index ),
          vk::SubpassContents::eInline
        );
        rec->setViewport( 0, 1, &shadow_viewport );
        rec->setScissor( 0, 1, &shadow_scissor );
        gct::gltf::draw_node(
          rec,
          doc.node,
          doc.mesh,
          doc.buffer,
          0u,
          {
            shadow_descriptor_set[ image_index ]
          }
        );
      }
      rec.convert_image(
        shadow_gbuffer.get_image( image_index ),
        vk::ImageLayout::eShaderReadOnlyOptimal
      );
      rec.dump_field(
        allocator,
        shadow_gbuffer.get_image( image_index ),
        "shadow0.png",
        0,
        0,
        0,
        0
      );
      rec.dump_field(
        allocator,
        shadow_gbuffer.get_image( image_index ),
        "shadow1.png",
        0,
        1,
        0,
        0
      );
      rec.dump_field(
        allocator,
        shadow_gbuffer.get_image( image_index ),
        "shadow2.png",
        0,
        2,
        0,
        0
      );
      rec.dump_field(
        allocator,
        shadow_gbuffer.get_image( image_index ),
        "shadow3.png",
        0,
        3,
        0,
        0
      );
      rec.dump_field(
        allocator,
        shadow_gbuffer.get_image( image_index ),
        "shadow4.png",
        0,
        4,
        0,
        0
      );
      rec.dump_field(
        allocator,
        shadow_gbuffer.get_image( image_index ),
        "shadow5.png",
        0,
        5,
        0,
        0
      );
    }
    command_buffer->execute_and_wait();
  }

  (*queue)->waitIdle();
}

