#include <iostream>
#include <unordered_set>
#include <boost/program_options.hpp>
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

struct shadow_matrices_t {
  glm::mat4 camera_projection[ 6 ];
  glm::mat4 camera[ 6 ];
};
struct cube_matrices_t {
  glm::mat4 camera_projection[ 6 ];
  glm::mat4 camera[ 6 ];
};

struct tone_state_t {
  std::uint32_t max;
  float scale;
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
  std::cout << nlohmann::json( groups[ 0 ] ).dump( 2 ) << std::endl;

  std::uint32_t width = 320;
  std::uint32_t height = 320;

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
  const auto device = selected.create_device(
    queue_requirements,
    gct::device_create_info_t()
  );
  const auto queue = device->get_queue( 0u );
  const auto gcb = queue->get_command_pool()->allocate();

  const auto descriptor_pool = device->get_descriptor_pool(
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

  const auto pipeline_cache = device->get_pipeline_cache();

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

  const auto [shadow_mat_descriptor_set_layout,shadow_mat_pipeline] = pipeline_cache->get_pipeline(
    CMAKE_CURRENT_BINARY_DIR "/shadow_mat/shadow_mat.comp.spv"
  );

  const auto shadow_mat_storage = allocator->create_buffer(
    sizeof( shadow_matrices_t ),
    vk::BufferUsageFlagBits::eStorageBuffer,
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  const auto shadow_mat_descriptor_set = descriptor_pool->allocate(
    shadow_mat_descriptor_set_layout
  );
  shadow_mat_descriptor_set->update({
    { "matrices", shadow_mat_storage }
  });
  
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
    { "matrices", shadow_mat_storage }
  });

