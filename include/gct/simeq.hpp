#ifndef GCT_SIMEQ_HPP
#define GCT_SIMEQ_HPP
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


namespace gct {

bool simeq( const float &a, const float &b, const float threshold );
bool simeq( const glm::vec3 &a, const glm::vec2 &b, const float threshold );
bool simeq( const glm::vec3 &a, const glm::vec3 &b, const float threshold );
bool simeq( const glm::vec4 &a, const glm::vec4 &b, const float threshold );

}

#endif

