#include <array>
#include <utility>
#include <glm/ext/matrix_clip_space.hpp>
#include <gct/generate_projection_matrix.hpp>

namespace gct {
std::tuple< float, float > generate_cube_projection_distance(
  const glm::vec3 &camera_pos,
  const std::vector< aabb > &aabbs
) {
  float near = std::numeric_limits< float >::max();
  float far = 0.5f;
  for( const auto &b: aabbs ) {
    auto min = b.min - camera_pos;
    auto max = b.max - camera_pos;
    far = std::max( far, std::max( std::abs( min.x ), std::abs( max.x ) ) );
    if( min.x <= 0.0f && max.x > 0.0f ) {
      near = 0.5f;
    }
    else {
      near = std::min( near, std::max( std::min( std::abs( min.x ), std::abs( max.x ) ), 0.5f ) );
    }
    far = std::max( far, std::max( std::abs( min.y ), std::abs( max.y ) ) );
    if( min.y <= 0.0f && max.y > 0.0f ) {
      near = 0.5f;
    }
    else {
      near = std::min( near, std::max( std::min( std::abs( min.y ), std::abs( max.y ) ), 0.5f ) );
    }
    far = std::max( far, std::max( std::abs( min.z ), std::abs( max.z ) ) );
    if( min.z <= 0.0f && max.z > 0.0f ) {
      near = 0.5f;
    }
    else {
      near = std::min( near, std::max( std::min( std::abs( min.z ), std::abs( max.z ) ), 0.5f ) );
    }
  }
  return std::make_tuple( near, far );
}


std::tuple< glm::mat4, float, float > generate_projection_matrix(
  const glm::mat4 &camera_matrix,
  float fovy,
  std::uint32_t width,
  std::uint32_t height,
  const std::vector< aabb > &aabbs
) {
  std::vector< glm::vec3 > v;
  v.reserve( aabbs.size() * 8u );
  for( const auto &b: aabbs ) {
    v.push_back( glm::vec3( b.min.x, b.min.y, b.min.z ) );
    v.push_back( glm::vec3( b.min.x, b.min.y, b.max.z ) );
    v.push_back( glm::vec3( b.min.x, b.max.y, b.min.z ) );
    v.push_back( glm::vec3( b.min.x, b.max.y, b.max.z ) );
    v.push_back( glm::vec3( b.max.x, b.min.y, b.min.z ) );
    v.push_back( glm::vec3( b.max.x, b.min.y, b.max.z ) );
    v.push_back( glm::vec3( b.max.x, b.max.y, b.min.z ) );
    v.push_back( glm::vec3( b.max.x, b.max.y, b.max.z ) );
  }
  float near = std::numeric_limits< float >::max();
  float far = 0.5f;
  for( const auto &e: v ) {
    auto p = camera_matrix * glm::vec4( e.x, e.y, e.z, 1.0f );
    p /= p.w;
    far = std::max( far, -p.z );
    near = std::min( near, std::max( -p.z/2.0f, 0.5f ) );
  }
  return std::make_tuple(
    glm::perspective( fovy, (float(width)/float(height)), near, far ),
    near,
    far
  );
}

}
