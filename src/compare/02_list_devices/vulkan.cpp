#include <iostream>
#include <vulkan/vulkan.h>
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

  // デバイスの個数を取得
  uint32_t device_count = 0u;
  if( vkEnumeratePhysicalDevices( **instance, &device_count, nullptr ) != VK_SUCCESS )
    return 1;
  // デバイスの情報を取得
  std::vector< VkPhysicalDevice > devices( device_count );
  if( vkEnumeratePhysicalDevices( **instance, &device_count, devices.data() ) != VK_SUCCESS )
    return 1;
  // デバイスの情報をダンプ
  for( const auto &device: devices ) {
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties( device, &props );
    std::cout << nlohmann::json( props ).dump( 2 ) << std::endl;
  }
}

