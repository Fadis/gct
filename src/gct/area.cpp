#include<vector>
#include <cmath>
#include <gct/area.hpp>
#include <gct/jc_voronoi.h>
#include <gct/triangulation.hpp>

namespace gct {

float area( const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c ) {
  return 0.5f * std::abs( ( a.x - c.x ) * ( b.y - c.y ) - ( b.x - c.x ) * ( a.y - c.y ) );
}

float area( const std::vector< glm::vec2 > &vertex ) {
  const auto t = ear_clipping_triangulation( vertex );
  float sum = 0.0f;
  for( const auto &v: t ) {
    sum += area( v[ 0 ], v[ 1 ], v[ 2 ] );
  }
  return sum;
}

}

