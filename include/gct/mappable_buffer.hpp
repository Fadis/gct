#ifndef GCT_MAPPABLE_BUFFER_HPP
#define GCT_MAPPABLE_BUFFER_HPP
#include <string>
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
#include <gct/buffer_create_info.hpp>
#include <gct/buffer.hpp>
namespace gct {
  struct device_t;
  struct allocator_t;
  class mappable_buffer_t : public std::enable_shared_from_this< mappable_buffer_t > {
  public:
    mappable_buffer_t(
      const std::shared_ptr< allocator_t >&,
      const buffer_create_info_t&,
      VmaAllocationCreateFlags flags = VmaAllocationCreateFlags( 0 )
    );
    mappable_buffer_t( const mappable_buffer_t& ) = delete;
    mappable_buffer_t( mappable_buffer_t&& ) = default;
    mappable_buffer_t &operator=( const mappable_buffer_t& ) = delete;
    mappable_buffer_t &operator=( mappable_buffer_t&& ) = default;
    const buffer_create_info_t &get_props() const { return get_buffer()->get_props(); }
    vk::BufferView &get_view() { return get_buffer()->get_view(); }
    const vk::BufferView &get_view() const { return get_buffer()->get_view(); }
    vk::Buffer &operator*() { return **get_buffer(); }
    const vk::Buffer &operator*() const { return **get_buffer(); }
    vk::Buffer *operator->() { return get_buffer()->operator->(); }
    const vk::Buffer *operator->() const { return get_buffer()->operator->(); }
    template< typename T >
    buffer_range_t< T > map() {
      return get_staging_buffer()->map< T >();
    }
    template< typename T >
    buffer_range_t< const T > map() const {
      return get_staging_buffer()->map< T >();
    }
    const std::shared_ptr< buffer_t > &get_staging_buffer() const {
      return staging;
    }
    const std::shared_ptr< buffer_t > &get_buffer() const {
      return device;
    }
    std::shared_ptr< buffer_view_t > get_view(
      const buffer_view_create_info_t &ci
    ) {
      return get_buffer()->get_view( ci );
    }
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
    std::shared_ptr< acceleration_structure_t > create_acceleration_structure(
      const acceleration_structure_create_info_t &ci
    ) {
      return get_buffer()->create_acceleration_structure( ci );
    }
    std::shared_ptr< acceleration_structure_t > create_acceleration_structure(
      const vk::AccelerationStructureTypeKHR &ci
    ) {
      return get_buffer()->create_acceleration_structure( ci );
    }
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)
    device_address_t get_address( const buffer_device_address_info_t &ci ) {
      return get_buffer()->get_address( ci );
    }
    device_address_t get_address() {
      return get_buffer()->get_address();
    }
#endif
    const std::shared_ptr< device_t > &get_device() const {
      return get_buffer()->get_device();
    }
  private:
    std::shared_ptr< buffer_t > staging;
    std::shared_ptr< buffer_t > device;
  };
  void to_json( nlohmann::json&, const mappable_buffer_t& );
}

#endif

