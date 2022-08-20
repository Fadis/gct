#include <gct/astc.hpp>
namespace gct {
  bool is_astc( const astc_header &header ) {
    return
      header.magic[ 0 ] == 0x13 &&
      header.magic[ 1 ] == 0xAB &&
      header.magic[ 2 ] == 0xA1 &&
      header.magic[ 3 ] == 0x5C;
  }
  vk::Format get_format(
    const astc_header &header,
    bool srgb
  ) {
    if( header.block_z != 1u ) return vk::Format::eUndefined;
    if( !srgb ) {
      if( header.block_x == 4u && header.block_y == 4u ) return vk::Format::eAstc4x4UnormBlock;
      if( header.block_x == 5u && header.block_y == 4u ) return vk::Format::eAstc5x4UnormBlock;
      if( header.block_x == 5u && header.block_y == 5u ) return vk::Format::eAstc5x5UnormBlock;
      if( header.block_x == 6u && header.block_y == 5u ) return vk::Format::eAstc6x5UnormBlock;
      if( header.block_x == 6u && header.block_y == 6u ) return vk::Format::eAstc6x6UnormBlock;
      if( header.block_x == 8u && header.block_y == 5u ) return vk::Format::eAstc8x5UnormBlock;
      if( header.block_x == 8u && header.block_y == 6u ) return vk::Format::eAstc8x6UnormBlock;
      if( header.block_x == 8u && header.block_y == 8u ) return vk::Format::eAstc8x8UnormBlock;
      if( header.block_x == 10u && header.block_y == 5u ) return vk::Format::eAstc10x5UnormBlock;
      if( header.block_x == 10u && header.block_y == 6u ) return vk::Format::eAstc10x6UnormBlock;
      if( header.block_x == 10u && header.block_y == 8u ) return vk::Format::eAstc10x8UnormBlock;
      if( header.block_x == 10u && header.block_y == 10u ) return vk::Format::eAstc10x10UnormBlock;
      if( header.block_x == 12u && header.block_y == 10u ) return vk::Format::eAstc12x10UnormBlock;
      if( header.block_x == 12u && header.block_y == 12u ) return vk::Format::eAstc12x12UnormBlock;
    }
    else {
      if( header.block_x == 4u && header.block_y == 4u ) return vk::Format::eAstc4x4SrgbBlock;
      if( header.block_x == 5u && header.block_y == 4u ) return vk::Format::eAstc5x4SrgbBlock;
      if( header.block_x == 5u && header.block_y == 5u ) return vk::Format::eAstc5x5SrgbBlock;
      if( header.block_x == 6u && header.block_y == 5u ) return vk::Format::eAstc6x5SrgbBlock;
      if( header.block_x == 6u && header.block_y == 6u ) return vk::Format::eAstc6x6SrgbBlock;
      if( header.block_x == 8u && header.block_y == 5u ) return vk::Format::eAstc8x5SrgbBlock;
      if( header.block_x == 8u && header.block_y == 6u ) return vk::Format::eAstc8x6SrgbBlock;
      if( header.block_x == 8u && header.block_y == 8u ) return vk::Format::eAstc8x8SrgbBlock;
      if( header.block_x == 10u && header.block_y == 5u ) return vk::Format::eAstc10x5SrgbBlock;
      if( header.block_x == 10u && header.block_y == 6u ) return vk::Format::eAstc10x6SrgbBlock;
      if( header.block_x == 10u && header.block_y == 8u ) return vk::Format::eAstc10x8SrgbBlock;
      if( header.block_x == 10u && header.block_y == 10u ) return vk::Format::eAstc10x10SrgbBlock;
      if( header.block_x == 12u && header.block_y == 10u ) return vk::Format::eAstc12x10SrgbBlock;
      if( header.block_x == 12u && header.block_y == 12u ) return vk::Format::eAstc12x12SrgbBlock;
    }
    return vk::Format::eUndefined;
  }
  std::pair< std::uint32_t, std::uint32_t > get_dims(
    const astc_header &header
  ) {
    const std::uint32_t width = header.dim_x[0] + ( std::uint32_t( header.dim_x[1] ) << 8) + ( std::uint32_t( header.dim_x[2] ) << 16);
    const std::uint32_t height = header.dim_y[0] + ( std::uint32_t( header.dim_y[1] ) << 8) + ( std::uint32_t( header.dim_y[2] ) << 16);
    return std::make_pair( width, height );
  }
  std::uint32_t get_size(
    const astc_header &header
  ) {
    const auto [width,height] = get_dims( header );
    return ( width / header.block_x ) * ( height / header.block_y ) * 16u;
  }
}

