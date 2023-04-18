#ifndef GCT_SPECTRUM_HPP
#define GCT_SPECTRUM_HPP

#include <map>
#include <glm/glm.hpp>

namespace gct {
extern const std::map< unsigned int, glm::vec3 > spectrum;
glm::vec3 wavelength_to_color( float );
glm::vec3 level_to_color( double value, double min, double max );
}

#endif
