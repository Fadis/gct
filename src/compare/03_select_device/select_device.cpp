#include <iostream>
#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <vw/config.h>
#include <vw/instance.h>

int main( int argc, const char *argv[] ) {
  const auto configs = vw::parse_configs( argc, argv );
  auto instance = vw::create_instance(
    configs,
    {},
    {}
  );
  auto devices = instance->enumeratePhysicalDevices();
  if( devices.empty() ) {
    std::cerr << "利用可能なデバイスがない" << std::endl;
    exit( 1 );
  }
  std::vector< const char* > dext;
  std::vector< const char* > dlayers;
  devices.erase( std::remove_if( devices.begin(), devices.end(), [&]( const auto &device ) -> bool {
    auto avail_dext = device.enumerateDeviceExtensionProperties();
    for( const char *w: dext )
      if( std::find_if( avail_dext.begin(), avail_dext.end(), [&]( const auto &v ) { return !strcmp( v.extensionName, w ); } ) == avail_dext.end() ) return true;
    const auto avail_dlayers = device.enumerateDeviceLayerProperties();
    for( const char *w: dlayers )
      if( std::find_if( avail_dlayers.begin(), avail_dlayers.end(), [&]( const auto &v ) { return !strcmp( v.layerName, w ); } ) == avail_dlayers.end() ) return true;
    const auto queue_props = device.getQueueFamilyProperties();
    bool presentable = false;
    if( configs.direct ) {
      const auto displays = device.getDisplayPropertiesKHR();
      if( displays.size() <= configs.display_index ) return true;
      const auto &display = displays[ configs.display_index ];
      auto modes = device.getDisplayModePropertiesKHR( display.display );
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
    const auto prop = devices[ index ].getProperties();
    std::cout << index << ": " << prop.deviceName << std::endl;
  }
  exit( 0 );
}

