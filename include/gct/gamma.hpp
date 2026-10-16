#ifndef GCT_GAMMA_HPP
#define GCT_GAMMA_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace gct {

glm::vec3 srgb_gamma( const glm::vec3 &v );

float srgb_degamma( float v );
glm::vec2 srgb_degamma( const glm::vec2 &v );
glm::vec3 srgb_degamma( const glm::vec3 &v );

}

#endif

