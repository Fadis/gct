#include <cmath>
#include <gct/uniform_grid.hpp>

namespace gct {

std::vector< glm::vec2 > generate_uniform_grid(
  std::uint32_t width,
  std::uint32_t height
) {
  std::vector< glm::vec2 > point;
  for( std::uint32_t y = 0u; y != height; ++y ) {
    for( std::uint32_t x = 0u; x != width; ++x ) {
      point.push_back( glm::vec2( float( x )/float( width - 1u ), float( y )/float( height - 1u ) ) );
    }
  }
  return point;
}

std::vector< glm::vec2 > generate_uniform_grid(
  std::uint32_t point_count,
  const glm::vec2 &scale
) {
  const auto width = std::uint32_t( sqrt( float( point_count ) )/sqrt( scale.y/scale.x ) );
  const auto height = point_count/width;
  return generate_uniform_grid( width, height );
}


}

