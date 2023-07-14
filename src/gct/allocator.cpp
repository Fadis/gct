#include <gct/instance.hpp>
#include <gct/device.hpp>
#include <gct/allocated_image.hpp>
#include <gct/allocator.hpp>
#include <gct/format.hpp>
#include <OpenImageIO/imageio.h>
#include <OpenImageIO/version.h>

namespace gct {
  allocator_t::allocator_t(
    const std::shared_ptr< device_t > &device,
    const VmaAllocatorCreateInfo &create_info
  ) :
    created_from< device_t >( device ), props( create_info ) {
    props.instance = VkInstance( **device->get_factory() );
    props.physicalDevice = VkPhysicalDevice( **device->get_physical_device_group().devices[ 0 ] );
    props.device = VkDevice( **device );
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
  std::shared_ptr< buffer_t > allocator_t::create_buffer(
    const buffer_create_info_t &create_info,
    VmaMemoryUsage usage,
    VmaAllocationCreateFlags flags
  ) {
    return std::shared_ptr< buffer_t >(
      new buffer_t(
        shared_from_this(),
        create_info,
        usage,
        flags
      )
    );
  }
  std::shared_ptr< buffer_t > allocator_t::create_buffer(
    std::size_t size,
    vk::BufferUsageFlags buffer_usage,
    VmaMemoryUsage usage,
    VmaAllocationCreateFlags flags
  ) {
    return create_buffer(
      buffer_create_info_t()
        .set_basic(
          vk::BufferCreateInfo()
            .setSize( size )
            .setUsage( buffer_usage )
        ),
      usage,
      flags
    );
  }
  std::shared_ptr< pixel_buffer_t > allocator_t::create_pixel_buffer(
    const buffer_create_info_t &create_info,
    VmaMemoryUsage usage,
    const vk::Extent3D &extent,
    vk::Format format
  ) {
    return std::shared_ptr< pixel_buffer_t >(
      new pixel_buffer_t(
        shared_from_this(),
        create_info,
        usage,
        extent,
        format
      )
    );
  }
  std::shared_ptr< pixel_buffer_t > allocator_t::create_pixel_buffer(
    vk::BufferUsageFlags buffer_usage,
    VmaMemoryUsage usage,
    const vk::Extent3D &extent,
    vk::Format format
  ) {
    const auto ps = format_to_size( format );
    const auto ppex = pixel_per_element_x( format );
    const auto ppey = pixel_per_element_y( format );
    return create_pixel_buffer(
      buffer_create_info_t()
        .set_basic(
          vk::BufferCreateInfo()
            .setSize( ( extent.width / ppex ) * ( extent.height / ppey ) * extent.depth * ps )
            .setUsage( buffer_usage )
        ),
      usage,
      extent,
      format
    );
  }

  vk::Format channels_to_format( unsigned int c, bool srgb ) {
    if( srgb ) {
      if( c == 1 ) return vk::Format::eR8Srgb;
      else if( c == 2 ) return vk::Format::eR8G8Srgb;
      else if( c == 3 ) return vk::Format::eR8G8B8A8Srgb;
      else if( c == 4 ) return vk::Format::eR8G8B8A8Srgb;
      return vk::Format::eUndefined;
    }
    else {
      if( c == 1 ) return vk::Format::eR8Unorm;
      else if( c == 2 ) return vk::Format::eR8G8Unorm;
      else if( c == 3 ) return vk::Format::eR8G8B8A8Unorm;
      else if( c == 4 ) return vk::Format::eR8G8B8A8Unorm;
      return vk::Format::eUndefined;
    }
  }

  std::shared_ptr< pixel_buffer_t > allocator_t::load_image(
    const std::string &filename,
    bool srgb
  ) {
    using namespace OIIO_NAMESPACE;
#if OIIO_VERSION_MAJOR >= 2 
    auto texture_file = ImageInput::open( filename );
#else
    std::shared_ptr< ImageInput > texture_file(
      ImageInput::open( filename ),
      []( auto p ) { if( p ) ImageInput::destroy( p ); }
    );
#endif
    if( !texture_file ) throw -1;
    const ImageSpec &spec = texture_file->spec();
    auto temporary_buffer = create_pixel_buffer(
      vk::BufferUsageFlagBits::eTransferSrc,
      VMA_MEMORY_USAGE_CPU_TO_GPU,
      vk::Extent3D{ uint32_t( spec.width ), uint32_t( spec.height ), 1u },
      channels_to_format( spec.nchannels, srgb )
    );
    {
      auto mapped = temporary_buffer->map< std::uint8_t >();
      if( spec.nchannels == 3 ) {
        std::vector< uint8_t > temp( spec.width * spec.height * 4u );
        texture_file->read_image( TypeDesc::UINT8, temp.data() );
        for( size_t i = spec.width * spec.height - 1; i; --i ) {
          temp[ i * 4 ] = temp[ i * spec.nchannels ];
          temp[ i * 4 + 1 ] = temp[ i * spec.nchannels + 1 ];
          temp[ i * 4 + 2 ] = temp[ i * spec.nchannels + 2 ];
          temp[ i * 4 + 3 ] = 255u;
        }
        std::copy( temp.begin(), temp.end(), mapped.begin() );
      }
      else {
        texture_file->read_image( TypeDesc::UINT8, mapped.begin() );
      }
    }
    return temporary_buffer;
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

