#include <iostream>
#include <gct/instance.hpp>
#include <gct/physical_device.hpp>
#include <vulkan2json/PhysicalDeviceProperties.hpp>

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
  // デバイスの情報をダンプ
  for( auto &group: instance->get_physical_devices( {} ) ) {
    if( group.devices.size() == 1 )
      std::cout << nlohmann::json( group.devices[ 0 ]->get_props().get_basic() ).dump( 2 ) << std::endl;
  }
}


