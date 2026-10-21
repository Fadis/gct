#include <gct/cross2.hpp>

namespace gct {

float rasterization_cross2( glm::vec2 a, glm::vec2 b ){ 
	return a.x * b.y - a.y * b.x; 
}

}

