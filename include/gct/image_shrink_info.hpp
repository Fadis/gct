#ifndef GCT_IMAGE_SHRINK_INFO_HPP
#define GCT_IMAGE_SHRINK_INFO_HPP

#include <nlohmann/json_fwd.hpp>
#include <glm/vec3.hpp>
#include <gct/setter.hpp>

namespace gct {
struct image_shrink_info {
  LIBGCT_SETTER(extent)
  glm::uvec3 extent = glm::uvec3( 1u, 1u, 1u );
};

void to_json( nlohmann::json &root, const image_shrink_info &v );
void from_json( const nlohmann::json &root, image_shrink_info &v );

}

#endif

