#ifndef GCT_MESHLET_TYPE_H
#define GCT_MESHLET_TYPE_H

#include <glm/vec4.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>

namespace gct {

struct meshlet_type {
  LIBGCT_SETTER( center )
  LIBGCT_SETTER( normal )
  glm::vec4 center; // 頂点座標の平均と全ての頂点を内包する球の半径
  glm::vec4 normal; // 法錐の軸と-sin(alpha)
};

void to_json( nlohmann::json &dest, const meshlet_type &src );
void from_json( const nlohmann::json &dest, meshlet_type &src );

}

#endif