  const auto cubemap_sampler = device->get_sampler(
    gct::get_basic_linear_sampler_create_info()
  );
  gct::cubemap_images2 shadow_cubemap_images( shadow_gbuffer.get_image_views() );
  /*const auto [cube_mat_descriptor_set_layout,cube_mat_pipeline] = pipeline_cache->get_pipeline(
    CMAKE_CURRENT_BINARY_DIR "/cubemat/cubemat.comp.spv"
  );
  const auto cube_mat_storage = allocator->create_buffer(
    sizeof( cube_matrices_t ),
    vk::BufferUsageFlagBits::eStorageBuffer,
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  const auto cube_mat_descriptor_set = descriptor_pool->allocate(
    cube_mat_descriptor_set_layout
  );
  cube_mat_descriptor_set->update({
    { "matrices", cube_mat_storage }
  });
  const auto cube_uniform = allocator->create_mappable_buffer(
    sizeof( gct::gltf::dynamic_uniforms_t ),
    vk::BufferUsageFlagBits::eUniformBuffer
  );
  const auto cube_descriptor_set = descriptor_pool->allocate(
    dynamic_descriptor_set_layout
  );
  cube_descriptor_set->update({
    { "dynamic_uniforms", cube_uniform },
    { "matrices", cube_mat_storage },
    { "shadow", cubemap_sampler, shadow_cubemap_images.get_cube_image_views()[ 0 ], vk::ImageLayout::eShaderReadOnlyOptimal }
  });
  const unsigned int sh_layer_count = 6u;
  const unsigned int cube_gbuffer_attrs = 8u;
  const unsigned int cube_gbuffer_image_size = 320u;
  gct::gbuffer cube_gbuffer(
    gct::gbuffer_create_info()
      .set_allocator( allocator )
      .set_width( cube_gbuffer_image_size )
      .set_height( cube_gbuffer_image_size )
      .set_layer( sh_layer_count )
      .set_swapchain_image_count( 1 )
      .set_color_buffer_count( cube_gbuffer_attrs )
      .set_format( vk::Format::eR32G32B32A32Sfloat ) 
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
  );

  const auto dynamic_uniform = allocator->create_mappable_buffer(
      sizeof( gct::gltf::dynamic_uniforms_t ),
      vk::BufferUsageFlagBits::eUniformBuffer
    );

  const auto [light_descriptor_set_layout,light_pipeline] = pipeline_cache->get_pipeline(
    CMAKE_CURRENT_BINARY_DIR "/lighting/lighting.comp.spv"
  );
  
  const auto diffuse = allocator->create_image(
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( cube_gbuffer_image_size, cube_gbuffer_image_size )
          .setArrayLayers( 6 )
          .setUsage(
            vk::ImageUsageFlagBits::eTransferSrc|
            vk::ImageUsageFlagBits::eStorage
          )
      )
      .rebuild_chain(),
    VMA_MEMORY_USAGE_GPU_ONLY
  )->get_view();
  const auto specular = allocator->create_image(
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( cube_gbuffer_image_size, cube_gbuffer_image_size )
          .setArrayLayers( 6 )
          .setUsage(
            vk::ImageUsageFlagBits::eTransferSrc|
            vk::ImageUsageFlagBits::eStorage
          )
      )
      .rebuild_chain(),
    VMA_MEMORY_USAGE_GPU_ONLY
  )->get_view();

  {
    auto command_buffer = queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( diffuse, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( specular, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }
  const auto light_descriptor_set = descriptor_pool->allocate(
    light_descriptor_set_layout
  );
  light_descriptor_set->update({
    { "gbuffer", cube_gbuffer.get_image_view( 0 ), vk::ImageLayout::eGeneral },
    { "diffuse_image", diffuse },
    { "specular_image", specular },
    { "dynamic_uniforms", dynamic_uniform }
  });*/

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
        shadow_gbuffer.get_render_pass()/*,
        cube_gbuffer.get_render_pass()*/
      },
      {
        CMAKE_CURRENT_BINARY_DIR "/shadow"/*,
        CMAKE_CURRENT_BINARY_DIR "/geometry"*/
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

  gct::cubemap_matrix cubemap_matrix( 
    glm::vec3{ 0.f, 0.f, 0.f },
    std::min( 0.1f*scale, 0.5f ),
    2.f*scale
  );

  auto const shadow_viewport =
    vk::Viewport()
      .setWidth( shadow_map_size )
      .setHeight( shadow_map_size )
      .setMinDepth( 0.0f )
      .setMaxDepth( 1.0f );
  vk::Rect2D const shadow_scissor( vk::Offset2D(0, 0), vk::Extent2D( shadow_map_size, shadow_map_size ) );

  /*auto const cube_viewport =
    vk::Viewport()
      .setWidth( cube_gbuffer_image_size )
      .setHeight( cube_gbuffer_image_size )
      .setMinDepth( 0.0f )
      .setMaxDepth( 1.0f );
  vk::Rect2D const cube_scissor( vk::Offset2D(0, 0), vk::Extent2D( cube_gbuffer_image_size, cube_gbuffer_image_size ) );
*/
  gct::glfw_walk walk( center, scale, walk_state_filename );
  const auto point_lights = gct::gltf::get_point_lights(
    doc.node,
    doc.point_light
  );
  if( !point_lights.empty() ) {
    walk.set_light_energy( point_lights[ 0 ].intensity / ( 4 * M_PI ) / 100 );
    walk.set_light_pos( point_lights[ 0 ].location );
  }

  const float light_size = 0.3;

  {
    auto command_buffer = queue->get_command_pool()->allocate();
    {
      auto rec = command_buffer->begin();
      {
        auto cube_mat_pc = gct::cubemat_push_constant_t()
          .set_ioffset( glm::ivec4( 0, 0, 0, 0 ) )
          .set_offset( glm::vec4( walk.get_light_pos(), 1.f ) )
          .set_near( std::min( 0.1f*scale, 0.5f ) )
          .set_far( 2.f*scale );
        rec->pushConstants(
          **shadow_mat_pipeline->get_props().get_layout(),
          vk::ShaderStageFlagBits::eCompute,
          0u,
          sizeof( gct::cubemat_push_constant_t ),
          reinterpret_cast< void* >( &cube_mat_pc )
        );
      }
      rec.bind(
        shadow_mat_pipeline,
        { shadow_mat_descriptor_set }
      );
      rec.dispatch_threads( 6, 1, 1 );
      rec.barrier(
        vk::AccessFlagBits::eShaderWrite,
        vk::AccessFlagBits::eShaderRead,
        vk::PipelineStageFlagBits::eComputeShader,
        vk::PipelineStageFlagBits::eGeometryShader,
        vk::DependencyFlagBits( 0 ),
        {
          shadow_mat_storage
        },
        {}
      );
 
      {
        auto dynamic_data = gct::gltf::dynamic_uniforms_t()
          .set_eye_pos( glm::vec4( walk.get_light_pos(), 1.f ) )
          .set_light_pos( glm::vec4( walk.get_light_pos(), 1.f ) )
          .set_light_energy( walk.get_light_energy() )
          .set_light_size( light_size )
          .set_ambient( ambient_level );
        rec.copy(
          dynamic_data,
          shadow_uniform
        );
      }
 
      rec.barrier(
        vk::AccessFlagBits::eTransferWrite,
        vk::AccessFlagBits::eShaderRead,
        vk::PipelineStageFlagBits::eTransfer,
        vk::PipelineStageFlagBits::eVertexShader,
        vk::DependencyFlagBits( 0 ),
        {
          shadow_uniform->get_buffer()
        },
        {}
      );
      {
        auto render_pass_token = rec.begin_render_pass(
          shadow_gbuffer.get_render_pass_begin_info( 0 ),
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
            shadow_descriptor_set
          }
        );
      }
      rec.convert_image(
        shadow_gbuffer.get_image( 0 ),
        vk::ImageLayout::eShaderReadOnlyOptimal
      );
      rec.barrier(
        vk::AccessFlagBits::eColorAttachmentWrite,
        vk::AccessFlagBits::eShaderRead,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::PipelineStageFlagBits::eFragmentShader,
        vk::DependencyFlagBits( 0 ),
        {},
        {
          shadow_gbuffer.get_image( 0 )
        }
      );

      /*{ 
        auto cube_mat_pc = gct::cubemat_push_constant_t()
          .set_ioffset( glm::ivec4( 0, 0, 0, 0 ) )
          .set_offset( glm::vec4( walk.get_camera_pos(), 1.f ) ) //////
          .set_near( std::min( 0.1f*scale, 0.5f ) )
          .set_far( 2.f*scale );
        rec->pushConstants(
          **cube_mat_pipeline->get_props().get_layout(),
          vk::ShaderStageFlagBits::eCompute,
          0u,
          sizeof( gct::cubemat_push_constant_t ),
          reinterpret_cast< void* >( &cube_mat_pc )
        );
      }

      rec.bind(
        cube_mat_pipeline,
        { cube_mat_descriptor_set }
      );
      rec.dispatch_threads( 6, 1, 1 );
      rec.barrier(
        vk::AccessFlagBits::eShaderWrite,
        vk::AccessFlagBits::eShaderRead,
        vk::PipelineStageFlagBits::eComputeShader,
        vk::PipelineStageFlagBits::eGeometryShader,
        vk::DependencyFlagBits( 0 ),
        {
          cube_mat_storage
        },
        {}
      );
 
      {
        auto dynamic_data = gct::gltf::dynamic_uniforms_t()
          .set_eye_pos( glm::vec4( walk.get_camera_pos(), 1.f ) )
          .set_light_pos( glm::vec4( walk.get_light_pos(), 1.f ) )
          .set_light_energy( walk.get_light_energy() )
          .set_light_size( light_size )
          .set_ambient( ambient_level );
        rec.copy(
          dynamic_data,
          cube_uniform
        );
      }
 
      rec.barrier(
        vk::AccessFlagBits::eTransferWrite,
        vk::AccessFlagBits::eShaderRead,
        vk::PipelineStageFlagBits::eTransfer,
        vk::PipelineStageFlagBits::eVertexShader,
        vk::DependencyFlagBits( 0 ),
        {
          cube_uniform->get_buffer()
        },
        {}
      );
      {
        auto render_pass_token = rec.begin_render_pass(
          cube_gbuffer.get_render_pass_begin_info( 0 ),
          vk::SubpassContents::eInline
        );
        rec->setViewport( 0, 1, &cube_viewport );
        rec->setScissor( 0, 1, &cube_scissor );
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
        cube_gbuffer.get_image( 0 ),
        vk::ImageLayout::eGeneral
      );
      
      rec.bind(
        light_pipeline,
        { light_descriptor_set }
      );
      rec.dispatch_threads( cube_gbuffer_image_size, cube_gbuffer_image_size, sh_layer_count );

      rec.compute_barrier(
        {},
        {
          diffuse->get_factory(),
          specular->get_factory()
        }
      );
      rec.convert_image(
        diffuse->get_factory(),
        vk::ImageLayout::eTransferSrcOptimal
      );
      rec.convert_image(
        specular->get_factory(),
        vk::ImageLayout::eTransferSrcOptimal
      );
      rec.dump_image(
        allocator,
        diffuse->get_factory(),
        "diffuse0.exr",
        0,
        0,
        0
      );
      rec.dump_image(
        allocator,
        diffuse->get_factory(),
        "diffuse1.exr",
        0,
        1,
        0
      );
      rec.dump_image(
        allocator,
        diffuse->get_factory(),
        "diffuse2.exr",
        0,
        2,
        0
      );
      rec.dump_image(
        allocator,
        diffuse->get_factory(),
        "diffuse3.exr",
        0,
        3,
        0
      );
      rec.dump_image(
        allocator,
        diffuse->get_factory(),
        "diffuse4.exr",
        0,
        4,
        0
      );
      rec.dump_image(
        allocator,
        diffuse->get_factory(),
        "diffuse5.exr",
        0,
        5,
        0
      );
      rec.dump_image(
        allocator,
        specular->get_factory(),
        "specular0.exr",
        0,
        0,
        0
      );
      rec.dump_image(
        allocator,
        specular->get_factory(),
        "specular1.exr",
        0,
        1,
        0
      );
      rec.dump_image(
        allocator,
        specular->get_factory(),
        "specular2.exr",
        0,
        2,
        0
      );
      rec.dump_image(
        allocator,
        specular->get_factory(),
        "specular3.exr",
        0,
        3,
        0
      );
      rec.dump_image(
        allocator,
        specular->get_factory(),
        "specular4.exr",
        0,
        4,
        0
      );
      rec.dump_image(
        allocator,
        specular->get_factory(),
        "specular5.exr",
        0,
        5,
        0
      );
      rec.dump_image(
        allocator,
        cube_gbuffer.get_image( 0 ),
        "albedo0.png",
        0,
        6,
        0
      );
      rec.dump_image(
        allocator,
        cube_gbuffer.get_image( 0 ),
        "albedo1.png",
        0,
        7,
        0
      );
      rec.dump_image(
        allocator,
        cube_gbuffer.get_image( 0 ),
        "albedo2.png",
        0,
        8,
        0
      );
      rec.dump_image(
        allocator,
        cube_gbuffer.get_image( 0 ),
        "albedo3.png",
        0,
        9,
        0
      );
      rec.dump_image(
        allocator,
        cube_gbuffer.get_image( 0 ),
        "albedo4.png",
        0,
        10,
        0
      );
      rec.dump_image(
        allocator,
        cube_gbuffer.get_image( 0 ),
        "albedo5.png",
        0,
        11,
        0
      );*/
      rec.convert_image(
        shadow_gbuffer.get_image( 0 ),
        vk::ImageLayout::eTransferSrcOptimal
      );
      rec.dump_field(
        allocator,
        shadow_gbuffer.get_image( 0 ),
        "shadow0.png",
        0,
        0,
        0,
        0
      );
      rec.dump_field(
        allocator,
        shadow_gbuffer.get_image( 0 ),
        "shadow1.png",
        0,
        1,
        0,
        0
      );
      rec.dump_field(
        allocator,
        shadow_gbuffer.get_image( 0 ),
        "shadow2.png",
        0,
        2,
        0,
        0
      );
      rec.dump_field(
        allocator,
        shadow_gbuffer.get_image( 0 ),
        "shadow3.png",
        0,
        3,
        0,
        0
      );
      rec.dump_field(
        allocator,
        shadow_gbuffer.get_image( 0 ),
        "shadow4.png",
        0,
        4,
        0,
        0
      );
      rec.dump_field(
        allocator,
        shadow_gbuffer.get_image( 1 ),
        "shadow5.png",
        0,
        5,
        0,
        0
      );
    }
    command_buffer->execute_and_wait();
  }
