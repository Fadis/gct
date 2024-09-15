#include <iostream>
#include <cstdint>
#include <vector>
#include <chrono>
#include <thread>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json.hpp>
#include <vulkan2json/SurfaceFormatKHR.hpp>

int main( int argc, const char *argv[] ) {
#ifdef VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
  // Vulkan-HppからvkCreateInstanceを呼べるようにする
  vk::DynamicLoader dl;
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
    "VK_KHR_display", // ディスプレイの操作を行う
    "VK_EXT_swapchain_colorspace"
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

  // 1個目のGPUに繋がっている1個目のディスプレイを1個目の表示モードで使う
  const auto devices = instance->enumeratePhysicalDevices();
  if( devices.empty() ) std::abort();
  const auto displays = devices[ 0 ].getDisplayPropertiesKHR();
  if( displays.empty() ) std::abort();
  const auto modes = devices[ 0 ].getDisplayModePropertiesKHR( displays[ 0 ].display );
  if( modes.empty() ) std::abort();
  // この条件でサーフェスを作る
  const auto surface = instance->createDisplayPlaneSurfaceKHRUnique(
    vk::DisplaySurfaceCreateInfoKHR()
      .setDisplayMode( modes[ 0 ].displayMode )
      .setImageExtent( modes[ 0 ].parameters.visibleRegion )
  );

  // サーフェスで利用可能なピクセルフォーマットを取得する
  const auto available_formats = devices[ 0 ].getSurfaceFormatsKHR(
    *surface
  );
  for( const auto &format: available_formats ) {
    std::cout << "利用可能なフォーマット : " << nlohmann::json( format ) << std::endl;
  }
  // 欲しいピクセルパッキング方法
  const std::vector< vk::Format > expected_formats{
    vk::Format::eA2B10G10R10UnormPack32, // 30bit BGR
    vk::Format::eA8B8G8R8UnormPack32, // 24bit BGR
    vk::Format::eB8G8R8A8Unorm, // 24bit RGB
    vk::Format::eR5G6B5UnormPack16 // 16bit RGB
  };

  // 色空間がsRGBで上記の方法でパッキングされたフォーマットを探す
  vk::Format selected_format = vk::Format::eUndefined;
  for( const auto &e: expected_formats ) {
    if( std::find_if(
      available_formats.begin(), available_formats.end(),
      [e]( const auto &f ) {
        return
          f.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear &&
          f.format == e;
      }
    ) != available_formats.end() ) {
      selected_format = e;
      break;
    }
  }

  // サーフェスの情報を取得
  const auto surface_capabilities = devices[ 0 ].getSurfaceCapabilitiesKHR(
    *surface
  );
  std::cout << "サーフェスの最小イメージ数 : " <<
    surface_capabilities.minImageCount << std::endl;
  std::cout << "サーフェスの最大イメージ数 : " <<
    surface_capabilities.maxImageCount << std::endl;

  // デバイスに備わっているキューを取得
  const auto queue_props = devices[ 0 ].getQueueFamilyProperties();
  uint32_t queue_family_index = 0u;
  // 描画要求を受け付けるキューを探す
  for( uint32_t i = 0; i < queue_props.size(); ++i ) {
    if( queue_props[ i ].queueFlags & vk::QueueFlagBits::eGraphics ) {
      queue_family_index = i;
      break;
    }
  }

  // 以下のデバイス拡張を使う
  std::vector< const char* > dexts{
    "VK_KHR_swapchain" // スワップチェインを使う
  };

  const float priority = 0.0f;
  // 描画要求を受け付けるキューを1つください
  std::vector< vk::DeviceQueueCreateInfo > queues{
    vk::DeviceQueueCreateInfo()
      .setQueueFamilyIndex( queue_family_index )
      .setQueuePriorities( priority )
  };

  // 論理デバイスを作る
  auto device = devices[ 0 ].createDeviceUnique(
    vk::DeviceCreateInfo()
      // このキューを使う
      .setQueueCreateInfos( queues )
      // このデバイス拡張を使う
      .setPEnabledExtensionNames( dexts )
  );

  // デバイスからキューを取得
  auto queue = device->getQueue( queue_family_index, 0u );

  // スワップチェーンを作る
  const auto swapchain = device->createSwapchainKHRUnique(
    vk::SwapchainCreateInfoKHR()
      // このサーフェスに描画結果を送る
      .setSurface( *surface )
      // スワップチェーンのイメージの数
      .setMinImageCount( surface_capabilities.minImageCount )
      // スワップチェーンのイメージのフォーマット
      .setImageFormat( selected_format )
      // スワップチェーンのイメージの色空間
      .setImageColorSpace( vk::ColorSpaceKHR::eSrgbNonlinear ) 
      // スワップチェーンのイメージの大きさ
      .setImageExtent( surface_capabilities.currentExtent )
      // レイヤーは1つ
      .setImageArrayLayers( 1u )
      // スワップチェーンのイメージは転送先にも使える必要がある
      .setImageUsage(
        vk::ImageUsageFlagBits::eTransferDst
      )
      // 投げたイメージが投げた順にサーフェスに送られるようなスワップチェーン
      .setPresentMode( vk::PresentModeKHR::eFifo )
      .setClipped( false )
  );

  // スワップチェーンのイメージを取得する
  const auto swapchain_images = device->getSwapchainImagesKHR( *swapchain );
  std::cout << "スワップチェインのイメージの数 : " <<
    swapchain_images.size() << std::endl;

  // コマンドプールを作る
  const auto command_pool = device->createCommandPoolUnique(
    vk::CommandPoolCreateInfo()
      .setQueueFamilyIndex( 0u )
  );

  {
    // コマンドバッファを作る
    const auto command_buffers = device->allocateCommandBuffersUnique(
      vk::CommandBufferAllocateInfo()
        .setCommandPool( *command_pool )
        .setLevel( vk::CommandBufferLevel::ePrimary )
        .setCommandBufferCount( 1u )
    );
    const auto &command_buffer = command_buffers[ 0 ];
 
    // ここからコマンドバッファにコマンドを積む
    command_buffer->begin(
      vk::CommandBufferBeginInfo()
    );
    for( auto &image: swapchain_images ) {
      // スワップチェインのイメージを転送先に適した形式に変換する
      command_buffer->pipelineBarrier(
        vk::PipelineStageFlagBits::eTransfer,
        vk::PipelineStageFlagBits::eTransfer,
        vk::DependencyFlagBits::eByRegion,
        {},
        {},
        {
          vk::ImageMemoryBarrier()
            .setSrcAccessMask( vk::AccessFlagBits::eTransferWrite )
            .setDstAccessMask( vk::AccessFlagBits::eTransferRead )
            .setOldLayout( vk::ImageLayout::eUndefined )
            .setNewLayout( vk::ImageLayout::eTransferDstOptimal )
            .setSrcQueueFamilyIndex( 0 )
            .setDstQueueFamilyIndex( 0 )
            .setImage( image )
            .setSubresourceRange(
              vk::ImageSubresourceRange()
                .setAspectMask( vk::ImageAspectFlagBits::eColor )
                .setBaseMipLevel( 0u )
                .setLevelCount( 1u )
                .setBaseArrayLayer( 0u )
                .setLayerCount( 1u )
            )
        }
      );
      // スワップチェインのイメージを青で塗りつぶす
      command_buffer->clearColorImage(
        image, // このイメージを
        vk::ImageLayout::eTransferDstOptimal,
        // 青
        vk::ClearColorValue()
          .setFloat32( std::array< float, 4u >{ 0.f, 0.f, 1.f, 1.f } ),
        {
          vk::ImageSubresourceRange()
            // 色を書き換える
            .setAspectMask( vk::ImageAspectFlagBits::eColor )
            // 0番目のミップレベル
            .setBaseMipLevel( 0u )
            .setLevelCount( 1u )
            // 0番目のレイヤー
            .setBaseArrayLayer( 0u )
            .setLayerCount( 1u )
        }
      );
      // スワップチェインのイメージを表示に適した形式に変換する
      command_buffer->pipelineBarrier(
        vk::PipelineStageFlagBits::eTransfer,
        vk::PipelineStageFlagBits::eTransfer,
        vk::DependencyFlagBits::eByRegion,
        {},
        {},
        {
          vk::ImageMemoryBarrier()
            .setSrcAccessMask( vk::AccessFlagBits::eTransferWrite )
            .setDstAccessMask( vk::AccessFlagBits::eTransferRead )
            .setOldLayout( vk::ImageLayout::eTransferDstOptimal )
            .setNewLayout( vk::ImageLayout::ePresentSrcKHR )
            .setSrcQueueFamilyIndex( 0 )
            .setDstQueueFamilyIndex( 0 )
            .setImage( image )
            .setSubresourceRange(
              vk::ImageSubresourceRange()
                .setAspectMask( vk::ImageAspectFlagBits::eColor )
                .setBaseMipLevel( 0u )
                .setLevelCount( 1u )
                .setBaseArrayLayer( 0u )
                .setLayerCount( 1u )
            )
        }
      );
    }
    // コマンドバッファの記録を終わる
    command_buffer->end();
    // コマンドバッファをキューに流す
    queue.submit(
      vk::SubmitInfo()
        .setCommandBufferCount( 1u )
        .setPCommandBuffers( &*command_buffer ),
      VK_NULL_HANDLE
    );
    // キューに流したコマンドが完了するまで待つ
    queue.waitIdle();
  }

  // スワップチェインのイメージと同数のセマフォを作る
  std::vector<
    vk::UniqueHandle< vk::Semaphore, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE >
  > image_acquired;
  for( auto &image: swapchain_images ) {
    image_acquired.push_back(
      device->createSemaphoreUnique(
        vk::SemaphoreCreateInfo()
      )
    );
  }

  // 1フレームの表示時間
  const auto frame_time = std::chrono::microseconds(
    1000*1000*1000/modes[ 0 ].parameters.refreshRate
  );

  uint32_t current_frame = 0u;
  while( 1 ) {

    const auto begin_time = std::chrono::high_resolution_clock::now();

    // スワップチェインから次に書くべきイメージのindexを貰う
    std::uint32_t image_index = device->acquireNextImageKHR(
      // このスワップチェーンから
      *swapchain,
      // イメージが貰えるまでいくらでも待つ
      std::numeric_limits< std::uint64_t >::max(),
      // イメージが表示から外れたらこのセマフォに通知
      *image_acquired[ current_frame ],
      VK_NULL_HANDLE
    ).value;

    // イメージを書き換えないですぐに表示にまわす
    if( queue.presentKHR(
      vk::PresentInfoKHR()
        .setWaitSemaphoreCount( 1u )
        // このセフォに通知が来るまで待ってから
	      .setPWaitSemaphores( &*image_acquired[ current_frame ] )
	      .setSwapchainCount( 1 )
        // このスワップチェーンの
	      .setPSwapchains( &*swapchain )
        // このイメージをサーフェスに送る
	      .setPImageIndices( &image_index )
    ) != vk::Result::eSuccess ) std::abort();

    ++current_frame;
    current_frame %= swapchain_images.size();

    // 1フレームの表示時間が経過するまで待つ
    const auto end_time = std::chrono::high_resolution_clock::now();
    const auto elapsed_time = end_time - begin_time;
    if( elapsed_time < frame_time ) {
      const auto sleep_for = frame_time - elapsed_time;
      std::this_thread::sleep_for( sleep_for );
    }
  }
}

