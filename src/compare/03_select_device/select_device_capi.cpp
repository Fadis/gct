#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vw/config.h>
#include <vw/instance.h>

int main( int argc, const char *argv[] ) {
  const auto configs = vw::parse_configs( argc, argv );
  auto instance = vw::create_instance(
    configs,
    {},
    {}
  );
  uint32_t device_count = 0u;
  if( vkEnumeratePhysicalDevices( *instance, &device_count, nullptr ) != VK_SUCCESS )
    return 1;
  std::vector< VkPhysicalDevice > devices( device_count );
  if( vkEnumeratePhysicalDevices( *instance, &device_count, devices.data() ) != VK_SUCCESS )
    return 1;
  if( devices.empty() ) {
    std::cerr << "利用可能なデバイスがない" << std::endl;
    return 1;
  }
  std::vector< const char* > dext;
  std::vector< const char* > dlayers;
  devices.erase( std::remove_if( devices.begin(), devices.end(), [&]( const auto &device ) -> bool {
    uint32_t ext_count = 0u;
    if( vkEnumerateDeviceExtensionProperties( device, nullptr, &ext_count, nullptr ) != VK_SUCCESS )
      return 1;
    std::vector< VkExtensionProperties > avail_dext( ext_count );
    if( vkEnumerateDeviceExtensionProperties( device, nullptr, &ext_count, avail_dext.data() ) != VK_SUCCESS )
      return 1;
    for( const char *w: dext )
      if( std::find_if( avail_dext.begin(), avail_dext.end(), [&]( const auto &v ) { return !strcmp( v.extensionName, w ); } ) == avail_dext.end() ) return true;
    uint32_t layer_count = 0u;
    if( vkEnumerateDeviceLayerProperties( device, &layer_count, nullptr ) != VK_SUCCESS )
      return true;
    std::vector< VkLayerProperties > avail_dlayers( layer_count );
    if( vkEnumerateDeviceLayerProperties( device, &layer_count, avail_dlayers.data() ) != VK_SUCCESS )
      return true;
    for( const char *w: dlayers )
      if( std::find_if( avail_dlayers.begin(), avail_dlayers.end(), [&]( const auto &v ) { return !strcmp( v.layerName, w ); } ) == avail_dlayers.end() ) return true;
    uint32_t queue_props_count = 0u;
    vkGetPhysicalDeviceQueueFamilyProperties( device, &queue_props_count, nullptr );
    std::vector< VkQueueFamilyProperties > queue_props( queue_props_count );
    vkGetPhysicalDeviceQueueFamilyProperties( device, &queue_props_count, queue_props.data() );



    bool presentable = false;
    if( configs.direct ) {
      uint32_t display_count = 0u;
      if( vkGetPhysicalDeviceDisplayPropertiesKHR( device, &display_count, nullptr ) != VK_SUCCESS )
        return 1;
      std::vector< VkDisplayPropertiesKHR > displays( display_count );
      if( vkGetPhysicalDeviceDisplayPropertiesKHR( device, &display_count, displays.data() ) != VK_SUCCESS )
        return 1;
      if( displays.size() <= configs.display_index ) return true;
      const auto &display = displays[ configs.display_index ];
      uint32_t mode_count = 0u;
      if( vkGetDisplayModePropertiesKHR( device, display.display, &mode_count, nullptr ) != VK_SUCCESS )
        return 1;
      std::vector< VkDisplayModePropertiesKHR > modes( mode_count );
      if( vkGetDisplayModePropertiesKHR( device, display.display, &mode_count, modes.data() ) != VK_SUCCESS )
        return 1;
      uint32_t width = configs.width;
      uint32_t height = configs.height;
      if( width == 0 && height == 0 ) {
        width = display.physicalResolution.width;
        height = display.physicalResolution.height;
      }
      std::vector< vk::DisplayModePropertiesKHR > available_modes;
      std::copy_if( modes.begin(), modes.end(), std::back_inserter( available_modes ), [&]( const auto &m ) { return m.parameters.visibleRegion.width == width && m.parameters.visibleRegion.height == height; } );
      if( available_modes.empty() ) return true;
      presentable = true;
    }
    else {
      for( uint32_t i = 0; i < queue_props.size(); ++i ) {
        if( glfwGetPhysicalDevicePresentationSupport( *instance, device, i ) ) {
          presentable = true;
          break;
        }
      }
    }
    return !presentable;
  } ), devices.end() );
  if( devices.empty() ) {
    std::cerr << "必要な拡張とレイヤーを備えたデバイスがない" << std::endl;
    exit( 1 );
  }
  std::cout << "利用可能なデバイス" << std::endl;
  for( unsigned int index = 0u; index != devices.size(); ++index ) {
    VkPhysicalDeviceProperties prop;
    vkGetPhysicalDeviceProperties( devices[ index ], &prop );
    std::cout << index << ": " << prop.deviceName << std::endl;
  }
}

