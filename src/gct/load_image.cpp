#include <gct/buffer.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <OpenImageIO/imageio.h>
#include <OpenImageIO/version.h>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>

namespace gct {
  std::uint32_t get_pot( std::uint32_t v ) {
    for( std::uint32_t i = 0u; i != 32u; ++i )
      if( ( 1u << i ) >= v ) return i;
    return 32u;
  }
  bool is_pot( std::uint32_t v ) {
    return 1u << get_pot( v ) == v;
  }
  void command_buffer_recorder_t::create_mipmap(
    const std::shared_ptr< image_t > &image,
    vk::ImageLayout from,
    vk::ImageLayout to 
  ) {
    unsigned int mip_width = image->get_props().get_basic().extent.width;
    unsigned int mip_height = image->get_props().get_basic().extent.height;
    unsigned int mipmap_count = get_pot( image->get_props().get_basic().extent.width );
    convert_image( image, 0, 1, from, vk::ImageLayout::eTransferSrcOptimal );
    convert_image( image, 1, mipmap_count - 1, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal );
    for( uint32_t i = 1u; i < mipmap_count; ++i ) {
      (*get_factory())->blitImage(
        **image, vk::ImageLayout::eTransferSrcOptimal,
        **image, vk::ImageLayout::eTransferDstOptimal,
        {
          vk::ImageBlit()
            .setSrcSubresource(
              vk::ImageSubresourceLayers()
                .setAspectMask( vk::ImageAspectFlagBits::eColor )
                .setBaseArrayLayer( 0 )
                .setLayerCount( 1 )
                .setMipLevel( i - 1 )
            )
            .setSrcOffsets( {
              vk::Offset3D( 0, 0, 0 ),
              vk::Offset3D( mip_width, mip_height, 1 ),
            } )
            .setDstSubresource(
              vk::ImageSubresourceLayers()
                .setAspectMask( vk::ImageAspectFlagBits::eColor )
                .setBaseArrayLayer( 0 )
                .setLayerCount( 1 )
                .setMipLevel( i )
            )
            .setDstOffsets( {
              vk::Offset3D( 0, 0, 0 ),
              vk::Offset3D( mip_width / 2, mip_height / 2, 1 ),
            } )
        },
        vk::Filter::eLinear
      );
      convert_image( image, i, 1, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eTransferSrcOptimal );
      mip_width /= 2;
      mip_height /= 2;
    }
    convert_image( image, 0, mipmap_count, vk::ImageLayout::eTransferSrcOptimal, to );
  }
  void command_buffer_recorder_t::buffer_to_image(
    bool mipmap,
    const std::shared_ptr< buffer_t > &temporary,
    const std::shared_ptr< image_t > &destination
  ) {
    //if( destination.size != temporary.size ) vk::throwResultException( vk::Result( result ), "destinationとtemporaryのサイズが合わない" );
    uint32_t mipmap_count = 1u;
    bool mipable = destination->get_props().get_basic().extent.width == destination->get_props().get_basic().extent.height && is_pot( destination->get_props().get_basic().extent.width );
    if( mipmap && mipable ) {
      mipmap_count = get_pot( destination->get_props().get_basic().extent.width );
    }
    convert_image( destination, 0, mipmap_count, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal );
    (*get_factory())->copyBufferToImage(
      **temporary,
      **destination,
      vk::ImageLayout::eTransferDstOptimal,
      {
        vk::BufferImageCopy()
          .setBufferOffset( vk::DeviceSize( 0 ) )
          .setImageSubresource(
            vk::ImageSubresourceLayers()
              .setAspectMask( vk::ImageAspectFlagBits::eColor )
              .setMipLevel( 0 )
              .setLayerCount( 1 )
          )
          .setImageExtent(
            vk::Extent3D()
              .setWidth( destination->get_props().get_basic().extent.width )
              .setHeight( destination->get_props().get_basic().extent.height )
              .setDepth( 1 )
          )
      }
    );
    //uint32_t mip_width = destination.width;
    //uint32_t mip_height = destination.height;
    if( mipmap && mipable ) {
      create_mipmap(
        destination,
        vk::ImageLayout::eTransferDstOptimal,
        vk::ImageLayout::eShaderReadOnlyOptimal
      );
    }
    get_factory()->unbound()->keep.push_back( temporary );
  }
  std::shared_ptr< image_t > command_buffer_recorder_t::load_image(
    const std::shared_ptr< allocator_t > &allocator,
    const std::string &filename,
    vk::ImageUsageFlagBits usage,
    bool mipmap,
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
    uint32_t mipmap_count = 1u;
    if( mipmap && spec.width == spec.height && is_pot( spec.width ) ) {
      mipmap_count = get_pot( spec.width );
    }
    constexpr std::array< vk::Format, 5u > unorm_formats{
      vk::Format::eUndefined,
      vk::Format::eR8Unorm,
      vk::Format::eR8G8Unorm,
      vk::Format::eR8G8B8A8Unorm,
      vk::Format::eR8G8B8A8Unorm
    };
    constexpr std::array< vk::Format, 5u > srgb_formats{
      vk::Format::eUndefined,
      vk::Format::eR8Srgb,
      vk::Format::eR8G8Srgb,
      vk::Format::eR8G8B8A8Srgb,
      vk::Format::eR8G8B8A8Srgb
    };
    auto final_image = allocator->create_image(
      image_create_info_t()
        .set_basic(
          vk::ImageCreateInfo()
            .setImageType( vk::ImageType::e2D )
            .setFormat( srgb ? srgb_formats[ spec.nchannels ] : unorm_formats[ spec.nchannels ] )
            .setExtent( { (uint32_t)spec.width, (uint32_t)spec.height, 1 } )
            .setMipLevels( mipmap_count )
            .setArrayLayers( 1 )
            .setSamples( vk::SampleCountFlagBits::e1 )
            .setTiling( vk::ImageTiling::eOptimal )
            .setUsage( usage | vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled )
            .setSharingMode( vk::SharingMode::eExclusive )
            .setQueueFamilyIndexCount( 0 )
            .setPQueueFamilyIndices( nullptr )
            .setInitialLayout( vk::ImageLayout::eUndefined )
        ),
        VMA_MEMORY_USAGE_GPU_ONLY
    );
    auto temporary_buffer = allocator->create_buffer(
      spec.width * spec.height * ( spec.nchannels == 3 ? 4 : spec.nchannels ),
      vk::BufferUsageFlagBits::eTransferSrc,
      VMA_MEMORY_USAGE_CPU_TO_GPU
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
    buffer_to_image( mipmap, temporary_buffer, final_image );
    return final_image;
  }
}
