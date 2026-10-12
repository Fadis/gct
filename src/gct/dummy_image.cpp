#include <gct/dummy_image.hpp>

namespace gct {
std::unordered_map< std::uint32_t, dummy_image< glm::vec4, 1u > > image_pool_1d;
std::unordered_map< std::uint32_t, dummy_image< glm::vec4, 2u > > image_pool_2d;
std::unordered_map< std::uint32_t, dummy_image< glm::vec4, 3u > > image_pool_3d;
std::unordered_map< std::uint32_t, dummy_image_array< glm::vec4, 1u > > image_pool_1d_array;
std::unordered_map< std::uint32_t, dummy_image_array< glm::vec4, 2u > > image_pool_2d_array;
std::unordered_map< std::uint32_t, dummy_image< glm::uvec4, 1u > > image_pool_1du;
std::unordered_map< std::uint32_t, dummy_image< glm::uvec4, 2u > > image_pool_2du;
std::unordered_map< std::uint32_t, dummy_image< glm::uvec4, 3u > > image_pool_3du;
std::unordered_map< std::uint32_t, dummy_image_array< glm::uvec4, 1u > > image_pool_1du_array;
std::unordered_map< std::uint32_t, dummy_image_array< glm::uvec4, 2u > > image_pool_2du_array;
std::unordered_map< std::uint32_t, dummy_image< std::int32_t, 1u > > image_pool_1da;
std::unordered_map< std::uint32_t, dummy_image< std::int32_t, 2u > > image_pool_2da;
std::unordered_map< std::uint32_t, dummy_image< std::int32_t, 3u > > image_pool_3da;
std::unordered_map< std::uint32_t, dummy_image_array< std::int32_t, 1u > > image_pool_1da_array;
std::unordered_map< std::uint32_t, dummy_image_array< std::int32_t, 2u > > image_pool_2da_array;
std::unordered_map< std::uint32_t, dummy_image< std::uint32_t, 1u > > image_pool_1dua;
std::unordered_map< std::uint32_t, dummy_image< std::uint32_t, 2u > > image_pool_2dua;
std::unordered_map< std::uint32_t, dummy_image< std::uint32_t, 3u > > image_pool_3dua;
std::unordered_map< std::uint32_t, dummy_image_array< std::uint32_t, 1u > > image_pool_1dua_array;
std::unordered_map< std::uint32_t, dummy_image_array< std::uint32_t, 2u > > image_pool_2dua_array;

dummy_image< glm::vec4, 2u > &create_dummy_image_2d( std::uint32_t index, std::uint32_t x, std::uint32_t y ) {
  image_pool_2d[ index ].extent = glm::ivec3( x, y, 0u );
  image_pool_2d[ index ].pixel.resize( x * y, glm::vec4( 0, 0, 0, 0 ) );
  return image_pool_2d[ index ];
}

void imageStore( dummy_image< glm::vec4, 1u > &d, std::int32_t p, const glm::vec4 &v ) {
  if( p < 0 || p >= d.extent.x ) return;
  d.pixel[ p ] = v;
}

void imageStore( dummy_image< glm::vec4, 2u > &d, const glm::ivec2 &p, const glm::vec4 &v ) {
  if( p.x < 0 || p.x >= d.extent.x ) return;
  if( p.y < 0 || p.y >= d.extent.y ) return;
  d.pixel[ p.x + d.extent.x * p.y ] = v;
}

void imageStore( dummy_image< glm::vec4, 3u > &d, const glm::ivec3 &p, const glm::vec4 &v ) {
  if( p.x < 0 || p.x >= d.extent.x ) return;
  if( p.y < 0 || p.y >= d.extent.y ) return;
  if( p.z < 0 || p.z >= d.extent.z ) return;
  d.pixel[ p.x + d.extent.x * p.y + d.extent.x * d.extent.y * p.z ] = v;
}

void imageStore( dummy_image< glm::uvec4, 1u > &d, std::int32_t p, const glm::uvec4 &v ) {
  if( p < 0 || p >= d.extent.x ) return;
  d.pixel[ p ] = v;
}

void imageStore( dummy_image< glm::uvec4, 2u > &d, const glm::ivec2 &p, const glm::uvec4 &v ) {
  if( p.x < 0 || p.x >= d.extent.x ) return;
  if( p.y < 0 || p.y >= d.extent.y ) return;
  d.pixel[ p.x + d.extent.x * p.y ] = v;
}

void imageStore( dummy_image< glm::uvec4, 3u > &d, const glm::ivec3 &p, const glm::uvec4 &v ) {
  if( p.x < 0 || p.x >= d.extent.x ) return;
  if( p.y < 0 || p.y >= d.extent.y ) return;
  if( p.z < 0 || p.z >= d.extent.z ) return;
  d.pixel[ p.x + d.extent.x * p.y + d.extent.x * d.extent.y * p.z ] = v;
}

void imageStore( dummy_image< glm::ivec4, 1u > &d, std::int32_t p, const glm::ivec4 &v ) {
  if( p < 0 || p >= d.extent.x ) return;
  d.pixel[ p ] = v;
}

void imageStore( dummy_image< glm::ivec4, 2u > &d, const glm::ivec2 &p, const glm::ivec4 &v ) {
  if( p.x < 0 || p.x >= d.extent.x ) return;
  if( p.y < 0 || p.y >= d.extent.y ) return;
  d.pixel[ p.x + d.extent.x * p.y ] = v;
}

void imageStore( dummy_image< glm::ivec4, 3u > &d, const glm::ivec3 &p, const glm::ivec4 &v ) {
  if( p.x < 0 || p.x >= d.extent.x ) return;
  if( p.y < 0 || p.y >= d.extent.y ) return;
  if( p.z < 0 || p.z >= d.extent.z ) return;
  d.pixel[ p.x + d.extent.x * p.y + d.extent.x * d.extent.y * p.z ] = v;
}

void imageStore( dummy_image< std::uint32_t, 1u > &d, std::int32_t p, const glm::uvec4 &v ) {
  if( p < 0 || p >= d.extent.x ) return;
  d.pixel[ p ] = v.x;
}

void imageStore( dummy_image< std::uint32_t, 2u > &d, const glm::ivec2 &p, const glm::uvec4 &v ) {
  if( p.x < 0 || p.x >= d.extent.x ) return;
  if( p.y < 0 || p.y >= d.extent.y ) return;
  d.pixel[ p.x + d.extent.x * p.y ] = v.x;
}

void imageStore( dummy_image< std::uint32_t, 3u > &d, const glm::ivec3 &p, const glm::uvec4 &v ) {
  if( p.x < 0 || p.x >= d.extent.x ) return;
  if( p.y < 0 || p.y >= d.extent.y ) return;
  if( p.z < 0 || p.z >= d.extent.z ) return;
  d.pixel[ p.x + d.extent.x * p.y + d.extent.x * d.extent.y * p.z ] = v.x;
}

void imageStore( dummy_image< std::int32_t, 1u > &d, std::int32_t p, const glm::ivec4 &v ) {
  if( p < 0 || p >= d.extent.x ) return;
  d.pixel[ p ] = v.x;
}

void imageStore( dummy_image< std::int32_t, 2u > &d, const glm::ivec2 &p, const glm::ivec4 &v ) {
  if( p.x < 0 || p.x >= d.extent.x ) return;
  if( p.y < 0 || p.y >= d.extent.y ) return;
  d.pixel[ p.x + d.extent.x * p.y ] = v.x;
}

void imageStore( dummy_image< std::int32_t, 3u > &d, const glm::ivec3 &p, const glm::ivec4 &v ) {
  if( p.x < 0 || p.x >= d.extent.x ) return;
  if( p.y < 0 || p.y >= d.extent.y ) return;
  if( p.z < 0 || p.z >= d.extent.z ) return;
  d.pixel[ p.x + d.extent.x * p.y + d.extent.x * d.extent.y * p.z ] = v.x;
}

}

