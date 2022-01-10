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
  const auto queue_props = (*physical_device)->getQueueFamilyProperties();
  uint32_t queue_family_index = 0u;
  // 計算要求を受け付けるキューを探す
  for( uint32_t i = 0; i < queue_props.size(); ++i ) {
    if( queue_props[ i ].queueFlags & vk::QueueFlagBits::eCompute ) {
      queue_family_index = i;
      break;
    }
  }

  const float priority = 0.0f;
  // 計算要求を受け付けるキューを1つください
  std::vector< vk::DeviceQueueCreateInfo > queues{
    vk::DeviceQueueCreateInfo()
      .setQueueFamilyIndex( queue_family_index )
      .setQueueCount( 1 )
      .setPQueuePriorities( &priority )
  };

  // 拡張VK_EXT_pipeline_creation_feedbackを使う
  std::vector< const char* > extension{
    VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
  };

  // 論理デバイスを作る
  auto device = (*physical_device)->createDeviceUnique(
    vk::DeviceCreateInfo()
      .setQueueCreateInfoCount( queues.size() )
      .setPQueueCreateInfos( queues.data() )
      .setEnabledExtensionCount( extension.size() )
      .setPpEnabledExtensionNames( extension.data() )
  );
  
  // デバイスからキューを取得
  auto queue = device->getQueue( queue_family_index, 0u );
}


