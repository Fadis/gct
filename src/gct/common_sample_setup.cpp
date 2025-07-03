#include <boost/program_options/value_semantic.hpp>
#include <cstdlib>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/spirit/include/qi.hpp>
#include <gct/glfw.hpp>
#include <gct/instance.hpp>
#include <gct/common_sample_setup.hpp>
#include <gct/get_extensions.hpp>
#include <gct/instance.hpp>
#include <gct/glfw.hpp>
#include <gct/queue.hpp>
#include <gct/device.hpp>
#include <gct/allocator.hpp>
#include <gct/device_create_info.hpp>
#include <gct/swapchain.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/image.hpp>
#include <gct/image_view.hpp>
#include <gct/vulkanhpp.hpp>
#include <gct/io_context.hpp>
#include <vulkan/vulkan_enums.hpp>

namespace gct {
common_sample_setup::common_sample_setup(
  int argc,
  const char *argv[],
  const std::vector< const char* > &device_extensions,
  const descriptor_pool_create_info_t &dpci,
  bool enable_glfw,
  bool enable_device_address,
  bool enable_gltf
) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  desc.add_options()
    ( "help,h", "show this message" )
    ( "debug,d", po::bool_switch(), "enable debug log" )
    ( "validation,v", po::bool_switch(), "enable validation layer" );
  if( enable_glfw ) {
    desc.add_options()
      ( "size,s", po::value< std::string >()->default_value("native"), "window size" )
      ( "fullscreen,f", po::bool_switch(), "fullscreen" )
      ( "walk,w", po::value< std::string >()->default_value(".walk"), "walk state filename" )
      ( "model,m", po::value< std::vector< std::string > >()->multitoken(), "glTF filename" )
      ( "ambient,a", po::value< float >()->default_value( 0.1 ), "ambient light level" )
      ( "light,l", po::value< unsigned int >()->default_value( 50u ), "max light count" )
      ( "geometry,g", po::bool_switch(), "force running geometry processing every frame" );
  }
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) || ( enable_glfw && enable_gltf && ( !vm.count( "model" ) || vm[ "model" ].as< std::vector< std::string > >().empty() ) ) ) {
    std::cout << desc << std::endl;
    exit( 0 );
  }
  
  std::vector< const char* > iext{};
  if( enable_glfw ) {
    walk_state_filename = vm[ "walk" ].as< std::string >();
    if( enable_gltf ) {
      model_filename = vm[ "model" ].as< std::vector< std::string > >()[ 0 ];
      model_filename_list = vm[ "model" ].as< std::vector< std::string > >();
    }
    ambient_level = std::min( std::max( vm[ "ambient" ].as< float >(), 0.f ), 1.f );
    glfw::get();
    uint32_t iext_count = 0u;
    auto exts = glfwGetRequiredInstanceExtensions( &iext_count );
    for( uint32_t i = 0u; i != iext_count; ++i )
      iext.push_back( exts[ i ] );
  }
  else {
    vulkanhpp::init();
    thread_pool::init();
  }
  iext.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
  const auto ilayers = get_instance_layers();
  const auto iexts = get_instance_extensions( std::vector< const char* >() );
  auto ici = instance_create_info_t()
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
#ifdef VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME
    ici.add_extension( VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME );
    ici.add_validation_feature_enable( vk::ValidationFeatureEnableEXT::eDebugPrintf );
#endif
  }
  instance.reset( new instance_t( ici ) );
  if( vm[ "validation" ].as< bool >() ) {
    instance->abort_on_validation_failure( vm[ "debug" ].as< bool >() );
  }

  auto groups = instance->get_physical_devices( {} );
  auto selected = groups[ 0 ].with_extensions( device_extensions );

  if( enable_glfw ) {
    const bool fullscreen = vm[ "fullscreen" ].as< bool >();
    const std::string window_size = vm[ "size" ].as< std::string >();
    if( window_size != "native" ) { 
      boost::fusion::vector< unsigned int, unsigned int > parsed_window_size;
      {
        auto iter = window_size.begin();
        const auto end = window_size.end();
        namespace qi = boost::spirit::qi;
        if( !qi::parse( iter, end, qi::uint_ >> 'x' >> qi::uint_, parsed_window_size ) ) {
          std::cerr << "Invalid window size: " << window_size << std::endl;
          exit( 1 );
        }
      }
      width = boost::fusion::at_c< 0 >( parsed_window_size );
      height = boost::fusion::at_c< 1 >( parsed_window_size );
    }
    else {
      width = 1920;
      height = 1080;
    }
    light_count = vm[ "light" ].as< unsigned int >();
    force_geometry = vm[ "geometry" ].as< bool >();
    window.reset( new gct::glfw_window( width, height, "window title", fullscreen ) );
    gct::glfw::get().poll();
    surface = window->get_surface( *groups[ 0 ].devices[ 0 ] );
 
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
    device = selected.create_device(
      queue_requirements,
      gct::device_create_info_t()
    );
    queue = device->get_queue( 0u );
 
    swapchain = device->get_swapchain( surface );
    swapchain_images = swapchain->get_images();
    swapchain_image_views = gct::get_views( swapchain_images );
  }
  else {
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
    device = selected.create_device(
      queue_requirements,
      gct::device_create_info_t()
    );
    queue = device->get_queue( 0u );
  }

  descriptor_pool = device->get_descriptor_pool( dpci );

  pipeline_cache = device->get_pipeline_cache();

  VmaAllocatorCreateInfo allocator_create_info{};
  if( enable_device_address ) {
    allocator_create_info.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
  }
  allocator = device->get_allocator(
    allocator_create_info
  );
  allocator_set
    .set_allocator( allocator )
    .set_pipeline_cache( pipeline_cache )
    .set_descriptor_pool( descriptor_pool );
}

}

