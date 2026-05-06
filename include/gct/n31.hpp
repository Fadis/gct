#ifndef GCT_N31_HPP
#define GCT_N31_HPP

#include <cstdint>
#include <glm/vec3.hpp>
#include <gct/signed_octahedron.hpp>

namespace gct {

std::uint32_t n31_pack( const signed_octahedron &normal );
signed_octahedron n31_unpack_normal( std::uint32_t packed );
std::uint32_t n31_encode( const glm::vec3 &normal );
glm::vec3 n31_decode( std::uint32_t packed );

}

#endif

