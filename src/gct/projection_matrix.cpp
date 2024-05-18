#include <cstdlib>
#include <glm/fwd.hpp>
#include <vector>
#include <nlohmann/json.hpp>
#include <glm/vec4.hpp>
#include <gct/projection_matrix.hpp>

namespace gct {

float split_depth(
  float near,
  float far,
  std::uint32_t i,
  std::uint32_t m
) {
  return near * std::pow( far / near, float( i ) / float( m ) );
}

glm::mat4 split_projection_matrix(
  const glm::mat4 &projection,
  std::uint32_t i,
  std::uint32_t m
) {
  const auto znear = projection[ 3 ][ 2 ] / ( -1 + projection[ 2 ][ 2 ] );
  const auto zfar = projection[ 3 ][ 2 ] / ( 1 + projection[ 2 ][ 2 ] );
  const auto pnear = split_depth( znear, zfar, i, m );
  const auto pfar = split_depth( znear, zfar, i + 1u, m );
  auto temp = projection;
  temp[ 2 ][ 2 ] = ( pfar + pnear ) / ( pnear - pfar );
  temp[ 3 ][ 2 ] = 2.f * pfar * pnear / ( pnear - pfar );
  return temp;
}

aabb get_clipping_aabb(
  const glm::mat4 &matrix
) {
  const auto inv = glm::inverse( matrix );
  std::vector< glm::vec4 > vertex{
    inv * glm::vec4( -1.f, -1.f, -1.f, 1.f ),
    inv * glm::vec4( -1.f, -1.f,  1.f, 1.f ),
    inv * glm::vec4( -1.f,  1.f, -1.f, 1.f ),
    inv * glm::vec4( -1.f,  1.f,  1.f, 1.f ),
    inv * glm::vec4(  1.f, -1.f, -1.f, 1.f ),
    inv * glm::vec4(  1.f, -1.f,  1.f, 1.f ),
    inv * glm::vec4(  1.f,  1.f, -1.f, 1.f ),
    inv * glm::vec4(  1.f,  1.f,  1.f, 1.f )
  };
  return from_vertices( vertex );
}

std::vector< aabb > get_clipping_aabb(
  const glm::mat4 &projection_matrix,
  const glm::mat4 &camera_matrix,
  std::uint32_t m
) {
  std::vector< aabb > temp;
  for( unsigned int i = 0u; i != m; ++i ) {
    const auto p = split_projection_matrix( projection_matrix, i, m );
    temp.push_back( get_clipping_aabb( p * camera_matrix ) );
  }
  return temp;
}

std::vector< std::pair< glm::vec4, glm::vec4 > > get_clippling_area_edges(
  const glm::mat4 &projection_matrix,
  const glm::mat4 &camera_matrix
) {
  const auto inv = glm::inverse( projection_matrix * camera_matrix );
  std::array< glm::vec4, 8u > vertex{
    inv * glm::vec4( -1.f, -1.f, -1.f, 1.f ),
    inv * glm::vec4( -1.f, -1.f,  1.f, 1.f ),
    inv * glm::vec4( -1.f,  1.f, -1.f, 1.f ),
    inv * glm::vec4( -1.f,  1.f,  1.f, 1.f ),
    inv * glm::vec4(  1.f, -1.f, -1.f, 1.f ),
    inv * glm::vec4(  1.f, -1.f,  1.f, 1.f ),
    inv * glm::vec4(  1.f,  1.f, -1.f, 1.f ),
    inv * glm::vec4(  1.f,  1.f,  1.f, 1.f )
  };
  for( auto &v: vertex ) {
    v /= v.w;
  }
  const std::array< unsigned int, 24 > indices{
    0,1,
    1,5,
    5,4,
    4,0,
    2,3,
    3,7,
    7,6,
    6,2,
    0,2,
    4,6,
    1,3,
    5,7
  };
  std::vector< std::pair< glm::vec4, glm::vec4 > > edges;
  for( unsigned int i = 0u; i != 12u; ++i ) {
    edges.push_back( std::make_pair( vertex[ indices[ i * 2u ] ], vertex[ indices[ i * 2u + 1u ] ] ) );
  }
  return edges;
}

std::vector< glm::vec4 > get_far_plane(
  const glm::mat4 &projection_matrix,
  const glm::mat4 &camera_matrix
) {
  const auto inv = glm::inverse( projection_matrix * camera_matrix );
  std::vector< glm::vec4 > vertex{
    inv * glm::vec4( -1.f, -1.f,  1.f, 1.f ),
    inv * glm::vec4( -1.f,  1.f,  1.f, 1.f ),
    inv * glm::vec4(  1.f, -1.f,  1.f, 1.f ),
    inv * glm::vec4(  1.f,  1.f,  1.f, 1.f )
  };
  for( auto &v: vertex ) {
    v /= v.w;
  }
  return vertex;
}

std::vector< glm::vec4 > get_near_plane(
  const glm::mat4 &projection_matrix,
  const glm::mat4 &camera_matrix
) {
  const auto inv = glm::inverse( projection_matrix * camera_matrix );
  std::vector< glm::vec4 > vertex{
    inv * glm::vec4( -1.f, -1.f, -1.f, 1.f ),
    inv * glm::vec4( -1.f,  1.f, -1.f, 1.f ),
    inv * glm::vec4(  1.f, -1.f, -1.f, 1.f ),
    inv * glm::vec4(  1.f,  1.f, -1.f, 1.f )
  };
  for( auto &v: vertex ) {
    v /= v.w;
  }
  return vertex;
}

aabb get_minimum_projection_matrix(
  const glm::mat4 &projection_matrix,
  const glm::mat4 &camera_matrix,
  const std::vector< glm::vec4 > &p,
  float near
) {
  std::vector< glm::vec4 > cam_p;
  for( const auto &v: p ) {
    cam_p.push_back( camera_matrix * glm::vec4( v.x, -v.y, v.z, v.w ) );
  }
  for( auto &v: cam_p ) {
    v.x *= near/glm::length(v);
    v.y *= near/glm::length(v);
  }
  auto min = cam_p[ 0 ];
  auto max = cam_p[ 0 ];
  for( unsigned int i = 0u; i != 4u; ++i ) {
    min = glm::min( min, cam_p[ i ] );
    max = glm::max( max, cam_p[ i ] );
  }
  return
    aabb()
      .emplace_min( min.x, min.y, min.z )
      .emplace_max( max.x, max.y, max.z );
}


bool is_visible(
  const glm::mat4 &matrix,
  const aabb &box
) {
  auto m = ( matrix * box );
  auto c = m && get_unit_aabb();
  return ( matrix * box ) && get_unit_aabb(); 
}

bool is_visible(
  const glm::mat4 &matrix,
  const aabb4 &box
) {
  return ( matrix * box ) && get_unit_aabb4(); 
}

}

