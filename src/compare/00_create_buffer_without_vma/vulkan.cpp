#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>
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

  uint32_t queue_props_count = 0u;
  vkGetPhysicalDeviceQueueFamilyProperties( **physical_device, &queue_props_count, nullptr );
  std::vector< VkQueueFamilyProperties > queue_props( queue_props_count );
  vkGetPhysicalDeviceQueueFamilyProperties( **physical_device, &queue_props_count, queue_props.data() );
  uint32_t queue_family_index = 0u;
  for( uint32_t i = 0; i < queue_props.size(); ++i ) {
    if( queue_props[ i ].queueFlags & VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT ) {
      queue_family_index = i;
      break;
    }
  }

  const float priority = 0.0f;
  VkDeviceQueueCreateInfo queue_create_info;
  queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queue_create_info.pNext = nullptr;
  queue_create_info.flags = 0;
  queue_create_info.queueFamilyIndex = queue_family_index;
  queue_create_info.queueCount = 1;
  queue_create_info.pQueuePriorities = &priority;

  std::vector< const char* > extension{
    VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
  };

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
  
  VkQueue queue;
  vkGetDeviceQueue(
    device,
    queue_family_index,
    0u,
    &queue
  );
  
  // ステージングバッファ用のバッファを作る
  VkBufferCreateInfo host_buffer_create_info;
  host_buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  host_buffer_create_info.pNext = nullptr;
  host_buffer_create_info.flags = 0;
  host_buffer_create_info.size = 1000;
  host_buffer_create_info.usage = VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
  host_buffer_create_info.sharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
  host_buffer_create_info.queueFamilyIndexCount = 0;
  host_buffer_create_info.pQueueFamilyIndices = nullptr;
  VkBuffer host_buffer;
  if( vkCreateBuffer(
    device,
    &host_buffer_create_info,
    nullptr,
    &host_buffer
  ) != VK_SUCCESS ) abort();

  // バッファに必要なメモリの要件を調べる
  VkMemoryRequirements host_buffer_memory_reqs;
  vkGetBufferMemoryRequirements(
    device,
    host_buffer,
    &host_buffer_memory_reqs
  );

  /////
  VkPhysicalDeviceMemoryProperties memory_props;
  vkGetPhysicalDeviceMemoryProperties( **physical_device, &memory_props );
  // ホスト側のメモリに置かれ、CPUから読み書きできるメモリを探す
  std::uint32_t host_heap_index = 0u;
  for( std::uint32_t i = 0u; i != memory_props.memoryHeapCount; ++i ) {
    if( !( memory_props.memoryHeaps[ i ].flags & VkMemoryHeapFlagBits::VK_MEMORY_HEAP_DEVICE_LOCAL_BIT ) ) {
      host_heap_index = i;
      break;
    }
  }
  std::uint32_t host_memory_index = 0u;
  for( std::uint32_t i = 0u; i != memory_props.memoryTypeCount; ++i ) {
    if( memory_props.memoryTypes[ i ].propertyFlags & VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) {
      if( ( host_buffer_memory_reqs.memoryTypeBits >> i ) & 0x1 ) {
        if( memory_props.memoryTypes[ i ].heapIndex == host_heap_index ) {
          host_memory_index = i;
          break;
        }
      }
    }
  }
  
  // ステージングバッファ用のメモリを確保する
  VkMemoryAllocateInfo host_memory_allocate_info;
  host_memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  host_memory_allocate_info.pNext = nullptr;
  host_memory_allocate_info.allocationSize = host_buffer_memory_reqs.size;
  host_memory_allocate_info.memoryTypeIndex = host_memory_index;
  VkDeviceMemory host_memory;
  if( vkAllocateMemory(
    device,
    &host_memory_allocate_info,
    nullptr,
    &host_memory
  ) != VK_SUCCESS ) abort();

  // メモリをステージングバッファ用のバッファに結びつける
  if( vkBindBufferMemory(
    device,
    host_buffer,
    host_memory,
    0u
  ) != VK_SUCCESS ) abort();
  
  // ステージングバッファ用のメモリをプロセスのアドレス空間にマップする
  void *mapped = nullptr;
  if( vkMapMemory(
    device,
    host_memory,
    0u,
    host_buffer_memory_reqs.size,
    0u,
    &mapped
  ) != VK_SUCCESS ) abort();

  // ステージングバッファ用のメモリに値を書き込む
  std::fill(
    reinterpret_cast< std::uint32_t* >( mapped ),
    reinterpret_cast< std::uint32_t* >( mapped ) + 1000 / sizeof( std::uint32_t ),
    1u
  );

  // ステージングバッファ用のメモリをプロセスのアドレス空間から外す
  vkUnmapMemory(
    device,
    host_memory
  );

  // ストレージバッファ用のバッファを作る
  VkBufferCreateInfo device_buffer_create_info;
  device_buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  device_buffer_create_info.pNext = nullptr;
  device_buffer_create_info.flags = 0;
  device_buffer_create_info.size = 1000;
  device_buffer_create_info.usage =
    VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_DST_BIT |
    VkBufferUsageFlagBits::VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
  device_buffer_create_info.sharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
  device_buffer_create_info.queueFamilyIndexCount = 0;
  device_buffer_create_info.pQueueFamilyIndices = nullptr;
  VkBuffer device_buffer;
  if( vkCreateBuffer(
    device,
    &device_buffer_create_info,
    nullptr,
    &device_buffer
  ) != VK_SUCCESS ) abort();
  
  // バッファに必要なメモリのサイズを取得する
  VkMemoryRequirements device_buffer_memory_reqs;
  vkGetBufferMemoryRequirements(
    device,
    device_buffer,
    &device_buffer_memory_reqs
  );

  // デバイス側のメモリに置かれ、GPUしか触れないメモリを探す
  std::uint32_t device_heap_index = 0u;
  for( std::uint32_t i = 0u; i != memory_props.memoryHeapCount; ++i ) {
    if( memory_props.memoryHeaps[ i ].flags & VkMemoryHeapFlagBits::VK_MEMORY_HEAP_DEVICE_LOCAL_BIT ) {
      device_heap_index = i;
      break;
    }
  }
  std::uint32_t device_memory_index = 0u;
  for( std::uint32_t i = 0u; i != memory_props.memoryTypeCount; ++i ) {
    if( ( device_buffer_memory_reqs.memoryTypeBits >> i ) & 0x1 ) {
      if( memory_props.memoryTypes[ i ].propertyFlags & VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) {
        if( memory_props.memoryTypes[ i ].heapIndex == device_heap_index ) {
          device_memory_index = i;
          break;
        }
      }
    }
  }
  
  // ストレージバッファ用のメモリを確保する
  VkMemoryAllocateInfo device_memory_allocate_info;
  device_memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  device_memory_allocate_info.pNext = nullptr;
  device_memory_allocate_info.allocationSize = device_buffer_memory_reqs.size;
  device_memory_allocate_info.memoryTypeIndex = device_memory_index;
  VkDeviceMemory device_memory;
  if( vkAllocateMemory(
    device,
    &device_memory_allocate_info,
    nullptr,
    &device_memory
  ) != VK_SUCCESS ) abort();

  // メモリをストレージバッファ用のバッファに結びつける
  if( vkBindBufferMemory(
    device,
    device_buffer,
    device_memory,
    0u
  ) != VK_SUCCESS ) abort();

  // コマンドプールを作る
  VkCommandPoolCreateInfo command_pool_create_info;
  command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  command_pool_create_info.pNext = nullptr;
  command_pool_create_info.flags = 0u;
  command_pool_create_info.queueFamilyIndex = queue_family_index;
  VkCommandPool command_pool;
  if( vkCreateCommandPool(
    device,
    &command_pool_create_info,
    nullptr,
    &command_pool
  ) != VK_SUCCESS ) abort();

  // コマンドバッファを作る
  VkCommandBufferAllocateInfo command_buffer_allocate_info;
  command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  command_buffer_allocate_info.pNext = nullptr;
  command_buffer_allocate_info.commandPool = command_pool;
  command_buffer_allocate_info.level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  command_buffer_allocate_info.commandBufferCount = 1u;
  VkCommandBuffer command_buffer;
  if( vkAllocateCommandBuffers(
    device,
    &command_buffer_allocate_info,
    &command_buffer
  ) != VK_SUCCESS ) abort();

  // コマンドバッファの記録を開始する
  VkCommandBufferBeginInfo command_buffer_begin_info;
  command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  command_buffer_begin_info.pNext = nullptr;
  command_buffer_begin_info.flags = VkCommandBufferUsageFlagBits::VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  command_buffer_begin_info.pInheritanceInfo = VK_NULL_HANDLE;
  if( vkBeginCommandBuffer(
    command_buffer,
    &command_buffer_begin_info
  ) != VK_SUCCESS ) abort();
  
  // ステージングバッファからストレージバッファへのコピー
  VkBufferCopy region;
  region.srcOffset = 0u;
  region.dstOffset = 0u;
  region.size = 1000u;
  vkCmdCopyBuffer(
    command_buffer,
    host_buffer,
    device_buffer,
    1u,
    &region
  );
  
  // コマンドバッファの記録を終了する
  vkEndCommandBuffer( command_buffer );

  // キューに流したコマンドの完了を待つためのフェンスを作る
  VkFenceCreateInfo fence_create_info;
  fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fence_create_info.pNext = nullptr;
  fence_create_info.flags = 0u;
  VkFence fence;
  if( vkCreateFence(
    device,
    &fence_create_info,
    nullptr,
    &fence
  ) != VK_SUCCESS ) abort();

  // コマンドバッファの内容をキューに流す
  VkSubmitInfo submit_info;
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submit_info.pNext = nullptr;
  submit_info.waitSemaphoreCount = 0u;
  submit_info.pWaitSemaphores = nullptr;
  submit_info.pWaitDstStageMask = nullptr;
  submit_info.commandBufferCount = 1u;
  submit_info.pCommandBuffers = &command_buffer;
  submit_info.signalSemaphoreCount = 0u;
  submit_info.pSignalSemaphores = nullptr;
  if( vkQueueSubmit(
    queue,
    1u,
    &submit_info,
    fence
  ) != VK_SUCCESS ) abort();

  // キューに流したコマンドの完了を待つ
  if( vkWaitForFences(
    device,
    1u,
    &fence,
    true,
    10000000000u
  ) != VK_SUCCESS ) abort();

  // フェンスを捨てる
  vkDestroyFence( device, fence, nullptr );
  // コマンドバッファを捨てる
  vkFreeCommandBuffers( device, command_pool, 1u, &command_buffer );
  // コマンドプールを捨てる
  vkDestroyCommandPool( device, command_pool, nullptr );
  // ストレージバッファを捨てる
  vkDestroyBuffer( device, device_buffer, nullptr );
  // ストレージバッファ用のメモリを捨てる
  vkFreeMemory( device, device_memory, nullptr );
  // ステージングバッファを捨てる
  vkDestroyBuffer( device, host_buffer, nullptr );
  // ステージングバッファ用のメモリを捨てる
  vkFreeMemory( device, host_memory, nullptr );
  // デバイスを捨てる
  vkDestroyDevice( device, nullptr );
}

