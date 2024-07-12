#include <gct/allocator.hpp>
#include <gct/get_device.hpp>
#include <gct/device.hpp>
#include <gct/buffer.hpp>
#include <gct/voxel_buffer.hpp>

namespace gct {
voxel_buffer::voxel_buffer(
  const std::shared_ptr< allocator_t > &allocator,
  unsigned int size_factor,
  unsigned int data_size
) {
  const std::uint32_t size = 1u << size_factor;
  buf = allocator->create_buffer(
    size * size * size * data_size,
    vk::BufferUsageFlagBits::eStorageBuffer|
    vk::BufferUsageFlagBits::eTransferSrc|
    vk::BufferUsageFlagBits::eTransferDst,
    VMA_MEMORY_USAGE_GPU_ONLY
  );
}

}

