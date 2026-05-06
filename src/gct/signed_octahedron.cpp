#include <cmath>
#include <limits>
#include <algorithm>
#include <glm/geometric.hpp>
#include <gct/signed_octahedron.hpp>

namespace gct {

signed_octahedron encode_signed_octahedron( const glm::vec3 &decoded ) {
  signed_octahedron encoded;
  const glm::vec3 n = decoded / ( std::abs( decoded.x ) + std::abs( decoded.y ) + std::abs( decoded.z ) );
  encoded.v.y = n.y * 0.5f + 0.5f;
  encoded.v.x = n.x * 0.5f + encoded.v.y;
  encoded.v.y = n.x * -0.5f + encoded.v.y;
  encoded.sign = (uint)std::clamp( n.z * std::numeric_limits< float >::max(), 0.0f, 1.0f );
  return encoded;
}

glm::vec3 decode_signed_octahedron( const signed_octahedron &encoded ) {
  glm::vec3 decoded;
  decoded.x = encoded.v.x - encoded.v.y;
  decoded.y = ( encoded.v.x + encoded.v.y ) - 1.0f;
  decoded.z = encoded.sign * 2.01f - 1.0f;
  decoded.z = decoded.z * ( 1.0f - std::abs( decoded.x ) - std::abs( decoded.y ) );
  return glm::normalize( decoded );
}

signed_diamond encode_signed_diamond( const glm::vec2 &decoded ) {
  float m = std::abs( decoded.x ) + std::abs( decoded.y );
  float x = decoded.x / m;
  signed_diamond encoded;
  encoded.sign = ( decoded.y >= 0.0f ) ? 1u : 0u;
  encoded.v = x * 0.5f + 0.5f;
  return encoded;
}

glm::vec2 decode_signed_diamond( const signed_diamond &encoded ) {
  glm::vec2 decoded;
  const float s = encoded.sign ? 1.0f : -1.0f;
  decoded.x = 2.0f * encoded.v - 1.0f;
  decoded.y = s * ( 1.0f - std::abs( decoded.x ) );
  return glm::normalize( decoded );
}

}

