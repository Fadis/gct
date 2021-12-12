#include <gct/instance.hpp>
#include <gct/device.hpp>
#include <gct/allocated_image.hpp>
#include <gct/allocator.hpp>

namespace gct {
  allocator_t::allocator_t(
    const std::shared_ptr< device_t > &device,
    const VmaAllocatorCreateInfo &create_info
  ) :
    created_from< device_t >( device ), props( create_info ) {
    props.instance = **device->get_factory();
    props.physicalDevice = **device->get_physical_device_group().devices[ 0 ];
    props.device = **device;
    VmaAllocator allocator;
    {
      const auto result = vmaCreateAllocator( &props, &allocator );
      if( result != VK_SUCCESS ) vk::throwResultException( vk::Result( result ), "アロケータを作成できない" );
    }
    handle.reset(
      new VmaAllocator( allocator ),
      []( VmaAllocator *p ) {
        if( p ) {
          vmaDestroyAllocator( *p );
          delete p;
        }
      }
    );
  }
  
  std::shared_ptr< image_t > allocator_t::create_image(
    const image_create_info_t &create_info,
    VmaMemoryUsage usage
  ) {
    return std::shared_ptr< allocated_image_t >(
      new allocated_image_t(
        shared_from_this(),
        create_info,
        usage
      )
    );
  }
  
  std::shared_ptr< buffer_t > allocator_t::create_buffer(
    const buffer_create_info_t &create_info,
    VmaMemoryUsage usage
  ) {
    return std::shared_ptr< buffer_t >(
      new buffer_t(
        shared_from_this(),
        create_info,
        usage
      )
    );
  }
  std::shared_ptr< buffer_t > allocator_t::create_buffer(
    std::size_t size,
    vk::BufferUsageFlags buffer_usage,
    VmaMemoryUsage usage
  ) {
    return create_buffer(
      buffer_create_info_t()
        .set_basic(
          vk::BufferCreateInfo()
            .setSize( size )
            .setUsage( buffer_usage )
        ),
      usage
    );
  }
  
  /*std::shared_ptr< image_t > allocator_t::get_image(
    const image_create_info_t &image_create_info,
    VmaMemoryUsage usage
  ) {
    image_t image;
    const std::shared_ptr< VmaAllocation > allocation( new VmaAllocation() );
    VmaAllocationCreateInfo image_alloc_info = {};
    VkImageCreateInfo raw_image_create_info = image_create_info;
    image_alloc_info.usage = usage;
    VkImage image_;
    const auto result = vmaCreateImage( *handle, &raw_image_create_info, &image_alloc_info.basic, &image_, allocation.get(), nullptr );
    if( result != VK_SUCCESS ) vk::throwResultException( vk::Result( result ), "イメージを作成できない" );
    image.set_allocation( allocation );
    image.emplace_image(
      new vk::Image( image_ ),
      [allocator=context.allocator,allocation]( vk::Image *p ) {
        if( p ) {
          vmaDestroyImage( *allocator, *p, *allocation );
          delete p;
        }
      }
    );
    image.set_width( image_create_info.extent.width );
    image.set_height( image_create_info.extent.height );
    image.set_format( image_create_info.format );
    return image;
  }*/
}

