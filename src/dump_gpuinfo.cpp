#include <iostream>
#include <nlohmann/json.hpp>
#include <gct/instance.hpp>
#include <gct/physical_device.hpp>
#include <gct/remove_chain.hpp>

int main() {
  std::shared_ptr< gct::instance_t > instance(
    new gct::instance_t(
      gct::instance_create_info_t()
        .set_application_info(
          vk::ApplicationInfo()
            .setPApplicationName( "my_application" )
            .setApplicationVersion(  VK_MAKE_VERSION( 1, 0, 0 ) )
            .setApiVersion( VK_API_VERSION_1_3 )
        )
        .add_layer(
          "VK_LAYER_KHRONOS_validation"
        )
    )
  );
  std::unordered_set< nlohmann::json > visited;
  nlohmann::json root = nlohmann::json::array();
  for( auto &group: instance->get_physical_devices( {} ) ) {
    if( group.devices.size() == 1 ) {
      const auto j = nlohmann::json( *group.devices[ 0 ] );
      if( j.contains( "props" ) && j[ "props" ].contains( "id" ) ) {
        nlohmann::json ident = nlohmann::json::object();
        ident[ "deviceUUID" ] = j[ "props" ][ "id" ][ "deviceUUID" ];
        ident[ "driverUUID" ] = j[ "props" ][ "id" ][ "driverUUID" ];
        if( visited.find( ident ) == visited.end() ) {
          visited.insert( ident );
          root.push_back( j );
        }
      }
      else {
        root.push_back( j );
      }
    }
  }
  std::cout << gct::remove_chain( root ).dump( 2 ) << std::endl;
}

