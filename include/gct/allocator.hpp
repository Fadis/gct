#ifndef GCT_ALLOCATOR_HPP
#define GCT_ALLOCATOR_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wreorder"
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#include <gct/vk_mem_alloc.h>
#pragma GCC diagnostic pop
#include <gct/created_from.hpp>
#include <gct/buffer.hpp>
#include <gct/image.hpp>
#include <gct/vk_mem_alloc.h>

namespace gct {
  class device_t;
  class image_create_info_t;
  class allocator_t : public created_from< device_t >, public std::enable_shared_from_this< allocator_t > {
  public:
    allocator_t(
      const std::shared_ptr< device_t > &device,
      const VmaAllocatorCreateInfo &create_info
    );
    allocator_t( const allocator_t& ) = delete;
    allocator_t( allocator_t&& ) = default;
    allocator_t &operator=( const allocator_t& ) = delete;
    allocator_t &operator=( allocator_t&& ) = default;
    VmaAllocator &operator*() { return *handle; }
    const VmaAllocator &operator*() const { return *handle; }
    std::shared_ptr< image_t > create_image(
      const image_create_info_t&,
      VmaMemoryUsage usage
    );
    std::shared_ptr< buffer_t > create_buffer(
      const buffer_create_info_t&,
      VmaMemoryUsage usage
    );
    std::shared_ptr< buffer_t > create_buffer(
      std::size_t size,
      vk::BufferUsageFlags,
      VmaMemoryUsage usage
    );
    const VmaAllocatorCreateInfo &get_props() const { return props; }
  private:
    VmaAllocatorCreateInfo props;
    std::shared_ptr< VmaAllocator > handle;
  };
}

#endif

