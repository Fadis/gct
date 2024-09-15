#include <OpenImageIO/imageio.h>
#include <OpenImageIO/version.h>
#include <gct/allocator.hpp>
#include <gct/numeric_types.hpp>
#include <gct/is_pot.hpp>
#include <gct/get_device.hpp>
#include <gct/buffer.hpp>
#include <gct/device.hpp>
#include <gct/create_image_from_file.hpp>

namespace gct {
std::tuple< std::shared_ptr< buffer_t >, std::shared_ptr< image_t >, std::shared_ptr< image_t > >
create_image_from_file(
  const std::shared_ptr< allocator_t > &allocator,
  const std::string &filename,
  vk::ImageUsageFlags usage,
  bool mipmap,
  integer_attribute_t attr,
  unsigned int max_channels_per_layer,
  bool enable_srgb
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
  if( spec.nchannels == 0 ) throw -1;
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
    throw -1;
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
          if( a == attr ) throw -1;
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
          if( a == attr ) throw -1;
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
    if( formats.empty() ) throw -1;
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
    if( formats.empty() ) throw -1;
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
  return std::make_tuple( temporary_buffer, nonlinear_image, linear_image );
}
}

