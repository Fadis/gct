#ifndef GCT_DUMMY_IMAGE_HPP
#define GCT_DUMMY_IMAGE_HPP

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


namespace gct {

template< typename T, unsigned int d >
struct dummy_image {
  glm::uvec3 extent = glm::uvec3{ 0, 0, 0 };
  std::vector< T > pixel;
};
template< typename T, unsigned int d >
struct dummy_image_array {
  glm::uvec3 extent = glm::uvec3{ 0, 0, 0 };
  std::vector< dummy_image< T, d > > layer;
};

extern std::unordered_map< std::uint32_t, dummy_image< glm::vec4, 1u > > image_pool_1d;
extern std::unordered_map< std::uint32_t, dummy_image< glm::vec4, 2u > > image_pool_2d;
extern std::unordered_map< std::uint32_t, dummy_image< glm::vec4, 3u > > image_pool_3d;
extern std::unordered_map< std::uint32_t, dummy_image_array< glm::vec4, 1u > > image_pool_1d_array;
extern std::unordered_map< std::uint32_t, dummy_image_array< glm::vec4, 2u > > image_pool_2d_array;
extern std::unordered_map< std::uint32_t, dummy_image< glm::uvec4, 1u > > image_pool_1du;
extern std::unordered_map< std::uint32_t, dummy_image< glm::uvec4, 2u > > image_pool_2du;
extern std::unordered_map< std::uint32_t, dummy_image< glm::uvec4, 3u > > image_pool_3du;
extern std::unordered_map< std::uint32_t, dummy_image_array< glm::uvec4, 1u > > image_pool_1du_array;
extern std::unordered_map< std::uint32_t, dummy_image_array< glm::uvec4, 2u > > image_pool_2du_array;
extern std::unordered_map< std::uint32_t, dummy_image< std::int32_t, 1u > > image_pool_1da;
extern std::unordered_map< std::uint32_t, dummy_image< std::int32_t, 2u > > image_pool_2da;
extern std::unordered_map< std::uint32_t, dummy_image< std::int32_t, 3u > > image_pool_3da;
extern std::unordered_map< std::uint32_t, dummy_image_array< std::int32_t, 1u > > image_pool_1da_array;
extern std::unordered_map< std::uint32_t, dummy_image_array< std::int32_t, 2u > > image_pool_2da_array;
extern std::unordered_map< std::uint32_t, dummy_image< std::uint32_t, 1u > > image_pool_1dua;
extern std::unordered_map< std::uint32_t, dummy_image< std::uint32_t, 2u > > image_pool_2dua;
extern std::unordered_map< std::uint32_t, dummy_image< std::uint32_t, 3u > > image_pool_3dua;
extern std::unordered_map< std::uint32_t, dummy_image_array< std::uint32_t, 1u > > image_pool_1dua_array;
extern std::unordered_map< std::uint32_t, dummy_image_array< std::uint32_t, 2u > > image_pool_2dua_array;

template< typename T >
T imageLoad( const dummy_image< T, 1u > &d, std::int32_t p ) {
  if( p < 0 || p >= d.extent.x ) return T( 0 );
  return d.pixel[ p ];
}

template< typename T >
T imageLoad( const dummy_image< T, 2u > &d, const glm::ivec2 &p ) {
  if( p.x < 0 || p.x >= d.extent.x ) return T( 0 );
  if( p.y < 0 || p.y >= d.extent.y ) return T( 0 );
  return d.pixel[ p.x + d.extent.x * p.y ];
}

template< typename T >
T imageLoad( const dummy_image< T, 3u > &d, const glm::ivec3 &p ) {
  if( p.x < 0 || p.x >= d.extent.x ) return T( 0 );
  if( p.y < 0 || p.y >= d.extent.y ) return T( 0 );
  if( p.z < 0 || p.z >= d.extent.z ) return T( 0 );
  return d.pixel[ p.x + d.extent.x * p.y + d.extent.x * d.extent.y * p.z ];
}

template< typename T >
T imageLoad( const dummy_image_array< T, 1u > &d, const glm::ivec2 &p ) {
  if( p.y < 0 || p.y >= d.layer.size() ) return T( 0 );
  return imageLoad( d.layer[ p.y ], p.x );
}

template< typename T >
T imageLoad( const dummy_image_array< T, 2u > &d, const glm::ivec3 &p ) {
  if( p.z < 0 || p.z >= d.layer.size() ) return T( 0 );
  return imageLoad( d.layer[ p.z ], glm::ivec2( p.x, p.y ) );
}

void imageStore( dummy_image< glm::vec4, 1u > &d, std::int32_t p, const glm::vec4 &v );
void imageStore( dummy_image< glm::vec4, 2u > &d, const glm::ivec2 &p, const glm::vec4 &v );
void imageStore( dummy_image< glm::vec4, 3u > &d, const glm::ivec3 &p, const glm::vec4 &v );
void imageStore( dummy_image< glm::uvec4, 1u > &d, std::int32_t p, const glm::uvec4 &v );
void imageStore( dummy_image< glm::uvec4, 2u > &d, const glm::ivec2 &p, const glm::uvec4 &v );
void imageStore( dummy_image< glm::uvec4, 3u > &d, const glm::ivec3 &p, const glm::uvec4 &v );
void imageStore( dummy_image< glm::ivec4, 1u > &d, std::int32_t p, const glm::ivec4 &v );
void imageStore( dummy_image< glm::ivec4, 2u > &d, const glm::ivec2 &p, const glm::ivec4 &v );
void imageStore( dummy_image< glm::ivec4, 3u > &d, const glm::ivec3 &p, const glm::ivec4 &v );
void imageStore( dummy_image< std::uint32_t, 1u > &d, std::int32_t p, const glm::uvec4 &v );
void imageStore( dummy_image< std::uint32_t, 2u > &d, const glm::ivec2 &p, const glm::uvec4 &v );
void imageStore( dummy_image< std::uint32_t, 3u > &d, const glm::ivec3 &p, const glm::uvec4 &v );
void imageStore( dummy_image< std::int32_t, 1u > &d, std::int32_t p, const glm::ivec4 &v );
void imageStore( dummy_image< std::int32_t, 2u > &d, const glm::ivec2 &p, const glm::ivec4 &v );
void imageStore( dummy_image< std::int32_t, 3u > &d, const glm::ivec3 &p, const glm::ivec4 &v );

dummy_image< glm::vec4, 2u > &create_dummy_image_2d( std::uint32_t index, std::uint32_t x, std::uint32_t y );

template< typename T >
void imageStore( dummy_image_array< T, 1u > &d, const glm::ivec2 &p, const glm::vec4 &v ) {
  if( p.y < 0 || p.y >= d.layer.size() ) return T( 0 );
  imageStore( d.layer[ p.y ], p.x, v );
}

template< typename T >
void imageStore( dummy_image_array< T, 2u > &d, const glm::ivec3 &p, const glm::vec4 &v ) {
  if( p.z < 0 || p.z >= d.layer.size() ) return T( 0 );
  imageStore( d.layer[ p.z ], glm::ivec2( p.x, p.y ), v );
}


template< typename T >
std::int32_t imageSize( dummy_image< T, 1u > &d ) {
  return d.extent.x;
}

template< typename T >
glm::ivec2 imageSize( dummy_image< T, 2u > &d ) {
  return glm::ivec2( d.extent.x, d.extent.y );
}

template< typename T >
glm::ivec3 imageSize( dummy_image< T, 3u > &d ) {
  return d.extent;
}

template< typename T >
glm::ivec2 imageSize( dummy_image_array< T, 1u > &d ) {
  return glm::ivec2( d.extent.x, d.layer.size() );
}

template< typename T >
glm::ivec3 imageSize( dummy_image_array< T, 2u > &d ) {
  return glm::ivec3( d.extent.x, d.extent.y, d.layer.size() );
}


}

#endif

