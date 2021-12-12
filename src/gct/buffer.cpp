#include <gct/allocator.hpp>
#include <gct/buffer_view.hpp>
#include <gct/buffer_view_create_info.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <gct/acceleration_structure.hpp>
#include <gct/acceleration_structure_create_info.hpp>
#endif
#include <gct/buffer_create_info.hpp>
#include <gct/buffer.hpp>
#include <gct/device.hpp>
#include <gct/buffer_device_address_info.hpp>
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
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
    std::shared_ptr< acceleration_structure_t > buffer_t::create_acceleration_structure(
      const acceleration_structure_create_info_t &create_info
    ) {
      return std::shared_ptr< acceleration_structure_t >(
        new acceleration_structure_t(
          shared_from_this(),
          create_info
        )
      );
    }
    std::shared_ptr< acceleration_structure_t > buffer_t::create_acceleration_structure(
      const vk::AccelerationStructureTypeKHR &type
    ) {
      return create_acceleration_structure(
        acceleration_structure_create_info_t()
          .set_basic(
            vk::AccelerationStructureCreateInfoKHR()
              .setOffset( 0 )
              .setSize( get_props().get_basic().size )
              .setType( type )
          )
      );
    }
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)
    device_address_t buffer_t::get_address( const buffer_device_address_info_t &info ) {
      auto copied = info;
      copied.set_buffer( shared_from_this() ).rebuild_chain();
      return device_address_t(
        shared_from_this(),
        (*get_device())->getBufferAddress(
          copied.get_basic()
        )
      );
    }
    device_address_t buffer_t::get_address() {
      return get_address(
        buffer_device_address_info_t()
      );
    }
#endif
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
  void to_json( nlohmann::json &root, const buffer_t &v ) {
    root = nlohmann::json::object();
    root[ "props" ] = v.get_props();
  }
}

