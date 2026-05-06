#ifndef GCT_DUMMY_BYTE_ADDRESS_BUFFER_HPP
#define GCT_DUMMY_BYTE_ADDRESS_BUFFER_HPP

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace gct {

std::unordered_map< std::uint32_t, std::vector< std::uint8_t > > &get_vertex_buffer();
std::uint32_t vertex_buffer_load( std::uint32_t vertex_buffer, std::uint32_t offset );
glm::uvec2 vertex_buffer_load2( std::uint32_t vertex_buffer, std::uint32_t offset );
glm::uvec3 vertex_buffer_load3( std::uint32_t vertex_buffer, std::uint32_t offset );
glm::uvec4 vertex_buffer_load4( std::uint32_t vertex_buffer, std::uint32_t offset );

}

#endif

