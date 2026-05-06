#ifndef GCT_N21T11_HPP
#define GCT_N21T11_HPP

#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <gct/signed_octahedron.hpp>

namespace gct {

std::uint32_t n21t11_pack( const signed_octahedron &normal );
std::uint32_t n21t11_pack( const signed_octahedron &normal, const signed_diamond &tangent );
signed_octahedron n21t11_unpack_normal( std::uint32_t packed );
signed_diamond n21t11_unpack_tangent( std::uint32_t packed );
std::uint32_t n21t11_encode( const glm::vec3 &normal, const glm::vec3 &tangent );
glm::vec3 n21t11_decode_normal( std::uint32_t packed );
glm::vec3 n21t11_decode_tangent( std::uint32_t packed, const glm::vec3 &normal );

}

#endif

