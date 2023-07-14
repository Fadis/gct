#include <fstream>
#include <gct/buffer.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/exception.hpp>
#include <gct/command_buffer_recorder.hpp>

namespace gct {
namespace {
  constexpr std::uint32_t leton( std::uint32_t v ) {
    if constexpr ( std::endian::native == std::endian::big ) {
      return
        ( ( v >> 24 ) & 0x000000FFu ) |
        ( ( v >> 8 ) & 0x0000FF00u ) |
        ( ( v << 8 ) & 0x00FF0000u ) |
        ( ( v << 24 ) & 0xFF000000u );
    }
    else if constexpr ( std::endian::native == std::endian::little ) {
      return v;
    }
    else {
      static_assert( std::endian::native == std::endian::little || std::endian::native == std::endian::big, "load_nnef_data : unsupported endian" );
    }
  }
}
  nnef_data_t
  command_buffer_recorder_t::load_nnef_data(
    const std::shared_ptr< allocator_t > &allocator,
    const std::filesystem::path &path,
    vk::BufferUsageFlags usage
  ) {
    if( !std::filesystem::exists( path ) ) {
      throw gct::exception::invalid_argument( "load_nnef_data : file not found", __FILE__, __LINE__ );
    }
    if( !std::filesystem::is_regular_file( path ) ) {
      throw gct::exception::invalid_argument( "load_nnef_data : not a regular file", __FILE__, __LINE__ );
    }
    const auto size = std::filesystem::file_size( path );
    if( size < 128u ) {
      throw gct::exception::invalid_argument( "load_nnef_data : not a nnef data", __FILE__, __LINE__ );
    }
    std::ifstream file( path, std::ios::in | std::ios::binary );
    const std::vector< std::uint8_t > data(
      ( std::istreambuf_iterator< char >( file ) ),
      std::istreambuf_iterator< char >()
    );
    if( data[ 0 ] != 'N' || data[ 1 ] != 0xefu ) {
      throw gct::exception::invalid_argument( "load_nnef_data : not a nnef data", __FILE__, __LINE__ );
    }
    if( data[ 2 ] != 1u || data[ 3 ] != 0u ) {
      throw gct::exception::invalid_argument( "load_nnef_data : unsupported nnef version", __FILE__, __LINE__ );
    }
    const auto data32 = reinterpret_cast< const std::uint32_t* >( data.data() + 4 );
    const auto data_size = leton( data32[ 0 ] );
    if( size < data_size + 128u ) {
      throw gct::exception::invalid_argument( "load_nnef_data : corrupted nnef", __FILE__, __LINE__ );
    }
    const auto rank = leton( data32[ 1 ] );
    if( rank > 8u ) {
      throw gct::exception::invalid_argument( "load_nnef_data : invalid rank", __FILE__, __LINE__ );
    }
    std::vector< std::uint32_t > shape;
    for( unsigned int i = 0u; i != rank; ++i ) {
      shape.push_back( leton( data32[ 2 + i ] ) );
    }
    const auto item_size = leton( data32[ 10 ] );
    if( item_size > 64u ) {
      throw gct::exception::invalid_argument( "load_nnef_data : invalid bits per item", __FILE__, __LINE__ );
    }
    const auto item_type = leton( data32[ 11 ] );
    numeric_type_t nt;
    if( item_type == 0u )  {
      nt.component = numeric_component_type_t::float_;
      nt.attr = integer_attribute_t::asis;
      if( item_size != 16u && item_size != 32u && item_size != 64u ) {
        throw gct::exception::invalid_argument( "load_nnef_data : unsupported float length", __FILE__, __LINE__ );
      }
      nt.depth = item_size;
      nt.sign = true;
      nt.rows = 1u;
    }
    else if( item_type == 1u ) {
      nt.component = numeric_component_type_t::int_;
      nt.attr = integer_attribute_t::asis;
      if( item_size != 8u && item_size != 16u && item_size != 32u && item_size != 64u ) {
        throw gct::exception::invalid_argument( "load_nnef_data : unsupported int length", __FILE__, __LINE__ );
      }
      nt.depth = item_size;
      nt.sign = false;
      nt.rows = 1u;
    }
    else if( item_type == 2u ) {
      nt.component = numeric_component_type_t::int_;
      nt.attr = integer_attribute_t::normalized;
      if( item_size != 8u && item_size != 16u && item_size != 32u && item_size != 64u ) {
        throw gct::exception::invalid_argument( "load_nnef_data : unsupported int length", __FILE__, __LINE__ );
      }
      nt.depth = item_size;
      nt.sign = false;
      nt.rows = 1u;
    }
    else if( item_type == 3u ) {
      nt.component = numeric_component_type_t::int_;
      nt.attr = integer_attribute_t::normalized;
      if( item_size != 8u && item_size != 16u && item_size != 32u && item_size != 64u ) {
        throw gct::exception::invalid_argument( "load_nnef_data : unsupported int length", __FILE__, __LINE__ );
      }
      nt.depth = item_size;
      nt.sign = true;
      nt.rows = 1u;
    }
    else if( item_type == 4u ) {
      nt.component = numeric_component_type_t::int_;
      nt.attr = integer_attribute_t::asis;
      if( item_size != 8u && item_size != 16u && item_size != 32u && item_size != 64u ) {
        throw gct::exception::invalid_argument( "load_nnef_data : unsupported int length", __FILE__, __LINE__ );
      }
      nt.depth = item_size;
      nt.sign = true;
      nt.rows = 1u;
    }
    else if( item_type == 5u ) {
      nt.component = numeric_component_type_t::int_;
      nt.attr = integer_attribute_t::asis;
      if( item_size != 8u && item_size != 16u && item_size != 32u && item_size != 64u ) {
        throw gct::exception::invalid_argument( "load_nnef_data : unsupported int length", __FILE__, __LINE__ );
      }
      nt.depth = item_size;
      nt.sign = true;
      nt.rows = 1u;
    }
    auto total_data_size = nt.depth / 8u;
    for( const auto &r: shape ) total_data_size *= r;
    if( total_data_size != data_size ) {
      throw gct::exception::invalid_argument( "load_nnef_data : data size and tensor size doesn't match", __FILE__, __LINE__ );
    }
    const auto data_begin = std::next( data.data(), 128 );
    auto buffer = load_buffer(
      allocator,
      reinterpret_cast< const void* >( data_begin ),
      total_data_size,
      usage
    );
    return nnef_data_t{
      nt,
      std::move( shape ),
      buffer
    };
  }
}

