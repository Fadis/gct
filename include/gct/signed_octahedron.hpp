#ifndef GCT_SIGNED_OCTAHEDRON_HPP
#define GCT_SIGNED_OCTAHEDRON_HPP

#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace gct {

struct signed_octahedron {
  glm::vec2 v;
  std::uint32_t sign;
  std::uint32_t reserved;
};

struct signed_diamond {
  float v;
  std::uint32_t sign;
};

signed_octahedron encode_signed_octahedron( const glm::vec3 &decoded );
glm::vec3 decode_signed_octahedron( const signed_octahedron &encoded );
signed_diamond encode_signed_diamond( const glm::vec2 &decoded );
glm::vec2 decode_signed_diamond( const signed_diamond &encoded );

}

#endif

