#include <cmath>
#include <gct/simeq.hpp>

namespace gct {

bool simeq( const float &a, const float &b, const float threshold ) {
  return
    ( std::abs( b - a ) ) < threshold;
}

bool simeq( const glm::vec3 &a, const glm::vec2 &b, const float threshold ) {
  return
    ( std::abs( b.x - a.x ) + std::abs( b.y -a.y ) ) < threshold;
}

bool simeq( const glm::vec3 &a, const glm::vec3 &b, const float threshold ) {
  return
    ( std::abs( b.x - a.x ) + std::abs( b.y -a.y ) + std::abs( b.z - a.z ) ) < threshold;
}

bool simeq( const glm::vec4 &a, const glm::vec4 &b, const float threshold ) {
  return
    ( std::abs( b.x - a.x ) + std::abs( b.y -a.y ) + std::abs( b.z - a.z ) + std::abs( b.w - a.w ) ) < threshold;
}

}

