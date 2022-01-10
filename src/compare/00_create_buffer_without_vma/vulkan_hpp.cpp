#include <iostream>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <gct/instance.hpp>
#include <gct/device.hpp>
#include <gct/physical_device.hpp>
#include <gct/device_create_info.hpp>
#include <gct/instance_create_info.hpp>

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
  const auto physical_device =
    instance->get_physical_devices( {} )[ 0 ].devices[ 0 ];

  const auto queue_props = (*physical_device)->getQueueFamilyProperties();
  uint32_t queue_family_index = 0u;
  for( uint32_t i = 0; i < queue_props.size(); ++i ) {
    if( queue_props[ i ].queueFlags & vk::QueueFlagBits::eCompute ) {
      queue_family_index = i;
      break;
    }
  }

  const float priority = 0.0f;
  std::vector< vk::DeviceQueueCreateInfo > queues{
    vk::DeviceQueueCreateInfo()
      .setQueueFamilyIndex( queue_family_index )
      .setQueueCount( 1 )
      .setPQueuePriorities( &priority )
  };

  std::vector< const char* > extension{
    VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
  };

  auto device = (*physical_device)->createDeviceUnique(
    vk::DeviceCreateInfo()
      .setQueueCreateInfoCount( queues.size() )
      .setPQueueCreateInfos( queues.data() )
      .setEnabledExtensionCount( extension.size() )
      .setPpEnabledExtensionNames( extension.data() )
  );
  
  auto queue = device->getQueue( queue_family_index, 0u );

  
  // ステージングバッファ用のバッファを作る
  auto host_buffer = device->createBufferUnique(
    vk::BufferCreateInfo()
      .setSize( 1000 )
      .setUsage( vk::BufferUsageFlagBits::eTransferSrc )
  );

  // バッファに必要なメモリの要件を調べる
  auto host_buffer_memory_reqs = device->getBufferMemoryRequirements( *host_buffer );

  // ホスト側のメモリに置かれ、CPUから読み書きできるメモリを探す
  const auto memory_props = (*physical_device)->getMemoryProperties();
  std::uint32_t host_heap_index = 0u;
  for( std::uint32_t i = 0u; i != memory_props.memoryHeapCount; ++i ) {
    if( !( memory_props.memoryHeaps[ i ].flags & vk::MemoryHeapFlagBits::eDeviceLocal ) ) {
      host_heap_index = i;
      break;
    }
  }
  std::uint32_t host_memory_index = 0u;
  for( std::uint32_t i = 0u; i != memory_props.memoryTypeCount; ++i ) {
    if( memory_props.memoryTypes[ i ].propertyFlags & vk::MemoryPropertyFlagBits::eHostVisible ) {
      if( ( host_buffer_memory_reqs.memoryTypeBits >> i ) & 0x1 ) {
        if( memory_props.memoryTypes[ i ].heapIndex == host_heap_index ) {
          host_memory_index = i;
          break;
        }
      }
    }
  }
  
  // ステージングバッファ用のメモリを確保する
  auto host_memory = device->allocateMemoryUnique(
    vk::MemoryAllocateInfo()
      .setAllocationSize( host_buffer_memory_reqs.size )
      .setMemoryTypeIndex( host_memory_index )
  );

  // メモリをステージングバッファ用のバッファに結びつける
  device->bindBufferMemory(
    *host_buffer,
    *host_memory,
    0u
  );
  
  // ステージングバッファ用のメモリをプロセスのアドレス空間にマップする
  auto mapped = device->mapMemory(
    *host_memory,
    0u,
    host_buffer_memory_reqs.size,
    vk::MemoryMapFlags( 0u )
  );

  // ステージングバッファ用のメモリに値を書き込む
  std::fill(
    reinterpret_cast< std::uint32_t* >( mapped ),
    reinterpret_cast< std::uint32_t* >( mapped ) + 1000 / sizeof( std::uint32_t ),
    1u
  );

  // ステージングバッファ用のメモリをプロセスのアドレス空間から外す
  device->unmapMemory(
    *host_memory
  );

  // ストレージバッファ用のバッファを作る
  auto device_buffer = device->createBufferUnique(
    vk::BufferCreateInfo()
      .setSize( 1000 )
      .setUsage(
        vk::BufferUsageFlagBits::eTransferDst |
        vk::BufferUsageFlagBits::eStorageBuffer
      )
  );
  
  // バッファに必要なメモリのサイズを取得する
  auto device_buffer_memory_reqs = device->getBufferMemoryRequirements( *device_buffer );

  // デバイス側のメモリに置かれ、GPUしか触れないメモリを探す
  std::uint32_t device_heap_index = 0u;
  for( std::uint32_t i = 0u; i != memory_props.memoryHeapCount; ++i ) {
    if( memory_props.memoryHeaps[ i ].flags & vk::MemoryHeapFlagBits::eDeviceLocal ) {
      device_heap_index = i;
      break;
    }
  }
  std::uint32_t device_memory_index = 0u;
  for( std::uint32_t i = 0u; i != memory_props.memoryTypeCount; ++i ) {
    if( ( device_buffer_memory_reqs.memoryTypeBits >> i ) & 0x1 ) {
      if( memory_props.memoryTypes[ i ].propertyFlags & vk::MemoryPropertyFlagBits::eDeviceLocal ) {
        if( memory_props.memoryTypes[ i ].heapIndex == device_heap_index ) {
          device_memory_index = i;
          break;
        }
      }
    }
  }
  
  // ストレージバッファ用のメモリを確保する
  auto device_memory = device->allocateMemoryUnique(
    vk::MemoryAllocateInfo()
      .setAllocationSize( device_buffer_memory_reqs.size )
      .setMemoryTypeIndex( device_memory_index )
  );
  
  // メモリをストレージバッファ用のバッファに結びつける
  device->bindBufferMemory(
    *device_buffer,
    *device_memory,
    0u
  );

  // コマンドプールを作る
  auto command_pool = device->createCommandPoolUnique(
    vk::CommandPoolCreateInfo()
      .setQueueFamilyIndex( queue_family_index )
  );

  // コマンドバッファを作る
  auto command_buffers = device->allocateCommandBuffersUnique(
    vk::CommandBufferAllocateInfo()
      .setCommandPool( *command_pool )
      .setLevel( vk::CommandBufferLevel::ePrimary )
      .setCommandBufferCount( 1u )
  );
  auto command_buffer = std::move( command_buffers[ 0 ] );

  // コマンドバッファの記録を開始する
  command_buffer->begin(
    vk::CommandBufferBeginInfo()
      .setFlags( vk::CommandBufferUsageFlagBits::eOneTimeSubmit )
  );
  
  // ステージングバッファからストレージバッファへのコピー
  const std::vector< vk::BufferCopy > regions{
    vk::BufferCopy()
      .setSize( 1000 )
  };
  command_buffer->copyBuffer(
    *host_buffer,
    *device_buffer,
    regions
  );
  
  // コマンドバッファの記録を終了する
  command_buffer->end();

  // キューに流したコマンドの完了を待つためのフェンスを作る
  auto fence = device->createFenceUnique(
    vk::FenceCreateInfo()
  );

  // コマンドバッファの内容をキューに流す
  const std::vector< vk::SubmitInfo > submit_info{
    vk::SubmitInfo()
      .setCommandBufferCount( 1u )
      .setPCommandBuffers( &*command_buffer )
  };
  queue.submit(
    submit_info,
    *fence
  );

  // キューに流したコマンドの完了を待つ
  const std::vector< vk::Fence > fences{
    *fence
  };
  if( device->waitForFences( fences, true, 10000000000u ) != vk::Result::eSuccess ) abort();
}

