#include <glm/common.hpp>
#include <gct/gamma.hpp>

namespace gct {

glm::vec3 srgb_gamma( const glm::vec3 &v ) {
  return glm::min( glm::max( v / (v + glm::vec3( 0.155, 0.155, 0.155 ) ) * glm::vec3( 1.019, 1.019, 1.019 ), glm::vec3( 0, 0, 0 ) ), glm::vec3( 1, 1, 1 ) );
}

float srgb_degamma( float v ) {
  return ( v < 0.04045f ) ? ( v / 12.92f ) : std::pow( ( v + 0.055f ) / 1.055f, 2.4f );
}

glm::vec2 srgb_degamma( const glm::vec2 &v ) {
  return glm::vec2( srgb_degamma( v.x ), srgb_degamma( v.y ) );
}

glm::vec3 srgb_degamma( const glm::vec3 &v ) {
  return glm::vec3( srgb_degamma( v.x ), srgb_degamma( v.y ), srgb_degamma( v.z ) );
}

}

