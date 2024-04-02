#include <vector>
#include <string>
#include <iostream>
#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>
#include <gct/device_create_info.hpp>
#include <gct/device.hpp>
#include <gct/instance.hpp>
#include <gct/physical_device.hpp>
#include <gct/shader_module.hpp>
#include <gct/shader_module_reflection.hpp>
#include <gct/descriptor_pool_create_info.hpp>
#include <gct/spv2json.hpp>

int main( int argc, const char *argv[] ) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  desc.add_options()
    ( "help,h", "show this message" )
    ( "input,i", po::value< std::string >(), "input file" )
    ( "validation,v", po::bool_switch(), "enable validation layer" );
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) || !vm.count( "input" ) ) {
    std::cout << desc << std::endl;
    exit( 0 );
  }
  std::vector< const char* > iext{};
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
    new gct::instance_t( std::move( ici ) )
  );
  auto groups = instance->get_physical_devices( {} );
  auto selected = groups[ 0 ].with_extensions( {} );
  auto device = selected.create_device(
    std::vector< gct::queue_requirement_t >{
      gct::queue_requirement_t{
        vk::QueueFlagBits::eCompute,
        0u,
        vk::Extent3D(),
#ifdef VK_EXT_GLOBAL_PRIORITY_EXTENSION_NAME
        vk::QueueGlobalPriorityEXT(),
#endif
        {},
        vk::CommandPoolCreateFlagBits::eResetCommandBuffer
      }
    },
    gct::device_create_info_t()
  );
  const auto shader_module = device->get_shader_module( vm[ "input" ].as< std::string >() );
  std::cout << nlohmann::json( *shader_module->get_props().get_reflection() ).dump(2) << std::endl;
}

