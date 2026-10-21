#include <array>
#include <vector>
#include <iterator>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

namespace gct {

namespace {

glm::vec2 rot90( const glm::vec2 &a ) {
  return glm::vec2{ -a.y, a.x };
}

}

bool intersect(
  const std::array< glm::vec2, 2u > &a,
  const std::array< glm::vec2, 2u > &b,
  bool edge
) {
  if( a[ 0 ] == b[ 0 ] || a[ 0 ] == b[ 1 ] || a[ 1 ] == b[ 0 ] || a[ 1 ] == b[ 1 ] ) return edge;
  auto an = rot90( normalize( a[ 1 ] - a[ 0 ] ) );
  auto a_projected = dot( an, a[ 0 ] );
  auto b_begin_projected = dot( an, b[ 0 ] );
  auto b_end_projected = dot( an, b[ 1 ] );
  auto bn = rot90( normalize( b[ 1 ] - b[ 0 ] ) );
  auto b_projected = dot( bn, b[ 0 ] );
  auto a_begin_projected = dot( bn, a[ 0 ] );
  auto a_end_projected = dot( bn, a[ 1 ] );
  if( a_begin_projected > a_end_projected ) std::swap( a_begin_projected, a_end_projected );
  if( b_begin_projected > b_end_projected ) std::swap( b_begin_projected, b_end_projected );
  if( edge ) {
    return
      ( a_begin_projected <= b_projected && b_projected <= a_end_projected ) &&
      ( b_begin_projected <= a_projected && a_projected <= b_end_projected );
  }
  else {
    return
      ( a_begin_projected < b_projected && b_projected < a_end_projected ) &&
      ( b_begin_projected < a_projected && a_projected < b_end_projected );
  }
}

}

