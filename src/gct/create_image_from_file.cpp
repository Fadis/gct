#include <OpenImageIO/imageio.h>
#include <OpenImageIO/version.h>
#include <ktx.h>
#include <vulkan/vulkan.hpp>
#include <ktxvulkan.h>
#include <gct/allocator.hpp>
#include <gct/numeric_types.hpp>
#include <gct/is_pot.hpp>
#include <gct/get_device.hpp>
#include <gct/buffer.hpp>
#include <gct/device.hpp>
#include <gct/physical_device.hpp>
#include <gct/image.hpp>
#include <gct/format.hpp>
#include <gct/dds.hpp>
#include <gct/mmaped_file.hpp>
#include <gct/create_image_from_file.hpp>

#ifdef ktxTexture_IsHDR
ktx_bool_t ktxTexture2_IsHDR(ktxTexture2*);
#endif

namespace gct {
create_image_from_file_state
create_image_from_file_using_openimageio(
  const std::shared_ptr< allocator_t > &allocator,
  const std::string &filename,
  vk::ImageUsageFlags usage,
  bool mipmap,
  integer_attribute_t attr,
  unsigned int max_channels_per_layer,
  bool enable_srgb
) {
#if OIIO_VERSION_MAJOR >= 4 || ( OIIO_VERSION_MAJOR == 3 && OIIO_VERSION_MINOR >= 1 )
    using namespace OIIO_CURRENT_NAMESPACE;
#else
    using namespace OIIO_NAMESPACE;
#endif
#if OIIO_VERSION_MAJOR >= 2 
  auto texture_file = ImageInput::open( filename );
#else
  std::shared_ptr< ImageInput > texture_file(
    ImageInput::open( filename ),
    []( auto p ) { if( p ) ImageInput::destroy( p ); }
  );
#endif
  if( !texture_file ) {
    return create_image_from_file_state();
  }
  const ImageSpec &spec = texture_file->spec();
  uint32_t mipmap_count = 1u;
  if( mipmap && spec.width == spec.height && is_pot( spec.width ) ) {
    mipmap_count = get_pot( spec.width );
  }
  if( spec.nchannels == 0 ) {
    return create_image_from_file_state();
  }
  const int src_channels_per_layer = std::max( std::min( std::min( 4, spec.nchannels ), int( max_channels_per_layer ) ), 1 );
  std::string space_str = spec["oiio:ColorSpace"];
  if( space_str == "" ) {
    space_str = "sRGB"; 
  }
  auto profile = parse_color_space( space_str );
  const int layers = spec.nchannels / src_channels_per_layer + ( ( spec.nchannels % src_channels_per_layer ) ? 1 : 0 );
  const int dest_channels_per_layer = ( src_channels_per_layer == 3 ) ? 4 : src_channels_per_layer;
  numeric_type_t nt;
  nt.composite = numeric_composite_type_t::vector;
  nt.rows = dest_channels_per_layer;
  if( spec.format == TypeDesc::UINT8 ) {
    nt.component = numeric_component_type_t::int_;
    nt.depth = 8u;
    nt.sign = false;
  }
  else if( spec.format == TypeDesc::INT8 ) {
    nt.component = numeric_component_type_t::int_;
    nt.depth = 8u;
    nt.sign = true;
  }
  else if( spec.format == TypeDesc::UINT16 ) {
    nt.component = numeric_component_type_t::int_;
    nt.depth = 16u;
    nt.sign = false;
  }
  else if( spec.format == TypeDesc::INT16 ) {
    nt.component = numeric_component_type_t::int_;
    nt.depth = 16u;
    nt.sign = true;
  }
  else if( spec.format == TypeDesc::UINT32 ) {
    nt.component = numeric_component_type_t::int_;
    nt.depth = 32u;
    nt.sign = false;
  }
  else if( spec.format == TypeDesc::INT32 ) {
    nt.component = numeric_component_type_t::int_;
    nt.depth = 32u;
    nt.sign = true;
  }
  else if( spec.format == TypeDesc::UINT64 ) {
    nt.component = numeric_component_type_t::int_;
    nt.depth = 64u;
    nt.sign = false;
  }
  else if( spec.format == TypeDesc::INT64 ) {
    nt.component = numeric_component_type_t::int_;
    nt.depth = 64u;
    nt.sign = true;
  }
  else if( spec.format == TypeDesc::HALF ) {
    nt.component = numeric_component_type_t::float_;
    nt.depth = 16u;
    nt.sign = true;
    attr = integer_attribute_t::asis;
  }
  else if( spec.format == TypeDesc::FLOAT ) {
    nt.component = numeric_component_type_t::float_;
    nt.depth = 32u;
    nt.sign = true;
    attr = integer_attribute_t::asis;
  }
  else if( spec.format == TypeDesc::DOUBLE ) {
    nt.component = numeric_component_type_t::float_;
    nt.depth = 64u;
    nt.sign = true;
    attr = integer_attribute_t::asis;
  }
  else {
    return create_image_from_file_state();
  }
  vk::Format primary_format = vk::Format::eUndefined;
  std::vector< vk::Format > secondary_formats;
  if( attr != integer_attribute_t::asis ) {
    if( enable_srgb ) {
      constexpr static std::array< integer_attribute_t, 2u > attrs{
        integer_attribute_t::normalized,
        integer_attribute_t::srgb
      };
      for( const auto a: attrs ) {
        nt.attr = a;
        const auto &formats = get_compatible_format( nt );
        if( formats.empty() ) {
          if( a == attr ) {
            return create_image_from_file_state();
          }
        }
        else {
          secondary_formats.push_back( formats[ 0 ] );
          if( a == attr ) {
            primary_format = formats[ 0 ];
          }
        }
      }
    }
    else {
      constexpr static std::array< integer_attribute_t, 1u > attrs{
        integer_attribute_t::normalized
      };
      for( const auto a: attrs ) {
        nt.attr = a;
        const auto &formats = get_compatible_format( nt );
        if( formats.empty() ) {
          if( a == attr ) {
            return create_image_from_file_state();
          }
        }
        else {
          secondary_formats.push_back( formats[ 0 ] );
          if( a == attr ) {
            primary_format = formats[ 0 ];
          }
        }
      }
    }
  }
  else {
    const auto &formats = get_compatible_format( nt );
    if( formats.empty() ) {
      return create_image_from_file_state();
    }
    secondary_formats.push_back( formats[ 0 ] );
    primary_format = formats[ 0 ];
  }
  vk::Format linear_format = vk::Format::eUndefined;
  auto linear_nt = nt;
  if( nt.component != numeric_component_type_t::float_ ) {
    linear_nt.component = numeric_component_type_t::float_;
    linear_nt.depth = 16;
    linear_nt.sign = true;
  }
  {
    const auto &formats = get_compatible_format( linear_nt );
    if( formats.empty() ) {
      return create_image_from_file_state();
    }
    linear_format = formats[ 0 ];
  }

  const auto api_version = get_device( *allocator ).get_api_version();
  const auto &exts = get_device( *allocator ).get_activated_extensions();
  const bool use_format_list = api_version >= VK_API_VERSION_1_2 || exts.find( "VK_KHR_image_format_list" ) != exts.end();
  usage |= vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
  if( !enable_srgb ) {
    usage |= vk::ImageUsageFlagBits::eStorage;
  }
  std::shared_ptr< image_t > nonlinear_image;
  if( use_format_list ) {
    nonlinear_image = allocator->create_image(
      image_create_info_t()
        .set_basic(
          vk::ImageCreateInfo()
            .setImageType( vk::ImageType::e2D )
            .setFormat( primary_format )
            .setExtent( { (uint32_t)spec.width, (uint32_t)spec.height, 1 } )
            .setMipLevels( mipmap_count )
            .setArrayLayers( layers )
            .setSamples( vk::SampleCountFlagBits::e1 )
            .setTiling( vk::ImageTiling::eOptimal )
            .setUsage( usage )
            .setSharingMode( vk::SharingMode::eExclusive )
            .setQueueFamilyIndexCount( 0 )
            .setPQueueFamilyIndices( nullptr )
            .setInitialLayout( vk::ImageLayout::eUndefined )
        )
        .add_format( secondary_formats.begin(), secondary_formats.end() )
        .set_profile( profile ),
        VMA_MEMORY_USAGE_GPU_ONLY
    );
  }
  else {
    nonlinear_image = allocator->create_image(
      image_create_info_t()
        .set_basic(
          vk::ImageCreateInfo()
            .setImageType( vk::ImageType::e2D )
            .setFormat( primary_format )
            .setExtent( { (uint32_t)spec.width, (uint32_t)spec.height, 1 } )
            .setMipLevels( mipmap_count )
            .setArrayLayers( layers )
            .setSamples( vk::SampleCountFlagBits::e1 )
            .setTiling( vk::ImageTiling::eOptimal )
            .setUsage( usage )
            .setSharingMode( vk::SharingMode::eExclusive )
            .setQueueFamilyIndexCount( 0 )
            .setPQueueFamilyIndices( nullptr )
            .setInitialLayout( vk::ImageLayout::eUndefined )
        )
        .set_profile( profile ),
        VMA_MEMORY_USAGE_GPU_ONLY
    );
  }

  std::shared_ptr< image_t > linear_image;
  if( !enable_srgb ) {
    linear_image = allocator->create_image(
      image_create_info_t()
        .set_basic(
          vk::ImageCreateInfo()
            .setImageType( vk::ImageType::e2D )
            .setFormat( linear_format )
            .setExtent( { (uint32_t)spec.width, (uint32_t)spec.height, 1 } )
            .setMipLevels( 1u/*mipmap_count*/ )
            .setArrayLayers( layers )
            .setSamples( vk::SampleCountFlagBits::e1 )
            .setTiling( vk::ImageTiling::eOptimal )
            .setUsage( usage )
            .setSharingMode( vk::SharingMode::eExclusive )
            .setQueueFamilyIndexCount( 0 )
            .setPQueueFamilyIndices( nullptr )
            .setInitialLayout( vk::ImageLayout::eUndefined )
        )
        .set_profile(
          color_profile()
            .set_space( color_space::cie_xyz )
            .set_gamma( color_gamma::linear )
            .set_max_intensity( 10000.0f )
        ),
        VMA_MEMORY_USAGE_GPU_ONLY
    );
  }

  auto temporary_buffer = allocator->create_buffer(
    spec.width * spec.height * int( nt.depth / 8u ) * dest_channels_per_layer * layers,
    vk::BufferUsageFlagBits::eTransferSrc,
    VMA_MEMORY_USAGE_CPU_TO_GPU
  );
  if( nt.depth == 8u ) {
    auto mapped = temporary_buffer->map< std::uint8_t >();
    const std::uint8_t one = get_one( nt );
    for( int layer_index = 0; layer_index != layers; ++layer_index ) {
      const int src_channels_offset = layer_index * src_channels_per_layer;
      const int dest_channels_offset = layer_index * dest_channels_per_layer;
      const int src_current_channels_per_layer = std::min( spec.nchannels - src_channels_offset, src_channels_per_layer );
      if( src_current_channels_per_layer == dest_channels_per_layer ) {
        texture_file->read_image(
#if OIIO_VERSION_MAJOR >= 2 
          0u, 0u,
#endif
          src_channels_offset,
          src_channels_offset + src_current_channels_per_layer,
          spec.format,
          reinterpret_cast< std::uint8_t* >( std::next( mapped.begin(), spec.width * spec.height * dest_channels_offset ) )
        );
      }
      else {
        std::vector< std::uint8_t > temp( spec.width * spec.height * dest_channels_per_layer );
        texture_file->read_image(
#if OIIO_VERSION_MAJOR >= 2 
          0u, 0u,
#endif
          src_channels_offset,
          src_channels_offset + src_current_channels_per_layer,
          spec.format,
          reinterpret_cast< std::uint8_t* >( temp.data() )
        );
        for( std::size_t i = spec.width * spec.height - 1; i; --i ) {
          for( std::size_t j = 0u; j != dest_channels_per_layer; ++j ) {
            if( j < src_current_channels_per_layer )
              temp[ i * dest_channels_per_layer + j ] = temp[ i * src_current_channels_per_layer + j ];
            else
              temp[ i * dest_channels_per_layer + j ] =
                ( j == 3u ) ? one : std::uint8_t( 0 );
          }
        }
        std::copy(
          temp.begin(),
          temp.end(),
          std::next( mapped.begin(), spec.width * spec.height * dest_channels_offset )
        );
      }
    }
  }
  else if( nt.depth == 16u ) {
    auto mapped = temporary_buffer->map< std::uint16_t >();
    const std::uint16_t one = get_one( nt );
    for( int layer_index = 0; layer_index != layers; ++layer_index ) {
      const int src_channels_offset = layer_index * src_channels_per_layer;
      const int dest_channels_offset = layer_index * dest_channels_per_layer;
      const int src_current_channels_per_layer = std::min( spec.nchannels - src_channels_offset, src_channels_per_layer );
      if( src_current_channels_per_layer == dest_channels_per_layer ) {
        texture_file->read_image(
#if OIIO_VERSION_MAJOR >= 2 
          0u, 0u,
#endif
          src_channels_offset,
          src_channels_offset + src_current_channels_per_layer,
          spec.format,
          reinterpret_cast< std::uint8_t* >( std::next( mapped.begin(), spec.width * spec.height * dest_channels_offset ) )
        );
      }
      else {
        std::vector< std::uint16_t > temp( spec.width * spec.height * dest_channels_per_layer );
        texture_file->read_image(
#if OIIO_VERSION_MAJOR >= 2 
          0u, 0u,
#endif
          src_channels_offset,
          src_channels_offset + src_current_channels_per_layer,
          spec.format,
          reinterpret_cast< std::uint8_t* >( temp.data() )
        );
        for( std::size_t i = spec.width * spec.height - 1; i; --i ) {
          for( std::size_t j = 0u; j != dest_channels_per_layer; ++j ) {
            if( j < src_current_channels_per_layer )
              temp[ i * dest_channels_per_layer + j ] = temp[ i * src_current_channels_per_layer + j ];
            else
              temp[ i * dest_channels_per_layer + j ] =
                ( j == 3u ) ? one : std::uint16_t( 0 );
          }
        }
        std::copy(
          temp.begin(),
          temp.end(),
          std::next( mapped.begin(), spec.width * spec.height * dest_channels_offset )
        );
      }
    }
  }
  else if( nt.depth == 32u ) {
    auto mapped = temporary_buffer->map< std::uint32_t >();
    const std::uint32_t one = get_one( nt );
    for( int layer_index = 0; layer_index != layers; ++layer_index ) {
      const int src_channels_offset = layer_index * src_channels_per_layer;
      const int dest_channels_offset = layer_index * dest_channels_per_layer;
      const int src_current_channels_per_layer = std::min( spec.nchannels - src_channels_offset, src_channels_per_layer );
      if( src_current_channels_per_layer == dest_channels_per_layer ) {
        texture_file->read_image(
#if OIIO_VERSION_MAJOR >= 2 
          0u, 0u,
#endif
          src_channels_offset,
          src_channels_offset + src_current_channels_per_layer,
          spec.format,
          reinterpret_cast< std::uint8_t* >( std::next( mapped.begin(), spec.width * spec.height * dest_channels_offset ) )
        );
      }
      else {
        std::vector< std::uint32_t > temp( spec.width * spec.height * dest_channels_per_layer );
        texture_file->read_image(
#if OIIO_VERSION_MAJOR >= 2 
          0u, 0u,
#endif
          src_channels_offset,
          src_channels_offset + src_current_channels_per_layer,
          spec.format,
          reinterpret_cast< std::uint8_t* >( temp.data() )
        );
        for( std::size_t i = spec.width * spec.height - 1; i; --i ) {
          for( std::size_t j = 0u; j != dest_channels_per_layer; ++j ) {
            if( j < src_current_channels_per_layer )
              temp[ i * dest_channels_per_layer + j ] = temp[ i * src_current_channels_per_layer + j ];
            else
              temp[ i * dest_channels_per_layer + j ] =
                ( j == 3u ) ? one : std::uint32_t( 0 );
          }
        }
        std::copy(
          temp.begin(),
          temp.end(),
          std::next( mapped.begin(), spec.width * spec.height * dest_channels_offset )
        );
      }
    }
  }
  else if( nt.depth == 64u ) {
    auto mapped = temporary_buffer->map< std::uint64_t >();
    const std::uint64_t one = get_one( nt );
    for( int layer_index = 0; layer_index != layers; ++layer_index ) {
      const int src_channels_offset = layer_index * src_channels_per_layer;
      const int dest_channels_offset = layer_index * dest_channels_per_layer;
      const int src_current_channels_per_layer = std::min( spec.nchannels - src_channels_offset, src_channels_per_layer );
      if( src_current_channels_per_layer == dest_channels_per_layer ) {
        texture_file->read_image(
#if OIIO_VERSION_MAJOR >= 2 
          0u, 0u,
#endif
          src_channels_offset,
          src_channels_offset + src_current_channels_per_layer,
          spec.format,
          reinterpret_cast< std::uint8_t* >( std::next( mapped.begin(), spec.width * spec.height * dest_channels_offset ) )
        );
      }
      else {
        std::vector< std::uint64_t > temp( spec.width * spec.height * dest_channels_per_layer );
        texture_file->read_image(
#if OIIO_VERSION_MAJOR >= 2 
          0u, 0u,
#endif
          src_channels_offset,
          src_channels_offset + src_current_channels_per_layer,
          spec.format,
          reinterpret_cast< std::uint8_t* >( temp.data() )
        );
        for( std::size_t i = spec.width * spec.height - 1; i; --i ) {
          for( std::size_t j = 0u; j != dest_channels_per_layer; ++j ) {
            if( j < src_current_channels_per_layer )
              temp[ i * dest_channels_per_layer + j ] = temp[ i * src_current_channels_per_layer + j ];
            else
              temp[ i * dest_channels_per_layer + j ] =
                ( j == 3u ) ? one : std::uint64_t( 0 );
          }
        }
        std::copy(
          temp.begin(),
          temp.end(),
          std::next( mapped.begin(), spec.width * spec.height * dest_channels_offset )
        );
      }
    }
  }
  return 
    create_image_from_file_state()
      .set_loaded( true )
      .set_staging_buffer( temporary_buffer )
      .set_nonlinear_image( nonlinear_image )
      .set_linear_image( linear_image )
      .set_generate_mipmap( mipmap )
      .add_copy_range(
        vk::BufferImageCopy()
          .setBufferOffset( 0 )
          .setBufferRowLength(
            nonlinear_image ?
              nonlinear_image->get_props().get_basic().extent.width :
              linear_image->get_props().get_basic().extent.width
          )
          .setBufferImageHeight(
            nonlinear_image ?
              nonlinear_image->get_props().get_basic().extent.height :
              linear_image->get_props().get_basic().extent.height
          )
          .setImageSubresource(
            vk::ImageSubresourceLayers()
              .setAspectMask( format_to_aspect(
                nonlinear_image ?
                  nonlinear_image->get_props().get_basic().format :
                  linear_image->get_props().get_basic().format
              ) )
              .setMipLevel( 0 )
              .setBaseArrayLayer( 0 )
              .setLayerCount(
                nonlinear_image ?
                  nonlinear_image->get_props().get_basic().arrayLayers :
                  linear_image->get_props().get_basic().arrayLayers
              )
          )
          .setImageOffset(
            vk::Offset3D()
              .setX( 0 )
              .setY( 0 )
              .setZ( 0 )
          )
          .setImageExtent(
            nonlinear_image ?
              nonlinear_image->get_props().get_basic().extent :
              linear_image->get_props().get_basic().extent
          )
      );
}

create_image_from_file_state
create_image_from_dds(
  const std::shared_ptr< allocator_t > &allocator,
  const std::string &filename,
  vk::ImageUsageFlags usage,
  bool mipmap,
  integer_attribute_t attr,
  unsigned int max_channels_per_layer,
  bool enable_srgb
) {
  gct::mmaped_file file( filename );
  const auto ci = gct::dds::get_dds_image_create_info( file.begin() );
  
  auto iter = gct::dds::get_dds_data( ci, file.begin() );

  auto temporary_buffer = allocator->create_buffer(
    std::distance( *iter, file.end() ),
    vk::BufferUsageFlagBits::eTransferSrc,
    VMA_MEMORY_USAGE_CPU_TO_GPU
  );
  {
    auto mapped = temporary_buffer->map< std::uint8_t >();
    std::copy( *iter, file.end(), mapped.begin() );
  }

  const bool hdr = is_hdr( ci.format );

  const auto profile =
    hdr ?
    color_profile()
      .set_space( color_space::cie_xyz )
      .set_gamma( color_gamma::linear ) :
    color_profile()
      .set_space( color_space::bt709 )
      .set_gamma( color_gamma::linear );

  const auto api_version = get_device( *allocator ).get_api_version();
  const auto &exts = get_device( *allocator ).get_activated_extensions();
  const bool use_format_list = api_version >= VK_API_VERSION_1_2 || exts.find( "VK_KHR_image_format_list" ) != exts.end();
  usage |= vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;

  std::vector< vk::Format > secondary_formats = get_compatible_format( ci.format );

  std::shared_ptr< image_t > image;
  if( use_format_list ) {
    image = allocator->create_image(
      image_create_info_t()
        .set_basic(
          vk::ImageCreateInfo( ci )
            .setUsage( usage )
        )
        .add_format( secondary_formats.begin(), secondary_formats.end() )
        .set_profile( profile ),
        VMA_MEMORY_USAGE_GPU_ONLY
    );
  }
  else {
    image = allocator->create_image(
      image_create_info_t()
        .set_basic(
          vk::ImageCreateInfo( ci )
            .setUsage( usage )
        )
        .set_profile( profile ),
        VMA_MEMORY_USAGE_GPU_ONLY
    );
  }

  auto state =
    create_image_from_file_state()
      .set_loaded( true )
      .set_staging_buffer( temporary_buffer )
      .set_nonlinear_image( image )
      .set_generate_mipmap( false );

  std::uint32_t offset = 0u;
  for( ; iter != gct::dds::dds_iterator(); ++iter ) {
    state.copy_range.push_back(
      vk::BufferImageCopy()
        .setBufferOffset( offset )
        .setBufferRowLength(
          std::max( image->get_props().get_basic().extent.width >> iter.get_mip_level(), format_to_block_width( ci.format ) )
        )
        .setBufferImageHeight(
          std::max( image->get_props().get_basic().extent.height >> iter.get_mip_level(), format_to_block_height( ci.format ) )
        )
        .setImageSubresource(
          vk::ImageSubresourceLayers()
            .setAspectMask( format_to_aspect(
              image->get_props().get_basic().format
            ) )
            .setMipLevel( iter.get_mip_level() )
            .setBaseArrayLayer( iter.get_array_layer() )
            .setLayerCount( 1u )
        )
        .setImageOffset(
          vk::Offset3D()
            .setX( 0 )
            .setY( 0 )
            .setZ( 0 )
        )
        .setImageExtent(
          vk::Extent3D()
            .setWidth( std::max( image->get_props().get_basic().extent.width >> iter.get_mip_level(), 1u ) )
            .setHeight( std::max( image->get_props().get_basic().extent.height >> iter.get_mip_level(), 1u ) )
            .setDepth( std::max( image->get_props().get_basic().extent.depth >> iter.get_mip_level(), 1u ) )
        )
    );
    offset += iter.get_subimage_bytes();
  }
  return state;
}

create_image_from_file_state
create_image_from_ktx1(
  const std::shared_ptr< allocator_t > &allocator,
  const std::string &filename,
  vk::ImageUsageFlags usage,
  bool mipmap,
  integer_attribute_t attr,
  unsigned int max_channels_per_layer,
  bool enable_srgb
) {
  ktxTexture* tex;
  KTX_error_code result = ktxTexture_CreateFromNamedFile(
    filename.c_str(),
    KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT,
    &tex
  );
  if( result != KTX_SUCCESS ) {
    throw std::runtime_error( std::string( "ktxTexture_CreateFromNamedFile failed : " ) + filename + " : " + ktxErrorString( result ) );
  }

  auto temporary_buffer = allocator->create_buffer(
    tex->dataSize,
    vk::BufferUsageFlagBits::eTransferSrc,
    VMA_MEMORY_USAGE_CPU_TO_GPU
  );
  const std::uint8_t *data = ktxTexture_GetData( tex );
  {
    auto mapped = temporary_buffer->map< std::uint8_t >();
    std::copy( data, std::next( data, tex->dataSize ), mapped.begin() );
  }

  const vk::Format format = static_cast< vk::Format >( ktxTexture_GetVkFormat( tex ) );

  auto ci = vk::ImageCreateInfo()
    .setFlags(
      tex->isCubemap ?
      vk::ImageCreateFlagBits::eCubeCompatible :
      vk::ImageCreateFlagBits( 0 )
    )
    .setImageType(
      ( tex->numDimensions == 1u ) ?
        vk::ImageType::e1D :
        ( tex->numDimensions == 2u ) ?
        vk::ImageType::e2D :
        vk::ImageType::e3D
    )
    .setFormat( format )
    .setExtent(
      vk::Extent3D()
        .setWidth( tex->baseWidth )
        .setHeight( tex->baseHeight )
        .setDepth( tex->baseDepth )
    )
    .setMipLevels( tex->numLevels )
    .setArrayLayers( tex->numLayers * tex->numFaces )
    .setSamples( vk::SampleCountFlagBits::e1 )
    .setTiling( vk::ImageTiling::eOptimal )
    .setSharingMode( vk::SharingMode::eExclusive )
    .setQueueFamilyIndexCount( 0 )
    .setPQueueFamilyIndices( nullptr )
    .setInitialLayout( vk::ImageLayout::eUndefined );

  const bool is_hdr = gct::is_hdr( format );
  const gct::color_profile profile =
    is_hdr ?
    gct::color_profile()
      .set_space( gct::color_space::cie_xyz )
      .set_gamma( gct::color_gamma::linear ) :
    gct::color_profile()
      .set_space( gct::color_space::bt709 )
      .set_gamma( gct::color_gamma::srgb );

  const auto api_version = get_device( *allocator ).get_api_version();
  const auto &exts = get_device( *allocator ).get_activated_extensions();
  const bool use_format_list = api_version >= VK_API_VERSION_1_2 || exts.find( "VK_KHR_image_format_list" ) != exts.end();
  usage |= vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;

  std::vector< vk::Format > secondary_formats = get_compatible_format( ci.format );

  std::shared_ptr< image_t > image;
  if( use_format_list ) {
    image = allocator->create_image(
      image_create_info_t()
        .set_basic(
          vk::ImageCreateInfo( ci )
            .setUsage( usage )
        )
        .add_format( secondary_formats.begin(), secondary_formats.end() )
        .set_profile( profile ),
        VMA_MEMORY_USAGE_GPU_ONLY
    );
  }
  else {
    image = allocator->create_image(
      image_create_info_t()
        .set_basic(
          vk::ImageCreateInfo( ci )
            .setUsage( usage )
        )
        .set_profile( profile ),
        VMA_MEMORY_USAGE_GPU_ONLY
    );
  }

  auto state =
    create_image_from_file_state()
      .set_loaded( true )
      .set_staging_buffer( temporary_buffer )
      .set_nonlinear_image( image )
      .set_generate_mipmap( false );

  for( std::uint32_t m = 0u; m != tex->numLevels; ++m ) {
    for( std::uint32_t l = 0u; l != tex->numLayers; ++l ) {
      for( std::uint32_t f = 0u; f != tex->numFaces; ++f ) {
        ktx_size_t offset = 0u;
        KTX_error_code result = ktxTexture_GetImageOffset( tex, m, l, f, &offset );
        if( result != KTX_SUCCESS ) {
          throw std::runtime_error( std::string( "ktxTexture_GetImageOffset failed : " ) + ktxErrorString( result ) );
        }
        state.copy_range.push_back(
          vk::BufferImageCopy()
            .setBufferOffset( offset )
            .setBufferRowLength(
              std::max( tex->baseWidth >> m, format_to_block_width( format ) )
            )
            .setBufferImageHeight(
              std::max( tex->baseHeight >> m, format_to_block_height( format ) )
            )
            .setImageSubresource(
              vk::ImageSubresourceLayers()
                .setAspectMask( format_to_aspect(
                  format
                ) )
                .setMipLevel( m )
                .setBaseArrayLayer( l * tex->numFaces + f )
                .setLayerCount( 1u )
            )
            .setImageOffset(
              vk::Offset3D()
                .setX( 0 )
                .setY( 0 )
                .setZ( 0 )
            )
            .setImageExtent(
              vk::Extent3D()
                .setWidth( std::max( tex->baseWidth >> m, 1u ) )
                .setHeight( std::max( tex->baseHeight >> m, 1u ) )
                .setDepth( std::max( tex->baseDepth >> m, 1u ) )
            )
        );
      }
    }
  }
  return state;
}

bool is_uastc( ktxTexture2 *tex ) {
  const khr_df_model_e color_model = ktxTexture2_GetColorModel_e( tex );
  return color_model == KHR_DF_MODEL_UASTC;
}

gct::color_profile ktx_profile_to_gct_profile( ktxTexture2 *tex ) {
  gct::color_profile profile;
  const vk::Format format = static_cast< vk::Format >( ktxTexture2_GetVkFormat( tex ) );
  const khr_df_transfer_e gamma = ktxTexture2_GetOETF_e( tex );
  if(
    gamma == KHR_DF_TRANSFER_UNSPECIFIED ||
    gamma == KHR_DF_TRANSFER_LINEAR
  ) {
    profile.set_gamma( gct::color_gamma::linear );
    profile.set_max_intensity( 10000.0f );
  }
  else if(
    gamma == KHR_DF_TRANSFER_SRGB ||
    gamma == KHR_DF_TRANSFER_SRGB_EOTF ||
    gamma == KHR_DF_TRANSFER_SCRGB ||
    gamma == KHR_DF_TRANSFER_SCRGB_EOTF
  ) {
    profile.set_gamma( gct::color_gamma::srgb );
  }
  else if(
    gamma == KHR_DF_TRANSFER_ITU ||
    gamma == KHR_DF_TRANSFER_ITU_OETF ||
    gamma == KHR_DF_TRANSFER_BT601 ||
    gamma == KHR_DF_TRANSFER_BT601_OETF ||
    gamma == KHR_DF_TRANSFER_BT709 ||
    gamma == KHR_DF_TRANSFER_BT709_OETF ||
    gamma == KHR_DF_TRANSFER_BT2020 ||
    gamma == KHR_DF_TRANSFER_BT2020_OETF ||
    gamma == KHR_DF_TRANSFER_SMTPE170M ||
    gamma == KHR_DF_TRANSFER_SMTPE170M_OETF ||
    gamma == KHR_DF_TRANSFER_SMTPE170M_EOTF
  ) {
    profile.set_gamma( gct::color_gamma::bt709 );
  }
  else if(
    gamma == KHR_DF_TRANSFER_NTSC ||
    gamma == KHR_DF_TRANSFER_NTSC_EOTF ||
    gamma == KHR_DF_TRANSFER_ADOBERGB ||
    gamma == KHR_DF_TRANSFER_ADOBERGB_EOTF
  ) {
    profile.set_gamma( gct::color_gamma::srgb );
  }
  else if(
    gamma == KHR_DF_TRANSFER_DCIP3 ||
    gamma == KHR_DF_TRANSFER_DCIP3_EOTF
  ) {
    profile.set_gamma( gct::color_gamma::dci_p3 );
  }
  else if(
    gamma == KHR_DF_TRANSFER_HLG_OETF ||
    gamma == KHR_DF_TRANSFER_HLG_EOTF
  ) {
    profile.set_gamma( gct::color_gamma::hlg );
    profile.set_max_intensity( 10000.0f );
  }
  else if(
    gamma == KHR_DF_TRANSFER_PQ_OETF ||
    gamma == KHR_DF_TRANSFER_PQ_EOTF
  ) {
    profile.set_gamma( gct::color_gamma::pq );
    profile.set_max_intensity( 10000.0f );
  }
  else {
    profile.set_gamma( gct::color_gamma::unknown );
  }
  const khr_df_primaries_e space = ktxTexture2_GetPrimaries_e( tex );
  if(
    space == KHR_DF_PRIMARIES_BT709
  ) {
    profile.set_space( gct::color_space::bt709 );
    if( profile.gamma == gct::color_gamma::srgb && is_srgb( format ) ) {
      profile.set_gamma( gct::color_gamma::linear );
    }
  }
  else if(
    space == KHR_DF_PRIMARIES_BT601_EBU ||
    space == KHR_DF_PRIMARIES_BT601_SMPTE
  ) {
    profile.set_space( gct::color_space::ntsc );
  }
  else if(
    space == KHR_DF_PRIMARIES_BT2020 
  ) {
    profile.set_space( gct::color_space::bt2020 );
  }
  else if(
    space == KHR_DF_PRIMARIES_CIEXYZ
  ) {
    profile.set_space( gct::color_space::cie_xyz );
  }
  else if(
    space == KHR_DF_PRIMARIES_ACES ||
    space == KHR_DF_PRIMARIES_ACESCC
  ) {
    profile.set_space( gct::color_space::aces_cg );
  }
  else if(
    space == KHR_DF_PRIMARIES_DISPLAYP3
  ) {
    profile.set_space( gct::color_space::display_p3 );
  }
  else if(
    space == KHR_DF_PRIMARIES_ADOBERGB
  ) {
    profile.set_space( gct::color_space::adobe_rgb );
  }
  else {
    profile.set_space( gct::color_space::unknown );
  }
  return profile;
}

create_image_from_file_state
create_image_from_ktx2(
  const std::shared_ptr< allocator_t > &allocator,
  const std::string &filename,
  vk::ImageUsageFlags usage,
  bool mipmap,
  integer_attribute_t attr,
  unsigned int max_channels_per_layer,
  bool enable_srgb
) {
  ktxTexture2* tex;
  KTX_error_code result = ktxTexture2_CreateFromNamedFile(
    filename.c_str(),
    KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT,
    &tex
  );
  if( result != KTX_SUCCESS ) {
    throw std::runtime_error( std::string( "ktxTexture2_CreateFromNamedFile failed : " ) + filename + " : " + ktxErrorString( result ) );
  }

  const bool transcode_is_needed = ktxTexture2_NeedsTranscoding( tex );

  const auto physical_device = allocator->get_factory()->get_physical_device_group().devices[ 0 ];
  ktx_transcode_fmt_e transcoded_format = KTX_TTF_RGBA32;
#ifdef ktxTexture_IsHDR
  const bool is_hdr = ktxTexture2_IsHDR( tex );
  const static std::vector< std::tuple< vk::Format, ktx_transcode_fmt_e > > hdr_possible_transcoded_formats{
    std::make_tuple( vk::Format::eAstc4x4UfloatBlock, ktx_transcode_fmt_e::KTX_TTF_ASTC_HDR_4x4_RGBA ),
    std::make_tuple( vk::Format::eAstc6x6UfloatBlock, ktx_transcode_fmt_e::KTX_TTF_ASTC_HDR_6x6_RGBA ),
    std::make_tuple( vk::Format::eBc6HUfloatBlock, ktx_transcode_fmt_e::KTX_TTF_BC6HU ),
    std::make_tuple( vk::Format::eR16G16B16A16Sfloat, ktx_transcode_fmt_e::KTX_TTF_RGBA_HALF )
  };
#endif
  const static std::vector< std::tuple< vk::Format, ktx_transcode_fmt_e > > ldr_possible_transcoded_formats{
    std::make_tuple( vk::Format::eAstc4x4SrgbBlock, ktx_transcode_fmt_e::KTX_TTF_ASTC_4x4_RGBA ),
    std::make_tuple( vk::Format::eBc7SrgbBlock, ktx_transcode_fmt_e::KTX_TTF_BC7_RGBA ),
    std::make_tuple( vk::Format::eEtc2R8G8B8A8SrgbBlock, ktx_transcode_fmt_e::KTX_TTF_ETC2_RGBA ),
    std::make_tuple( vk::Format::ePvrtc24BppSrgbBlockIMG, ktx_transcode_fmt_e::KTX_TTF_PVRTC2_4_RGBA ),
    std::make_tuple( vk::Format::ePvrtc14BppSrgbBlockIMG, ktx_transcode_fmt_e::KTX_TTF_PVRTC1_4_RGBA ),
    std::make_tuple( vk::Format::eBc3SrgbBlock, ktx_transcode_fmt_e::KTX_TTF_BC3_RGBA ),
    std::make_tuple( vk::Format::eBc1RgbSrgbBlock, ktx_transcode_fmt_e::KTX_TTF_BC1_RGB ),
    std::make_tuple( vk::Format::eR8G8B8A8Srgb, ktx_transcode_fmt_e::KTX_TTF_RGBA32 )
  };
  for( const auto &f:
#ifdef ktxTexture_IsHDR
    is_hdr ?
    hdr_possible_transcoded_formats :
#endif
    ldr_possible_transcoded_formats
  ) {
    const auto available_for_sampling = physical_device->get_format_properties( std::get< 0 >( f ) ).get_basic().optimalTilingFeatures | vk::FormatFeatureFlagBits::eSampledImage;
    if( available_for_sampling ) {
      transcoded_format = std::get< 1 >( f );
    }
  }

  if( transcode_is_needed ) {
    KTX_error_code result = ktxTexture2_TranscodeBasis( tex, transcoded_format, 0u );
    if( result != KTX_SUCCESS ) {
      throw std::runtime_error( std::string( "ktxTexture2_TranscodeBasis failed : " ) + ktxErrorString( result ) );
    }
  }

  auto temporary_buffer = allocator->create_buffer(
    tex->dataSize,
    vk::BufferUsageFlagBits::eTransferSrc,
    VMA_MEMORY_USAGE_CPU_TO_GPU
  );
  {
    auto mapped = temporary_buffer->map< std::uint8_t >();
    std::copy( tex->pData, std::next( tex->pData, tex->dataSize ), mapped.begin() );
  }

  const vk::Format format = static_cast< vk::Format >( ktxTexture2_GetVkFormat( tex ) );

  auto ci = vk::ImageCreateInfo()
    .setFlags(
      tex->isCubemap ?
      vk::ImageCreateFlagBits::eCubeCompatible :
      vk::ImageCreateFlagBits( 0 )
    )
    .setImageType(
      ( tex->numDimensions == 1u ) ?
        vk::ImageType::e1D :
        ( tex->numDimensions == 2u ) ?
        vk::ImageType::e2D :
        vk::ImageType::e3D
    )
    .setFormat( format )
    .setExtent(
      vk::Extent3D()
        .setWidth( tex->baseWidth )
        .setHeight( tex->baseHeight )
        .setDepth( tex->baseDepth )
    )
    .setMipLevels( tex->numLevels )
    .setArrayLayers( tex->numLayers * tex->numFaces )
    .setSamples( vk::SampleCountFlagBits::e1 )
    .setTiling( vk::ImageTiling::eOptimal )
    .setSharingMode( vk::SharingMode::eExclusive )
    .setQueueFamilyIndexCount( 0 )
    .setPQueueFamilyIndices( nullptr )
    .setInitialLayout( vk::ImageLayout::eUndefined );

  const bool is_hdr = gct::is_hdr( format );
  const auto profile = ktx_profile_to_gct_profile( tex );

  const auto api_version = get_device( *allocator ).get_api_version();
  const auto &exts = get_device( *allocator ).get_activated_extensions();
  const bool use_format_list = api_version >= VK_API_VERSION_1_2 || exts.find( "VK_KHR_image_format_list" ) != exts.end();
  usage |= vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;

  std::vector< vk::Format > secondary_formats = get_compatible_format( ci.format );

  std::shared_ptr< image_t > image;
  if( use_format_list ) {
    image = allocator->create_image(
      image_create_info_t()
        .set_basic(
          vk::ImageCreateInfo( ci )
            .setUsage( usage )
        )
        .add_format( secondary_formats.begin(), secondary_formats.end() )
        .set_profile( profile ),
        VMA_MEMORY_USAGE_GPU_ONLY
    );
  }
  else {
    image = allocator->create_image(
      image_create_info_t()
        .set_basic(
          vk::ImageCreateInfo( ci )
            .setUsage( usage )
        )
        .set_profile( profile ),
        VMA_MEMORY_USAGE_GPU_ONLY
    );
  }

  auto state =
    create_image_from_file_state()
      .set_loaded( true )
      .set_staging_buffer( temporary_buffer )
      .set_nonlinear_image( image )
      .set_generate_mipmap( false );

  for( std::uint32_t m = 0u; m != tex->numLevels; ++m ) {
    for( std::uint32_t l = 0u; l != tex->numLayers; ++l ) {
      for( std::uint32_t f = 0u; f != tex->numFaces; ++f ) {
        ktx_size_t offset = 0u;
        KTX_error_code result = ktxTexture2_GetImageOffset( tex, m, l, f, &offset );
        if( result != KTX_SUCCESS ) {
          throw std::runtime_error( std::string( "ktxTexture2_GetImageOffset failed : " ) + ktxErrorString( result ) );
        }
        state.copy_range.push_back(
          vk::BufferImageCopy()
            .setBufferOffset( offset )
            .setBufferRowLength(
              std::max( tex->baseWidth >> m, format_to_block_width( format ) )
            )
            .setBufferImageHeight(
              std::max( tex->baseHeight >> m, format_to_block_height( format ) )
            )
            .setImageSubresource(
              vk::ImageSubresourceLayers()
                .setAspectMask( format_to_aspect(
                  format
                ) )
                .setMipLevel( m )
                .setBaseArrayLayer( l * tex->numFaces + f )
                .setLayerCount( 1u )
            )
            .setImageOffset(
              vk::Offset3D()
                .setX( 0 )
                .setY( 0 )
                .setZ( 0 )
            )
            .setImageExtent(
              vk::Extent3D()
                .setWidth( std::max( tex->baseWidth >> m, 1u ) )
                .setHeight( std::max( tex->baseHeight >> m, 1u ) )
                .setDepth( std::max( tex->baseDepth >> m, 1u ) )
            )
        );
      }
    }
  }
  return state;
}

create_image_from_file_state
create_image_from_file(
  const std::shared_ptr< allocator_t > &allocator,
  const std::string &filename,
  vk::ImageUsageFlags usage,
  bool mipmap,
  integer_attribute_t attr,
  unsigned int max_channels_per_layer,
  bool enable_srgb
) {
  const auto ext_begin = filename.rfind( "." );
  const auto ext = filename.substr( filename.rfind( "." ), filename.size() );
  if( ext == ".dds" ) {
    return create_image_from_dds(
      allocator,
      filename,
      usage,
      mipmap,
      attr,
      max_channels_per_layer,
      enable_srgb
    );
  }
  else if( ext == ".ktx" ) {
    return create_image_from_ktx1(
      allocator,
      filename,
      usage,
      mipmap,
      attr,
      max_channels_per_layer,
      enable_srgb
    );
  }
  else if( ext == ".ktx2" ) {
    return create_image_from_ktx2(
      allocator,
      filename,
      usage,
      mipmap,
      attr,
      max_channels_per_layer,
      enable_srgb
    );
  }

  return create_image_from_file_using_openimageio(
    allocator,
    filename,
    usage,
    mipmap,
    attr,
    max_channels_per_layer,
    enable_srgb
  );
}

}

