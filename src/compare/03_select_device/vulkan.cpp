#include <iostream>
#include <gct/instance.hpp>
#include <gct/physical_device.hpp>

int main() {
  // インスタンスを作成
  std::shared_ptr< gct::instance_t > instance(
    new gct::instance_t(
      gct::instance_create_info_t()
        // アプリケーションの情報を指定
        .set_application_info(
          vk::ApplicationInfo()
            // アプリケーションの名前
            .setPApplicationName( "my_application" )
            // アプリケーションのバージョン
            .setApplicationVersion(  VK_MAKE_VERSION( 1, 0, 0 ) )
            // 使用するVulkanのバージョンをVulkan 1.2にする
            .setApiVersion( VK_API_VERSION_1_2 )
        )
        // バリデーションレイヤーを使う
        .add_layer(
          "VK_LAYER_KHRONOS_validation"
        )
    )
  );
  const auto physical_device =
    instance->get_physical_devices( {} )[ 0 ].devices[ 0 ];

  // デバイスに備わっているキューを取得
  uint32_t queue_props_count = 0u;
  vkGetPhysicalDeviceQueueFamilyProperties( **physical_device, &queue_props_count, nullptr );
  std::vector< VkQueueFamilyProperties > queue_props( queue_props_count );
  vkGetPhysicalDeviceQueueFamilyProperties( **physical_device, &queue_props_count, queue_props.data() );
  uint32_t queue_family_index = 0u;
  // 計算要求を受け付けるキューを探す
  for( uint32_t i = 0; i < queue_props.size(); ++i ) {
    if( queue_props[ i ].queueFlags & VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT ) {
      queue_family_index = i;
      break;
    }
  }

  const float priority = 0.0f;
  // 計算要求を受け付けるキューを1つください
  VkDeviceQueueCreateInfo queue_create_info;
  queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queue_create_info.pNext = nullptr;
  queue_create_info.flags = 0;
  queue_create_info.queueFamilyIndex = queue_family_index;
  queue_create_info.queueCount = 1;
  queue_create_info.pQueuePriorities = &priority;

  // 拡張VK_EXT_pipeline_creation_feedbackを使う
  std::vector< const char* > extension{
    VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
  };

  // 論理デバイスを作る
  VkDeviceCreateInfo device_create_info;
  device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  device_create_info.pNext = nullptr;
  device_create_info.flags = 0;
  device_create_info.queueCreateInfoCount = 1;
  device_create_info.pQueueCreateInfos = &queue_create_info;
  device_create_info.enabledLayerCount = 0;
  device_create_info.ppEnabledLayerNames = nullptr;
  device_create_info.enabledExtensionCount = extension.size();
  device_create_info.ppEnabledExtensionNames = extension.data();
  device_create_info.pEnabledFeatures = nullptr;
  VkDevice device;
  if( vkCreateDevice(
    **physical_device,
    &device_create_info,
    nullptr,
    &device
  ) != VK_SUCCESS ) abort();
  
  // デバイスからキューを取得
  VkQueue queue;
  vkGetDeviceQueue(
    device,
    queue_family_index,
    0u,
    &queue
  );

  // 論理デバイスを捨てる
  vkDestroyDevice(
    device,
    nullptr
  );
}


