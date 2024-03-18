#ifndef GCT_ALLOCATOR_HPP
#define GCT_ALLOCATOR_HPP
#include <string>
#include <memory>
#include <nlohmann/json_fwd.hpp>
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
#include <gct/image_create_info.hpp>
#include <gct/buffer_create_info.hpp>
#include <gct/vk_mem_alloc.h>

namespace gct {
  class device_t;
  class image_create_info_t;
  class image_t;
  class image_view_t;
  class buffer_t;
  class pixel_buffer_t;
  class mappable_buffer_t;
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
    std::vector< std::shared_ptr< image_view_t > > create_image_views(
      const image_create_info_t&,
      VmaMemoryUsage usage,
      unsigned int count
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
    std::shared_ptr< buffer_t > create_buffer(
      const buffer_create_info_t&,
      VmaMemoryUsage usage,
      VmaAllocationCreateFlags flags
    );
    std::shared_ptr< buffer_t > create_buffer(
      std::size_t size,
      vk::BufferUsageFlags,
      VmaMemoryUsage usage,
      VmaAllocationCreateFlags flags
    );
    std::shared_ptr< mappable_buffer_t > create_mappable_buffer(
      const buffer_create_info_t&
    );
    std::shared_ptr< mappable_buffer_t > create_mappable_buffer(
      std::size_t size,
      vk::BufferUsageFlags
    );
    std::shared_ptr< mappable_buffer_t > create_mappable_buffer(
      const buffer_create_info_t&,
      VmaAllocationCreateFlags flags
    );
    std::shared_ptr< mappable_buffer_t > create_mappable_buffer(
      std::size_t size,
      vk::BufferUsageFlags,
      VmaAllocationCreateFlags flags
    );
    std::shared_ptr< pixel_buffer_t > create_pixel_buffer(
      const buffer_create_info_t &create_info,
      VmaMemoryUsage usage,
      const vk::Extent3D &extent,
      vk::Format format
    );
    std::shared_ptr< pixel_buffer_t > create_pixel_buffer(
      vk::BufferUsageFlags buffer_usage,
      VmaMemoryUsage usage,
      const vk::Extent3D &extent,
      vk::Format format
    );
    std::shared_ptr< pixel_buffer_t > load_image(
      const std::string &filename,
      bool srgb
    );
    const VmaAllocatorCreateInfo &get_props() const { return props; }
  private:
    VmaAllocatorCreateInfo props;
    std::shared_ptr< VmaAllocator > handle;
  };
  void to_json( nlohmann::json &dest, const allocator_t &src );
}

void to_json( nlohmann::json &dest, const VmaAllocatorCreateInfo &src );

#endif

