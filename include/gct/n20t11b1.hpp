#ifndef GCT_N20T11b1_HPP
#define GCT_N20T11b1_HPP

#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <gct/signed_octahedron.hpp>

namespace gct {

std::uint32_t n20t11b1_pack( const signed_octahedron &normal );
std::uint32_t n20t11b1_pack( const signed_octahedron &normal, const signed_diamond &tangent );
signed_octahedron n20t11b1_unpack_normal( std::uint32_t packed );
signed_diamond n20t11b1_unpack_tangent( std::uint32_t packed );
std::uint32_t n20t11b1_encode( const glm::vec3 &normal, const glm::vec4 &tangent );
glm::vec3 n20t11b1_decode_normal( std::uint32_t packed );
glm::vec4 n20t11b1_decode_tangent( std::uint32_t packed, const glm::vec3 &normal );

}

#endif

