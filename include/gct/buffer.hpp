#ifndef GCT_BUFFER_HPP
#define GCT_BUFFER_HPP
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
#include <gct/device_address.hpp>
#include <gct/property.hpp>
namespace gct {
  template< typename T >
  class buffer_range_t {
  public:
    using value_type = T;
    using reference = value_type&;
    using pointer = value_type*;
    using iterator = value_type*;
    using const_iterator = const value_type*;
    using const_reference = const value_type&;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    buffer_range_t(
      const std::shared_ptr< value_type > &head_,
      size_type length_
    ) :
      head( head_ ),
      length( length_ ) {}
    [[nodiscard]] iterator begin() { return head.get(); }
    [[nodiscard]] iterator end() { return std::next( head.get(), length ); }
    [[nodiscard]] const_iterator begin() const { return head.get(); }
    [[nodiscard]] const_iterator end() const { return std::next( head.get(), length ); }
    [[nodiscard]] size_type size() const { return length; }
    [[nodiscard]] bool empty() const { return length == 0u; }
    [[nodiscard]] reference operator[]( int n ) {
      return head.get()[ n ];
    }
    [[nodiscard]] const_reference operator[]( int n ) const {
      return head.get()[ n ];
    }
    [[nodiscard]] T& operator*() {
      return *head.get();
    }
    [[nodiscard]] const T& operator*() const {
      return *head.get();
    }
    [[nodiscard]] T& operator->() {
      return *head.get();
    }
    [[nodiscard]] const T& operator->() const {
      return *head.get();
    }
  private:
    std::shared_ptr< value_type > head;
    size_type length;
  };


  struct device_t;
  struct allocator_t;
  struct buffer_view_t;
  struct buffer_view_create_info_t;
#if defined(VK_VERSION_1_2) || defined(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)
  struct buffer_device_address_info_t;
#endif
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
  struct acceleration_structure_t;
  struct acceleration_structure_create_info_t;
#endif
  void unmap_memory( const std::shared_ptr< allocator_t >&, const std::shared_ptr< VmaAllocation >& );
  class buffer_t :
    public property< buffer_create_info_t >,
    public created_from< allocator_t >,
    public std::enable_shared_from_this< buffer_t > {
  public:
    buffer_t(
      const std::shared_ptr< allocator_t >&,
      const buffer_create_info_t&,
      VmaMemoryUsage usage,
      VmaAllocationCreateFlags flags = VmaAllocationCreateFlags( 0 )
    );
    buffer_t( const buffer_t& ) = delete;
    buffer_t( buffer_t&& ) = default;
    buffer_t &operator=( const buffer_t& ) = delete;
    buffer_t &operator=( buffer_t&& ) = default;
    [[nodiscard]] vk::BufferView &get_view() { return *buffer_view; }
    [[nodiscard]] const vk::BufferView &get_view() const { return *buffer_view; }
    [[nodiscard]] vk::Buffer &operator*() {
      return *handle;
    }
    [[nodiscard]] const vk::Buffer &operator*() const {
      return *handle;
    }
    [[nodiscard]] vk::Buffer *operator->() {
      return handle.get();
    }
    [[nodiscard]] const vk::Buffer *operator->() const {
      return handle.get();
    }
    //buffer_range_t< std::uint8_t > map();
    //buffer_range_t< const std::uint8_t > map() const;
    template< typename T >
    [[nodiscard]] buffer_range_t< T > map() {
      void* mapped_memory = map_raw();
      const auto length = props.get_basic().size;
      return buffer_range_t< T >(
        std::shared_ptr< T >(
          reinterpret_cast< T* >( mapped_memory ),
          [allocator=get_factory(),allocation=allocation,handle=handle]( auto *p ) {
            if( p ) unmap_memory( allocator, allocation );
          }
        ),
        length / sizeof( T )
      );
    }
    template< typename T >
    [[nodiscard]] buffer_range_t< const T > map() const {
      void* mapped_memory = map_raw();
      const auto length = props.get_basic().size;
      return buffer_range_t< const T >(
        std::shared_ptr< const T >(
          reinterpret_cast< T* >( mapped_memory ),
          [allocator=get_factory(),allocation=allocation,handle=handle]( auto *p ) {
            if( p ) unmap_memory( allocator, allocation );
          }
        ),
        length / sizeof( T )
      );
    }
    [[nodiscard]] std::shared_ptr< buffer_view_t > get_view(
      const buffer_view_create_info_t&
    );
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
    [[nodiscard]] std::shared_ptr< acceleration_structure_t > create_acceleration_structure(
      const acceleration_structure_create_info_t&
    ); 
    [[nodiscard]] std::shared_ptr< acceleration_structure_t > create_acceleration_structure(
      const vk::AccelerationStructureTypeKHR&
    );
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)
    [[nodiscard]] device_address_t get_address( const buffer_device_address_info_t& );
    [[nodiscard]] device_address_t get_address();
#endif
    [[nodiscard]] const std::shared_ptr< device_t > &get_device() const;
  private:
    [[nodiscard]] void *map_raw() const;
    std::shared_ptr< vk::Buffer > handle;
    std::shared_ptr< VmaAllocation > allocation;
    vk::UniqueHandle< vk::BufferView, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > buffer_view;
  };
  void to_json( nlohmann::json&, const buffer_t& );
  class pixel_buffer_t : public buffer_t {
  public:
    pixel_buffer_t(
      const std::shared_ptr< allocator_t >&,
      const buffer_create_info_t&,
      VmaMemoryUsage usage,
      const vk::Extent3D&,
      vk::Format
    );
    void dump_image(
      const std::string &filename
    );
    [[nodiscard]] const vk::Extent3D &get_extent() const { return extent; }
    [[nodiscard]] vk::Format get_format() const { return format; }
  private:
    vk::Extent3D extent;
    vk::Format format;
  };
}

#endif

