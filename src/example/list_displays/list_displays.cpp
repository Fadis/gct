#include <iostream>
#include <cstdint>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json.hpp>

int main( int argc, const char *argv[] ) {
#ifdef VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
  // Vulkan-HppからvkCreateInstanceを呼べるようにする
#if VK_HEADER_VERSION >= 301
    vk::detail::DynamicLoader dl;
#else
    vk::DynamicLoader dl;
#endif
  PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr =
  dl.getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" );
  VULKAN_HPP_DEFAULT_DISPATCHER.init( vkGetInstanceProcAddr );
#endif
  const auto app_info = vk::ApplicationInfo()
    // アプリケーションの名前
    .setPApplicationName( argc ? argv[ 0 ] : "my_application" )
    // アプリケーションのバージョン
    .setApplicationVersion( VK_MAKE_VERSION(1, 0, 0) )
    // エンジンの名前
    .setPEngineName( "my_engine" )
    // エンジンのバージョン
    .setEngineVersion( VK_MAKE_VERSION(1, 0, 0) )
    // 使用するVulkanのバージョンをVulkan 1.2にする
    .setApiVersion( VK_API_VERSION_1_2 );
  // バリデーションレイヤーを使う
  const std::vector< const char * > layers{
    "VK_LAYER_KHRONOS_validation"
  };
  // 以下の拡張を使う
  const std::vector< const char* > exts{
    "VK_KHR_surface", // 表示する為の描画を行う
    "VK_KHR_display" // ディスプレイの操作を行う
  };
  // インスタンスを作成
  auto instance = vk::createInstanceUnique(
    vk::InstanceCreateInfo()
      // アプリケーションの情報を指定
      .setPApplicationInfo( &app_info )
      // 使用するレイヤーを指定
      .setPEnabledLayerNames( layers )
      // 使用する拡張を指定
      .setPEnabledExtensionNames( exts )
  );
#ifdef VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
  // Vulkan-Hppからこのインスタンスで利用可能な全ての関数を呼べるようにする
  VULKAN_HPP_DEFAULT_DISPATCHER.init( *instance );
#endif
  // インスタンスがサポートするVulkanのバージョンを取得  
  const auto version = vk::enumerateInstanceVersion();
  std::cout << "Vulkan " <<
    VK_VERSION_MAJOR( version ) << "." <<
    VK_VERSION_MINOR( version ) << "." <<
    VK_VERSION_PATCH( version ) << std::endl; 

  const auto devices = instance->enumeratePhysicalDevices();
  for( const auto &device: devices ) {
    // GPUの情報を取得
    const auto props = device.getProperties();
    // このGPUに繋がっているディスプレイの情報を取得
    const auto displays = device.getDisplayPropertiesKHR();
    for( const auto &display: displays ) {
      std::cout << "GPU " << props.deviceName <<
        " にディスプレイ " << display.displayName <<
        " が接続されている" << std::endl;
      std::cout << "  最大解像度 : " <<
        display.physicalResolution.width << "x" <<
        display.physicalResolution.height << std::endl;
      // ディスプレイの対応モードを取得
      const auto modes = device.getDisplayModePropertiesKHR( display.display );
      for( const auto &mode: modes ) {
        std::cout << "  対応モード : " <<
          mode.parameters.visibleRegion.width << "x" <<
          mode.parameters.visibleRegion.height << "@" <<
          float( mode.parameters.refreshRate ) / 1000.f <<
          "Hz" << std::endl;
      }
    }
  }
}

