#include <iostream>
#include <unordered_set>
#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>
#define GLM_ENABLE_EXPERIMENTAL
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
#include <gct/compute.hpp>

struct tone_state_t {
  std::uint32_t max;
  float scale;
};

struct sh_coef_t {
  glm::vec4 coef[ 9 ];
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
    VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME,
    VK_KHR_MAINTENANCE1_EXTENSION_NAME,
    VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
  } );

  std::uint32_t width = 1920u*2;
  std::uint32_t height = 1080u*2;

 
  std::vector< gct::queue_requirement_t > queue_requirements{
    gct::queue_requirement_t{
      vk::QueueFlagBits::eGraphics,
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

  const auto dynamic_descriptor_set_layout = device->get_descriptor_set_layout(
    {
      CMAKE_CURRENT_BINARY_DIR "/geometry",
      CMAKE_CURRENT_BINARY_DIR "/shadow"
    },
    1u
  );
  
  auto cubemap_sampler = device->get_sampler(
    gct::get_basic_linear_sampler_create_info()
  );

  gct::cubemap_matrix_generator shadow_mat(
    allocator,
    descriptor_pool,
    pipeline_cache,
    CMAKE_CURRENT_BINARY_DIR "/shadow_mat/shadow_mat.comp.spv"
  );
  
  const unsigned int shadow_map_size = 1024u;
  gct::gbuffer shadow_gbuffer(
    gct::gbuffer_create_info()
      .set_allocator( allocator )
      .set_width( shadow_map_size )
      .set_height( shadow_map_size )
      .set_layer( 6 )
      .set_swapchain_image_count( 1 )
      .set_color_buffer_count( 1 )
      .set_flags( vk::ImageCreateFlagBits::eCubeCompatible )
      .set_format( vk::Format::eR32G32Sfloat ) 
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
      .set_clear_color( gct::color::web::white )
  );
  
  const auto shadow_uniform = allocator->create_mappable_buffer(
    sizeof( gct::gltf::dynamic_uniforms_t ),
    vk::BufferUsageFlagBits::eUniformBuffer
  );
  const auto shadow_descriptor_set = descriptor_pool->allocate(
    dynamic_descriptor_set_layout
  );
  shadow_descriptor_set->update({
    { "dynamic_uniforms", shadow_uniform },
    { "matrices", shadow_mat.get_buffer()[ 0 ] }
  });

  gct::cubemap_images2 shadow_cubemap_images( shadow_gbuffer.get_image_views() );

  gct::cubemap_matrix_generator cube_mat(
    allocator,
    descriptor_pool,
    pipeline_cache,
    CMAKE_CURRENT_BINARY_DIR "/cube_mat/cube_mat.comp.spv"
  );

  constexpr std::size_t cube_map_size = 32u;
  constexpr std::size_t gbuf_count = 8u;
  gct::gbuffer cube_gbuffer(
    gct::gbuffer_create_info()
      .set_allocator( allocator )
      .set_width( cube_map_size )
      .set_height( cube_map_size )
      .set_layer( 6 )
      .set_swapchain_image_count( 1 )
      .set_color_buffer_count( gbuf_count )
      .set_flags( vk::ImageCreateFlagBits::eCubeCompatible )
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
  );

  const auto cube_uniform = allocator->create_mappable_buffer(
    sizeof( gct::gltf::dynamic_uniforms_t ),
    vk::BufferUsageFlagBits::eUniformBuffer
  );
  const auto cube_descriptor_set = descriptor_pool->allocate(
    dynamic_descriptor_set_layout
  );
  cube_descriptor_set->update({
    { "dynamic_uniforms", cube_uniform },
    { "matrices", cube_mat.get_buffer()[ 0 ] },
    { "shadow", cubemap_sampler, shadow_cubemap_images.get_cube_image_views()[ 0 ], vk::ImageLayout::eShaderReadOnlyOptimal }
  });

  gct::cubemap_images2 cube_cubemap_images( cube_gbuffer.get_image_views() );

  const auto cube_image_create_info = gct::image_create_info_t()
    .set_basic( 
      gct::basic_2d_image( cube_map_size, cube_map_size )
        .setArrayLayers( 6 )
        .setFlags( vk::ImageCreateFlagBits::eCubeCompatible )
        .setUsage(
          vk::ImageUsageFlagBits::eTransferSrc|
          vk::ImageUsageFlagBits::eSampled|
          vk::ImageUsageFlagBits::eStorage
        )
    )
    .rebuild_chain();

  const auto diffuse_specular = allocator->create_image(
    cube_image_create_info,
    VMA_MEMORY_USAGE_GPU_ONLY
  )->get_view();
  
  {
    auto command_buffer = queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( diffuse_specular, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }
  
  const auto lighting = gct::compute(
    gct::compute_create_info()
      .set_allocator( allocator )
      .set_descriptor_pool( descriptor_pool )
      .set_pipeline_cache( pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/lighting/lighting.comp.spv" )
      .set_swapchain_image_count( 1 )
      .add_resource( { "gbuffer", cube_gbuffer.get_image_views(), vk::ImageLayout::eGeneral } )
      .add_resource( { "dest_image", diffuse_specular } )
      .add_resource( { "dynamic_uniforms", cube_uniform } )
  );

  gct::cubemap_images2 light_cubemap_image( { diffuse_specular } );

  unsigned int sphere_map_size = 320u;
  const auto sphere_image_create_info = gct::image_create_info_t()
    .set_basic( 
      gct::basic_2d_image( sphere_map_size, sphere_map_size )
        .setArrayLayers( 1 )
        .setUsage(
          vk::ImageUsageFlagBits::eTransferSrc|
          vk::ImageUsageFlagBits::eStorage
        )
    )
    .rebuild_chain();

  const auto sphere_image = allocator->create_image(
    sphere_image_create_info,
    VMA_MEMORY_USAGE_GPU_ONLY
  )->get_view();
  
  {
    auto command_buffer = queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( sphere_image, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }
  
  const auto sphere = gct::compute(
    gct::compute_create_info()
      .set_allocator( allocator )
      .set_descriptor_pool( descriptor_pool )
      .set_pipeline_cache( pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/cube_to_sphere/cube_to_sphere.comp.spv" )
      .set_swapchain_image_count( 1 )
      .add_resource( { "src_image", cubemap_sampler, light_cubemap_image.get_cube_image_views() } )
      .add_resource( { "dest_image", sphere_image } )
  );


  const auto sh_coef = allocator->create_mappable_buffer(
    sizeof( sh_coef_t ),
    vk::BufferUsageFlagBits::eStorageBuffer
  );

  const auto gensh = gct::compute(
    gct::compute_create_info()
      .set_allocator( allocator )
      .set_descriptor_pool( descriptor_pool )
      .set_pipeline_cache( pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/gensh/gensh.comp.spv" )
      .set_swapchain_image_count( 1 )
      .add_resource( { "src_image", cubemap_sampler, light_cubemap_image.get_cube_image_views() } )
      .add_resource( { "shcoef", sh_coef } )
  );

  const auto sh_sphere_image = allocator->create_image(
    sphere_image_create_info,
    VMA_MEMORY_USAGE_GPU_ONLY
  )->get_view();
  
  {
    auto command_buffer = queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( sh_sphere_image, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }
  
  const auto sh_sphere = gct::compute(
    gct::compute_create_info()
      .set_allocator( allocator )
      .set_descriptor_pool( descriptor_pool )
      .set_pipeline_cache( pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/sh_to_sphere/sh_to_sphere.comp.spv" )
      .set_swapchain_image_count( 1 )
      .add_resource( { "shcoef", sh_coef } )
      .add_resource( { "dest_image", sh_sphere_image } )
  );

  gct::gltf::document_t doc;
  {
    auto gcb = queue->get_command_pool()->allocate();
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
          cube_gbuffer.get_render_pass()
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
  }

  auto center = ( doc.node.min + doc.node.max ) / 2.f;
  auto scale = std::abs( glm::length( doc.node.max - doc.node.min ) );

  const glm::mat4 projection = glm::perspective( 0.6981317007977318f, (float(width)/float(height)), std::min(0.1f*scale,0.5f), scale );
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

  gct::hysteresis< float > tone_scale(
    1.f, 10, 60
  );

  uint32_t current_frame = 0u;
  {
    gct::blocking_timer frame_rate;
    ++walk;
    auto image_index = 0;// swapchain->acquire_next_image( sync.image_acquired );

    auto command_buffer = queue->get_command_pool()->allocate();
    {
      auto rec = command_buffer->begin();

      shadow_mat( rec, walk.get_light_pos(), std::min( 0.1f*scale, 0.5f ), 2.f*scale );
      cube_mat( rec, walk.get_camera_pos(), std::min( 0.1f*scale, 0.5f ), 2.f*scale );

      {
        auto dynamic_data = gct::gltf::dynamic_uniforms_t()
          .set_eye_pos( glm::vec4( walk.get_camera_pos(), 1.0 ) )
          .set_light_pos( glm::vec4( walk.get_light_pos(), 1.0 ) )
          .set_light_energy( walk.get_light_energy() )
          .set_light_size( light_size )
          .set_ambient( ambient_level );
        rec.copy(
          dynamic_data,
          shadow_uniform
        );
      }

      {
        auto dynamic_data = gct::gltf::dynamic_uniforms_t()
          .set_eye_pos( glm::vec4( walk.get_camera_pos(), 1.0 ) )
          .set_light_pos( glm::vec4( walk.get_light_pos(), 1.0 ) )
          .set_light_energy( walk.get_light_energy() )
          .set_light_size( light_size )
          .set_ambient( ambient_level );
        rec.copy(
          dynamic_data,
          cube_uniform
        );
      }

      rec.transfer_to_graphics_barrier(
        {
          shadow_uniform->get_buffer(),
          cube_uniform->get_buffer()
        },
        {}
      );

      {
        auto render_pass_token = rec.begin_render_pass(
          shadow_gbuffer.get_render_pass_begin_info( image_index ),
          vk::SubpassContents::eInline
        );
        rec->setViewport( 0, 1, &shadow_gbuffer.get_viewport() );
        rec->setScissor( 0, 1, &shadow_gbuffer.get_scissor() );
        gct::gltf::draw_node(
          rec,
          doc.node,
          doc.mesh,
          doc.buffer,
          0u,
          {
            shadow_descriptor_set
          }
        );
      }
      
      rec.convert_image(
        shadow_gbuffer.get_image( image_index ),
        vk::ImageLayout::eShaderReadOnlyOptimal
      );

      {
        auto render_pass_token = rec.begin_render_pass(
          cube_gbuffer.get_render_pass_begin_info( image_index ),
          vk::SubpassContents::eInline
        );
        rec->setViewport( 0, 1, &cube_gbuffer.get_viewport() );
        rec->setScissor( 0, 1, &cube_gbuffer.get_scissor() );
        gct::gltf::draw_node(
          rec,
          doc.node,
          doc.mesh,
          doc.buffer,
          1u,
          {
            cube_descriptor_set
          }
        );
      }
      rec.convert_image(
        cube_gbuffer.get_image( image_index ),
        vk::ImageLayout::eGeneral
      );
      
      lighting( rec, image_index, cube_map_size, cube_map_size, 6u );

      rec.compute_barrier(
        {},
        {
          diffuse_specular->get_factory(),
        }
      );

      sphere( rec, image_index, sphere_map_size, sphere_map_size, 1u );

      gensh( rec, image_index, 32, 32, 1u );

      rec.compute_barrier(
        {
          sh_coef->get_buffer()
        },
        {}
      );

      sh_sphere( rec, image_index, sphere_map_size, sphere_map_size, 1u );

      rec.compute_to_transfer_barrier(
        {
          sh_coef->get_buffer()
        },
        {
          sphere_image->get_factory(),
          sh_sphere_image->get_factory(),
        }
      );
      rec.dump_buffer_as< sh_coef_t >( sh_coef ).then(
        []( sh_coef_t &&v ) {
          for( unsigned int i = 0u; i != 9u; ++i )
            std::cout << glm::to_string( v.coef[ i ] ) << std::endl;
        }
      );
      /*for( unsigned int i = 0u; i != 6u; ++i ) {
        rec.dump_image(
          allocator,
          diffuse_specular->get_factory(),
          "diffuse_specular" + std::to_string( i ) + ".png",
          0,
          i,
          0
        );
      }*/
      rec.dump_image(
        allocator,
        sphere_image->get_factory(),
        "diffuse_specular.png",
        0,
        0,
        0
      );
      rec.dump_image(
        allocator,
        sh_sphere_image->get_factory(),
        "sh_diffuse_specular.png",
        0,
        0,
        0
      );
    }
    command_buffer->execute(
      gct::submit_info_t()
    );
    command_buffer->wait_for_executed();
  }

  (*queue)->waitIdle();
  walk.save( walk_state_filename );
}

