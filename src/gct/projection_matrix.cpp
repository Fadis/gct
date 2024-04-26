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