/*
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
      auto mapped = tone_staging[ image_index ]->map< tone_state_t >();
      tone_scale.set( mapped.begin()->max / 65536.f );
      mapped.begin()->max = 0u;
      mapped.begin()->scale = tone_scale.get();
    }

    {
      auto rec = sync.command_buffer->begin();
      cubemap_matrix.move_center( walk.get_light_pos() );

      for( unsigned int i = 0u; i != 6u; ++i ) {
        auto dynamic_data = gct::gltf::dynamic_uniforms_t()
          .set_projection_matrix( cubemap_matrix.get_projection_matrix() )
          .set_camera_matrix( cubemap_matrix.get_view_matrix( i ) )
          .set_eye_pos( glm::vec4( walk.get_camera_pos(), 1.0 ) )
          .set_light_pos( glm::vec4( walk.get_light_pos(), 1.0 ) )
          .set_light_energy( walk.get_light_energy() )
          .set_light_size( light_size )
          .set_ambient( ambient_level );
        rec.copy(
          dynamic_data,
          staging_cube_uniform[ i + image_index * 6u ],
          cube_uniform[ i + image_index * 6u ]
        );
        rec.barrier(
          vk::AccessFlagBits::eTransferRead,
          vk::AccessFlagBits::eShaderRead,
          vk::PipelineStageFlagBits::eTransfer,
          vk::PipelineStageFlagBits::eVertexShader,
          vk::DependencyFlagBits( 0 ),
          { cube_uniform[ i + image_index * 6u ] },
          {}
        );
        auto render_pass_token = rec.begin_render_pass(
          cubemap_images[ image_index ].get_render_pass_begin_info( i ),
          vk::SubpassContents::eInline
        );
        rec->setViewport( 0, 1, &cube_viewport );
        rec->setScissor( 0, 1, &cube_scissor );
        gct::gltf::draw_node(
          rec,
          doc.node,
          doc.mesh,
          doc.buffer,
          0u,
          {
            cube_descriptor_set[ i + image_index * 6u ]
          }
        );
      }
      rec.barrier(
        vk::AccessFlagBits::eColorAttachmentWrite,
        vk::AccessFlagBits::eShaderRead,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::PipelineStageFlagBits::eFragmentShader,
        vk::DependencyFlagBits( 0 ),
        {},
        { cubemap_images[ image_index ].get_image( 0 ) }
      );

      rec.copy(
        tone_staging[ image_index ],
        tone[ image_index ]
      );
      auto dynamic_data = gct::gltf::dynamic_uniforms_t()
        .set_projection_matrix( projection )
        .set_camera_matrix( walk.get_lookat() )
        .set_eye_pos( glm::vec4( walk.get_camera_pos(), 1.0 ) )
        .set_light_pos( glm::vec4( walk.get_light_pos(), 1.0 ) )
        .set_light_energy( walk.get_light_energy() )
        .set_light_size( light_size )
        .set_ambient( ambient_level );
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
      {
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
          1u,
          {
            dynamic_descriptor_set[ image_index ]
          }
        );
      }
      rec.convert_image(
        gbuffer.get_image( image_index ),
        vk::ImageLayout::eGeneral
      );
      
      rec.bind(
        light_pipeline,
        { light_descriptor_set[ image_index ] }
      );
      rec.dispatch_threads( width, height, 1 );

      rec.compute_barrier(
        {},
        {
          diffuse[ image_index ]->get_factory(),
          specular[ image_index ]->get_factory()
        }
      );
      rec.bind(
        mix_ao_pipeline,
        { mix_ao_descriptor_set[ image_index ] }
      );
      rec.dispatch_threads( width, height, 1 );
      rec.compute_barrier(
        {},
        {
          mixed_out[ image_index ]->get_factory(),
          bloom_out[ image_index ]->get_factory()
        }
      );
      rec.bind(
        bloom_hgauss_pipeline,
        { bloom_hgauss_descriptor_set[ image_index ] }
      );
      rec.dispatch_threads( width, height, 1 );
      rec.compute_barrier(
        {},
        { bloom_gauss_temp[ image_index ]->get_factory() }
      );
      rec.bind(
        bloom_vgauss_pipeline,
        { bloom_vgauss_descriptor_set[ image_index ] }
      );
      rec.dispatch_threads( width, height, 1 );
      rec.compute_barrier(
        {},
        { bloom_out[ image_index ]->get_factory() }
      );
      rec.transfer_to_compute_barrier(
        { tone[ image_index ] },
        {}
      );
      rec.bind(
        tone_pipeline,
        { tone_descriptor_set[ image_index ] }
      );
      rec.dispatch_threads( width, height, 1 );
      rec.compute_to_transfer_barrier(
        { tone[ image_index ] },
        {}
      );
      rec.copy(
        tone[ image_index ],
        tone_staging[ image_index ]
      );
      rec.convert_image(
        swapchain_images[ image_index ],
        vk::ImageLayout::eGeneral
      );
      rec.bind(
        gamma_pipeline,
        { gamma_descriptor_set[ image_index ] }
      );
      rec.dispatch_threads( width, height, 1 );
      rec.convert_image(
        swapchain_images[ image_index ],
        vk::ImageLayout::ePresentSrcKHR
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
  walk.save( walk_state_filename );
  */
}

