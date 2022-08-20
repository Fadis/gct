#include <gct/buffer.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <OpenImageIO/imageio.h>
#include <OpenImageIO/version.h>
#include <gct/device.hpp>
#include <gct/queue.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/get_device.hpp>
#include <gct/astc.hpp>
#include <gct/format.hpp>
#include <gct/mmaped_file.hpp>
namespace gct {
  std::shared_ptr< image_t > command_buffer_recorder_t::load_astc(
    const std::shared_ptr< allocator_t > &allocator,
    const std::vector< std::string > &filename,
    vk::ImageUsageFlagBits usage,
    bool srgb
  ) {
    const auto temp = mmaped_file( *filename.begin() );
    const auto header = reinterpret_cast< const astc_header* >( temp.begin() );
    const auto [width,height] = get_dims( *header );
    const auto format = get_format( *header, srgb );

    std::shared_ptr< image_t > final_image;
    final_image = allocator->create_image(
      image_create_info_t()
        .set_basic(
          vk::ImageCreateInfo()
            .setImageType( vk::ImageType::e2D )
            .setFormat( format )
            .setExtent( { width, height, 1 } )
            .setMipLevels( filename.size() )
            .setArrayLayers( 1 )
            .setSamples( vk::SampleCountFlagBits::e1 )
            .setTiling( vk::ImageTiling::eOptimal )
            .setUsage( usage | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled )
            .setSharingMode( vk::SharingMode::eExclusive )
            .setQueueFamilyIndexCount( 0 )
            .setPQueueFamilyIndices( nullptr )
            .setInitialLayout( vk::ImageLayout::eUndefined )
        ),
        VMA_MEMORY_USAGE_GPU_ONLY
    );
    for( unsigned int mip_level = 0u; mip_level != filename.size(); ++mip_level ) {
      const auto data = mmaped_file( filename[ mip_level ] );
      const auto header = reinterpret_cast< const astc_header* >( data.begin() );
      const auto [local_width,local_height] = get_dims( *header );
      if( local_width != ( width >> mip_level ) ) throw -1;
      if( local_height != ( height >> mip_level ) ) throw -1;
      const auto local_format = get_format( *header, srgb );
      if( local_format != format ) throw -1;
      const auto size = get_size( *header );
      auto temporary_buffer = allocator->create_buffer(
        size,
        vk::BufferUsageFlagBits::eTransferSrc,
        VMA_MEMORY_USAGE_CPU_TO_GPU
      );
      {
        auto mapped = temporary_buffer->map< std::uint8_t >();
        std::copy( data.begin(), data.end(), mapped.begin() );
      }
      copy(
        temporary_buffer,
        final_image,
        vk::BufferImageCopy()
          .setBufferOffset( 0 )
          .setBufferRowLength( final_image->get_props().get_basic().extent.width >> mip_level )
          .setBufferImageHeight( final_image->get_props().get_basic().extent.height >> mip_level )
          .setImageSubresource(
            vk::ImageSubresourceLayers()
              .setAspectMask( format_to_aspect( final_image->get_props().get_basic().format ) )
              .setMipLevel( mip_level )
              .setBaseArrayLayer( 0 )
              .setLayerCount( 1 )
          )
          .setImageOffset(
            vk::Offset3D()
              .setX( 0 )
              .setY( 0 )
              .setZ( 0 )
          )
          .setImageExtent(
            vk::Extent3D()
              .setWidth( final_image->get_props().get_basic().extent.width >> mip_level )
              .setHeight( final_image->get_props().get_basic().extent.height >> mip_level )
              .setDepth( 1 )
          ),
        vk::ImageLayout::eShaderReadOnlyOptimal
      );
    }
    return final_image;
  }
}

