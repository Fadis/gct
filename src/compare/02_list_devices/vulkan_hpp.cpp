#include <iostream>
#include <vulkan/vulkan.hpp>
#include <gct/instance.hpp>
#include <vulkan2json/PhysicalDeviceProperties.hpp>

int main() {
  std::shared_ptr< gct::instance_t > instance(
    new gct::instance_t(
      gct::instance_create_info_t()
        .set_application_info(
          vk::ApplicationInfo()
            .setPApplicationName( "my_application" )
            .setApplicationVersion(  VK_MAKE_VERSION( 1, 0, 0 ) )
            .setApiVersion( VK_API_VERSION_1_2 )
        )
        .add_layer(
          "VK_LAYER_KHRONOS_validation"
        )
    )
  );
  
  // デバイスの情報を取得
  auto devices = (*instance)->enumeratePhysicalDevices();
  // デバイスの情報をダンプ
  for( const auto &device: devices ) {
    const auto props = device.getProperties();
    std::cout << nlohmann::json( props ).dump( 2 ) << std::endl;
  }
}

