#include <glm/common.hpp>
#include <gct/gamma.hpp>

namespace gct {

glm::vec3 srgb_gamma( glm::vec3 v ) {
  return glm::min( glm::max( v / (v + glm::vec3( 0.155, 0.155, 0.155 ) ) * glm::vec3( 1.019, 1.019, 1.019 ), glm::vec3( 0, 0, 0 ) ), glm::vec3( 1, 1, 1 ) );
}

}

