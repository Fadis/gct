#include <gct/project.hpp>
#include <gct/n21t11.hpp>

namespace gct {

std::uint32_t n21t11_pack( const signed_octahedron &normal ) {
  return
    ( normal.sign << 31 ) |
    ( (std::uint32_t)( normal.v.x * 0x3FF ) << 20 ) |
    ( (std::uint32_t)( normal.v.y * 0x3FF ) << 10 );
}


std::uint32_t n21t11_pack( const signed_octahedron &normal, const signed_diamond &tangent ) {
  return
    ( normal.sign << 31 ) |
    ( tangent.sign << 30 ) |
    ( (std::uint32_t)( normal.v.x * 0x3FF ) << 20 ) |
    ( (std::uint32_t)( normal.v.y * 0x3FF ) << 10 ) |
    ( (std::uint32_t)( tangent.v * 0x3FF ) );
}

signed_octahedron n21t11_unpack_normal( std::uint32_t packed ) {
  return signed_octahedron{
    glm::vec2{
      (float)( ( packed >> 20 ) & 0x3FF ) / 0x3FF,
      (float)( ( packed >> 10 ) & 0x3FF ) / 0x3FF
    },
    ( packed >> 31 ) & 0x1u
  };
}

signed_diamond n21t11_unpack_tangent( std::uint32_t packed ) {
  return signed_diamond{
    (float)( packed & 0x3FF ) / 0x3FF,
    ( packed >> 30 ) & 0x1u
  };
}

std::uint32_t n21t11_encode( const glm::vec3 &normal, const glm::vec3 &tangent ) {
  const auto encoded_normal = encode_signed_octahedron( normal );
  const auto early_packed = n21t11_pack( encoded_normal );
  const auto early_unpacked_normal = n21t11_unpack_normal( early_packed );
  const auto aligned_normal = decode_signed_octahedron( early_unpacked_normal );
  const auto projected_tangent = dimension_reduce( tangent, aligned_normal );
  const auto encoded_tangent = encode_signed_diamond( projected_tangent );
  const auto packed = n21t11_pack( encoded_normal, encoded_tangent );
  return packed;
}

glm::vec3 n21t11_decode_normal( std::uint32_t packed ) {
  const auto encoded_normal = n21t11_unpack_normal( packed );
  return decode_signed_octahedron( encoded_normal );
}

glm::vec3 n21t11_decode_tangent( std::uint32_t packed, const glm::vec3 &normal ) {
  const auto encoded_tangent = n21t11_unpack_tangent( packed );
  const auto projected_tangent = decode_signed_diamond( encoded_tangent );
  return dimension_expand( projected_tangent, normal );
}

}

