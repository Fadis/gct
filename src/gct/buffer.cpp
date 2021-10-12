#include <gct/allocator.hpp>
#include <gct/buffer_view.hpp>
#include <gct/buffer_view_create_info.hpp>
#include <gct/buffer_create_info.hpp>
#include <gct/buffer.hpp>

namespace gct {
  buffer_t::buffer_t(
    const std::shared_ptr< allocator_t > &allocator,
    const buffer_create_info_t &create_info,
    VmaMemoryUsage usage
  ) :
    created_from< allocator_t >( allocator ),
    props( create_info ),
    allocation( new VmaAllocation() ) {
    props.rebuild_chain();
    VmaAllocationCreateInfo buffer_alloc_info = {};
    VkBufferCreateInfo raw_buffer_create_info = props.get_basic();
    buffer_alloc_info.usage = usage;
    VkBuffer buffer_;
    const auto result = vmaCreateBuffer( **allocator, &raw_buffer_create_info, &buffer_alloc_info, &buffer_, allocation.get(), nullptr );
    if( result != VK_SUCCESS ) vk::throwResultException( vk::Result( result ), "バッファを作成できない" );
    handle.reset(
      new vk::Buffer( buffer_ ),
      [allocator=allocator,allocation=allocation]( vk::Buffer *p ) {
        if( p ) {
          vmaDestroyBuffer( **allocator, *p, *allocation );
          delete p;
        }
      }
    );
  }
  /*buffer_range_t< std::uint8_t > buffer_t::map() {
    void* mapped_memory;
    const auto result = vmaMapMemory( **get_factory(), *allocation, &mapped_memory );
    if( result != VK_SUCCESS ) vk::throwResultException( vk::Result( result ), "バッファをマップできない" );
    const auto length = props.get_basic().size;
    return buffer_range_t< std::uint8_t >(
      std::shared_ptr< std::uint8_t >(
        reinterpret_cast< std::uint8_t* >( mapped_memory ),
        [allocator=get_factory(),allocation=allocation,handle=handle]( std::uint8_t *p ) {
          if( p ) vmaUnmapMemory( **allocator, *allocation );
        }
      ),
      length
    );
  }
  buffer_range_t< const std::uint8_t > buffer_t::map() const {
    void* mapped_memory;
    const auto result = vmaMapMemory( **get_factory(), *allocation, &mapped_memory );
    if( result != VK_SUCCESS ) vk::throwResultException( vk::Result( result ), "バッファをマップできない" );
    const auto length = props.get_basic().size;
    return buffer_range_t< const std::uint8_t >(
      std::shared_ptr< const std::uint8_t >(
        reinterpret_cast< std::uint8_t* >( mapped_memory ),
        [allocator=get_factory(),allocation=allocation,handle=handle]( std::uint8_t *p ) {
          if( p ) vmaUnmapMemory( **allocator, *allocation );
        }
      ),
      length
    );
  }*/
  std::shared_ptr< buffer_view_t > buffer_t::get_view(
    const buffer_view_create_info_t &create_info
  ) {
    return std::shared_ptr< buffer_view_t >(
      new buffer_view_t(
        shared_from_this(),
        create_info
      )
    );
  }
  const std::shared_ptr< device_t > &buffer_t::get_device() const {
    return get_factory()->get_factory();
  }
  void *buffer_t::map_raw() const {
    void* mapped_memory;
    const auto result = vmaMapMemory( **get_factory(), *allocation, &mapped_memory );
    if( result != VK_SUCCESS ) vk::throwResultException( vk::Result( result ), "バッファをマップできない" );
    return mapped_memory;
  }
  void unmap_memory( const std::shared_ptr< allocator_t > &allocator, const std::shared_ptr< VmaAllocation > &allocation ) {
    vmaUnmapMemory( **allocator, *allocation );
  }
}

