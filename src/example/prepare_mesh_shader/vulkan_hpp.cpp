#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <vulkan2json/PhysicalDeviceMeshShaderFeaturesEXT.hpp>
#include <vulkan2json/PhysicalDeviceMeshShaderPropertiesEXT.hpp>
#include <gct/instance.hpp>
#include <gct/physical_device.hpp>
#include <gct/spirv_reflect.h>
#include <gct/spv2json.hpp>
#include <vulkan/vulkan_structs.hpp>

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
            .setApplicationVersion(  VK_MAKE_VERSION( 1, 3, 0 ) )
            // 使用するVulkanのバージョンをVulkan 1.2にする
            .setApiVersion( VK_API_VERSION_1_3 )
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
  // 描画要求を受け付けるキューを探す
  for( uint32_t i = 0; i < queue_props.size(); ++i ) {
    if( queue_props[ i ].queueFlags & vk::QueueFlagBits::eGraphics ) {
      queue_family_index = i;
      break;
    }
  }

  const float priority = 0.0f;
  // 描画要求を受け付けるキューを1つください
  std::vector< vk::DeviceQueueCreateInfo > queues{
    vk::DeviceQueueCreateInfo()
      .setQueueFamilyIndex( queue_family_index )
      .setQueueCount( 1 )
      .setPQueuePriorities( &priority )
  };

  // 拡張VK_EXT_mesh_shaderを使う
  std::vector< const char* > extension{
    VK_EXT_MESH_SHADER_EXTENSION_NAME
  };

  // 物理デバイスの情報を取得
  // メッシュシェーダーに関する情報も一緒に取得する
  vk::PhysicalDeviceProperties2 properties;
  vk::PhysicalDeviceMeshShaderPropertiesEXT mesh_shader_properties;
  properties.pNext = &mesh_shader_properties;
  (*physical_device)->getProperties2( &properties );

  std::cout << "GPU Name : " << properties.properties.deviceName << std::endl;

  std::cout << "Mesh Shader Properties : " << std::endl;
  std::cout << nlohmann::json( mesh_shader_properties ).dump( 2 ) << std::endl;

  // 物理デバイスで使える機能を取得
  vk::PhysicalDeviceFeatures2 features;
  // メッシュシェーダーの対応状況も一緒に取得する
  vk::PhysicalDeviceMeshShaderFeaturesEXT mesh_shader_features;
  features.setPNext( &mesh_shader_features );
  
  // maintenance4の対応状況も一緒に取得する
  // Vulkan 1.3以上向けにコンパイルされたシェーダーはLocalSizeIdを使う可能性があるので、それに対応できるようにする 
  vk::PhysicalDeviceMaintenance4Features maintenance4_features;
  mesh_shader_features.setPNext( &maintenance4_features );

  (*physical_device)->getFeatures2( &features );
  
  std::cout << "Mesh Shader Features : " << std::endl;
  std::cout << nlohmann::json( mesh_shader_features ).dump( 2 ) << std::endl;

  // メッシュシェーダーからマルチビューを使わない
  // この機能を使う場合は別途vk::PhysicalDeviceMultiviewFeaturesを設定する必要がある
  mesh_shader_features.setMultiviewMeshShader( false );
  
  // メッシュシェーダーからシェーディングレートの指定を行わない
  // この機能を使う場合は別途vk::PhysicalDeviceFragmentShadingRateKHRを設定する必要がある
  mesh_shader_features.setPrimitiveFragmentShadingRateMeshShader( false );

  // メッシュシェーダーを有効にして論理デバイスを作る
  vk::DeviceCreateInfo dci;
  dci
    .setQueueCreateInfoCount( queues.size() )
    .setPQueueCreateInfos( queues.data() )
    .setEnabledExtensionCount( extension.size() )
    .setPpEnabledExtensionNames( extension.data() );
  dci.setPNext( &mesh_shader_features );

  auto device = (*physical_device)->createDeviceUnique( dci );
  
  // デバイスからキューを取得
  auto queue = device->getQueue( queue_family_index, 0u );

  // ファイルからSPIR-Vを読む
  std::fstream mesh_file( CMAKE_CURRENT_BINARY_DIR "/geometry/geometry.mesh.spv", std::ios::in|std::ios::binary );
  if( !mesh_file.good() ) abort();
  std::vector< std::uint8_t > mesh_code;
  mesh_code.assign(
    std::istreambuf_iterator< char >( mesh_file ),
    std::istreambuf_iterator< char >()
  );

  // メッシュシェーダーのシェーダーモジュールを作る
  auto mesh_shader = device->createShaderModuleUnique(
    vk::ShaderModuleCreateInfo()
      .setCodeSize( mesh_code.size() )
      .setPCode( reinterpret_cast< const uint32_t* >( mesh_code.data() ) )
  );
  
  // シェーダーの内容をパースする
  SpvReflectShaderModule reflect;
  if( spvReflectCreateShaderModule(
    mesh_code.size(),
    mesh_code.data(),
    &reflect
  ) != SPV_REFLECT_RESULT_SUCCESS ) std::abort();

  // シェーダーがどのシェーダーステージ用の物かを表示する
  std::cout << "Shader Stage : " << nlohmann::json( SpvReflectShaderStageFlagBits( reflect.shader_stage ) ) << std::endl;
 
  // ファイルからSPIR-Vを読む
  std::fstream fragment_file( CMAKE_CURRENT_BINARY_DIR "/geometry/geometry.frag.spv", std::ios::in|std::ios::binary );
  if( !fragment_file.good() ) abort();
  std::vector< std::uint8_t > fragment_code;
  fragment_code.assign(
    std::istreambuf_iterator< char >( fragment_file ),
    std::istreambuf_iterator< char >()
  );

  // フラグメントシェーダーのシェーダーモジュールを作る
  auto fragment_shader = device->createShaderModuleUnique(
    vk::ShaderModuleCreateInfo()
      .setCodeSize( fragment_code.size() )
      .setPCode( reinterpret_cast< const uint32_t* >( fragment_code.data() ) )
  );

  // 空のデスクリプタセットレイアウトを作る
  const auto descriptor_set_layout = device->createDescriptorSetLayoutUnique( {} );
  
  // パイプラインレイアウトを作る
  auto pipeline_layout = device->createPipelineLayoutUnique(
    vk::PipelineLayoutCreateInfo()
      .setSetLayoutCount( 1u )
      // このレイアウトのデスクリプタセットとくっつく
      .setPSetLayouts( &*descriptor_set_layout )
  );

  const std::vector< vk::AttachmentDescription > attachments{
    // 色を出力するアタッチメントを追加
    vk::AttachmentDescription()
      // RGBA各8bitの
      .setFormat( vk::Format::eR8G8B8A8Unorm )
      // サンプリング点が1テクセルあたり1つで
      .setSamples( vk::SampleCountFlagBits::e1 )
      // レンダーパスに入る前の値は読めなくてもよくて
      .setLoadOp( vk::AttachmentLoadOp::eClear )
      // 書いた内容はレンダーパスの後で読める必要があり
      .setStoreOp( vk::AttachmentStoreOp::eStore )
      // ステンシルとして読める必要がなくて
      .setStencilLoadOp( vk::AttachmentLoadOp::eDontCare )
      // ステンシルとして書ける必要もない
      .setStencilStoreOp( vk::AttachmentStoreOp::eDontCare )
      // 初期レイアウトは何でも良くて
      .setInitialLayout( vk::ImageLayout::eUndefined )
      // このレンダーパスが汎用的に使えるレイアウトになっている
      .setFinalLayout( vk::ImageLayout::eGeneral ),
    // 深度を出力するアタッチメントを追加
    vk::AttachmentDescription()
      // D16bitの
      .setFormat( vk::Format::eD16Unorm )
      // サンプリング点が1テクセルあたり1つで
      .setSamples( vk::SampleCountFlagBits::e1 )
      // レンダーパスに入る前の値は読めなくてもよくて
      .setLoadOp( vk::AttachmentLoadOp::eClear )
      // 書いた内容はレンダーパスの後で読める必要があり
      .setStoreOp( vk::AttachmentStoreOp::eStore )
      // ステンシルとして読める必要がなくて
      .setStencilLoadOp( vk::AttachmentLoadOp::eDontCare )
      // ステンシルとして書ける必要もない
      .setStencilStoreOp( vk::AttachmentStoreOp::eDontCare )
      // 初期レイアウトは何でも良くて
      .setInitialLayout( vk::ImageLayout::eUndefined )
      // このレンダーパスが終わったら深度とステンシルを保持するのに適したレイアウトになっている
      .setFinalLayout( vk::ImageLayout::eDepthStencilAttachmentOptimal )
  };

  const auto color_attachment =
    vk::AttachmentReference()
      // 0番目のアタッチメントを
      .setAttachment( 0 )
      // 色を吐くのに丁度良いレイアウトにして使う
      .setLayout( vk::ImageLayout::eColorAttachmentOptimal );

  const auto depth_attachment =
    vk::AttachmentReference()
      // 1番目のアタッチメントを
      .setAttachment( 1 )
      // 深度とステンシルを吐くのに丁度良いレイアウトにして使う
      .setLayout( vk::ImageLayout::eDepthStencilAttachmentOptimal );

  // サブパスを定義する
  const std::vector< vk::SubpassDescription > subpass{
    vk::SubpassDescription()
      // グラフィクスパイプラインに対応するサブパス
      .setPipelineBindPoint( vk::PipelineBindPoint::eGraphics )
      // Color Attachmentを1つ使う
      .setColorAttachmentCount( 1 )
      .setPColorAttachments( &color_attachment )
      // Depth Stencil Attachmentを使う
      .setPDepthStencilAttachment( &depth_attachment )
  };

  // レンダーパスを作る
  const auto render_pass = device->createRenderPassUnique(
    vk::RenderPassCreateInfo()
      // このアタッチメントがあり
      .setAttachmentCount( attachments.size() )
      .setPAttachments( attachments.data() )
      // このサブパスがある
      .setSubpassCount( subpass.size() )
      .setPSubpasses( subpass.data() )
  );

  // 使用するシェーダモジュール
  const std::vector< vk::PipelineShaderStageCreateInfo > shader{
    vk::PipelineShaderStageCreateInfo()
      .setStage( vk::ShaderStageFlagBits::eMeshEXT )
      .setModule( *mesh_shader )
      .setPName( "main" ),
    vk::PipelineShaderStageCreateInfo()
      .setStage( vk::ShaderStageFlagBits::eFragment )
      .setModule( *fragment_shader )
      .setPName( "main" )
  };

  // ビューポートとシザーの設定
  const auto viewport =
    vk::PipelineViewportStateCreateInfo()
      // 1個のビューポートと
      .setViewportCount( 1 )
      // 1個のシザーを使う
      .setScissorCount( 1 );
      // 具体的な値は後でDynamicStateを使って設定する

  // ラスタライズの設定
  const auto rasterization =
    vk::PipelineRasterizationStateCreateInfo()
      // 範囲外の深度を丸めない
      .setDepthClampEnable( false )
      // ラスタライズを行う
      .setRasterizerDiscardEnable( false )
      // 三角形の中を塗る
      .setPolygonMode( vk::PolygonMode::eFill )
      // 背面カリングを行わない
      .setCullMode( vk::CullModeFlagBits::eNone )
      // 表面は時計回り
      .setFrontFace( vk::FrontFace::eClockwise )
      // 深度バイアスを使わない
      .setDepthBiasEnable( false )
      // 線を描く時は太さ1.0で
      .setLineWidth( 1.0f );

  // マルチサンプルの設定
  const auto multisample =
    // 全部デフォルト(マルチサンプルを使わない)
    vk::PipelineMultisampleStateCreateInfo();

  // 深度とステンシルの設定
  const auto depth_stencil =
    vk::PipelineDepthStencilStateCreateInfo()
      // 深度テストをする
      .setDepthTestEnable( true )
      // 深度値を深度バッファに書く
      .setDepthWriteEnable( true )
      // 深度値がより小さい場合手前と見做す
      .setDepthCompareOp( vk::CompareOp::eLessOrEqual )
      // 深度の範囲を制限しない
      .setDepthBoundsTestEnable( false )
      // ステンシルテストをしない
      .setStencilTestEnable( false );

  // カラーブレンドの設定
  const auto color_blend_attachment =
    vk::PipelineColorBlendAttachmentState()
      // フレームバッファに既にある色と新しい色を混ぜない
      // (新しい色で上書きする)
      .setBlendEnable( false )
      // RGBA全ての要素を書く
      .setColorWriteMask(
        vk::ColorComponentFlagBits::eR |
        vk::ColorComponentFlagBits::eG |
        vk::ColorComponentFlagBits::eB |
        vk::ColorComponentFlagBits::eA
      );
  const auto color_blend =
    vk::PipelineColorBlendStateCreateInfo()
      // 論理演算をしない
      .setLogicOpEnable( false )
      // 論理演算をする場合clearを使う
      .setLogicOp( vk::LogicOp::eClear )
      // カラーアタッチメント1つ分の設定がある
      .setAttachmentCount( 1 )
      // このブレンドの設定を0番目のカラーアタッチメントで使う
      .setPAttachments( &color_blend_attachment )
      // カラーブレンドに使う定数
      .setBlendConstants( { 0.f, 0.f, 0.f, 0.f } );


  // 後から変更できるパラメータの設定
  const std::vector< vk::DynamicState > dynamic_state{
    vk::DynamicState::eViewport,
    vk::DynamicState::eScissor
  };
  const auto dynamic =
    vk::PipelineDynamicStateCreateInfo()
      .setDynamicStateCount( dynamic_state.size() )
      .setPDynamicStates( dynamic_state.data() );

  // テッセレーションの設定
  const auto tessellation =
    // 全部デフォルト(テッセレーションを使わない)
    vk::PipelineTessellationStateCreateInfo();

  const auto create_info = 
    vk::GraphicsPipelineCreateInfo()
      .setStageCount( shader.size() )
      .setPStages( shader.data() )
  // メッシュシェーダーは頂点配列から頂点を読まないのでVertexInputStateとInputAssemblyStateは設定しなくて良い
  //    .setPVertexInputState( &vistat )
  //    .setPInputAssemblyState( &input_assembly )
      .setPTessellationState( &tessellation )
      .setPViewportState( &viewport )
      .setPRasterizationState( &rasterization )
      .setPMultisampleState( &multisample )
      .setPDepthStencilState( &depth_stencil )
      .setPColorBlendState( &color_blend )
      .setPDynamicState( &dynamic )
      .setLayout( *pipeline_layout )
      // このパイプラインレイアウトで
      .setRenderPass( *render_pass )
      // このレンダーパスの0番目のサブパスとして使う
      .setSubpass( 0 );
  
  // グラフィクスパイプラインを作る
  auto pipeline = device->createGraphicsPipelinesUnique(
    nullptr,
    { create_info },
    nullptr
  );

  // 空のデスクリプタセットのデスクリプタプールを作る
  const auto descriptor_pool = device->createDescriptorPoolUnique(
    vk::DescriptorPoolCreateInfo()
      // vkFreeDescriptorSetでデスクリプタセットを解放できる
      .setFlags( vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet )
      // 最大1セットのデスクリプタセットを確保できる
      .setMaxSets( 1 )
      .setPoolSizeCount( 0 )
  );

  // 空のデスクリプタセットを作る
  const auto descriptor_set = std::move( device->allocateDescriptorSetsUnique(
    vk::DescriptorSetAllocateInfo()
      // このデスクリプタプールから
      .setDescriptorPool( *descriptor_pool )
      // 1セット
      .setDescriptorSetCount( 1 )
      // この内容のデスクリプタセットを
      .setPSetLayouts( &*descriptor_set_layout )
  )[ 0 ] );

}


