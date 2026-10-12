#ifndef GCT_AREA_H
#define GCT_AREA_H

#include <vector>
#include <glm/vec2.hpp>

namespace gct {

float area( const glm::vec2&, const glm::vec2&, const glm::vec2& );
float area( const std::vector< glm::vec2 > &vertex );

}

#endif
