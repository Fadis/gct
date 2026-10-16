#ifndef GCT_LINEAR_INTERPOLATION_H
#define GCT_LINEAR_INTERPOLATION_H

#include <glm/vec2.hpp>
#include <gct/host_image.hpp>

namespace gct {

float linear_interpolation( const host_image &image, std::uint32_t channel, const glm::vec2 p );

}

#endif

