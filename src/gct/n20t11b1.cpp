#include <gct/project.hpp>
#include <gct/n20t11b1.hpp>

namespace gct {

std::uint32_t n20t11b1_pack( const signed_octahedron &normal ) {
  return
    ( normal.sign << 31 ) |
    ( (std::uint32_t)( normal.v.x * 0x3FF ) << 20 ) |
    ( (std::uint32_t)( normal.v.y * 0x1FF ) << 11 );
}


std::uint32_t n20t11b1_pack( const signed_octahedron &normal, const signed_diamond &tangent ) {
  return
    ( normal.sign << 31 ) |
    ( tangent.sign << 30 ) |
    ( (std::uint32_t)( normal.v.x * 0x3FF ) << 20 ) |
    ( (std::uint32_t)( normal.v.y * 0x1FF ) << 11 ) |
    ( tangent.flip << 10 ) |
    ( (std::uint32_t)( tangent.v * 0x3FF ) );
}

signed_octahedron n20t11b1_unpack_normal( std::uint32_t packed ) {
  return signed_octahedron{
    glm::vec2{
      (float)( ( packed >> 20 ) & 0x3FF ) / 0x3FF,
      (float)( ( packed >> 11 ) & 0x1FF ) / 0x1FF
    },
    ( packed >> 31 ) & 0x1u
  };
}

signed_diamond n20t11b1_unpack_tangent( std::uint32_t packed ) {
  return signed_diamond{
    (float)( packed & 0x3FF ) / 0x3FF,
    ( packed >> 30 ) & 0x1u,
    ( packed >> 10 ) & 0x1u
  };
}

std::uint32_t n20t11b1_encode( const glm::vec3 &normal, const glm::vec4 &tangent ) {
  const auto encoded_normal = encode_signed_octahedron( normal );
  const auto early_packed = n20t11b1_pack( encoded_normal );
  const auto early_unpacked_normal = n20t11b1_unpack_normal( early_packed );
  const auto aligned_normal = decode_signed_octahedron( early_unpacked_normal );
  const auto projected_tangent = dimension_reduce( tangent, aligned_normal );
  auto encoded_tangent = encode_signed_diamond( projected_tangent );
  encoded_tangent.flip = tangent.w < 0.0 ? 1u : 0u;
  const auto packed = n20t11b1_pack( encoded_normal, encoded_tangent );
  return packed;
}

glm::vec3 n20t11b1_decode_normal( std::uint32_t packed ) {
  const auto encoded_normal = n20t11b1_unpack_normal( packed );
  return decode_signed_octahedron( encoded_normal );
}

glm::vec4 n20t11b1_decode_tangent( std::uint32_t packed, const glm::vec3 &normal ) {
  const auto encoded_tangent = n20t11b1_unpack_tangent( packed );
  const auto projected_tangent = decode_signed_diamond( encoded_tangent );
  const glm::vec3 tangent = dimension_expand( projected_tangent, normal );
  return glm::vec4( tangent.x, tangent.y, tangent.z, encoded_tangent.flip ? -1.0 : 1.0 );
}

}

