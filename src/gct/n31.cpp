#include <gct/project.hpp>
#include <gct/n31.hpp>

namespace gct {

std::uint32_t n31_pack( const signed_octahedron &normal ) {
  return
    ( normal.sign << 30 ) |
    ( (std::uint32_t)( normal.v.x * 0x7FFF ) << 15 ) |
    ( (std::uint32_t)( normal.v.y * 0x7FFF ) );
}

signed_octahedron n31_unpack( std::uint32_t packed ) {
  return signed_octahedron{
    glm::vec2{
      (float)( ( packed >> 15 ) & 0x7FFF ) / 0x7FFF,
      (float)( ( packed ) & 0x7FFF ) / 0x7FFF
    },
    ( packed >> 30 ) & 0x1u
  };
}

std::uint32_t n31_encode( const glm::vec3 &normal ) {
  const auto encoded_normal = encode_signed_octahedron( normal );
  const auto packed = n31_pack( encoded_normal );
  return packed;
}

glm::vec3 n31_decode( std::uint32_t packed ) {
  const auto encoded_normal = n31_unpack( packed );
  return decode_signed_octahedron( encoded_normal );
}

}

