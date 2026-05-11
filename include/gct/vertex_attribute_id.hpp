#ifndef GCT_VERTEX_ATTRIBUTE_ID_H
#define GCT_VERTEX_ATTRIBUTE_ID_H

#include <string>
#include <nlohmann/json_fwd.hpp>

namespace gct {

enum class vertex_attribute_id {
  index = 0,
  position = 1,
  normal = 2,
  tangent = 3,
  texcoord_0 = 4,
  color_0 = 5,
  joint_0 = 6,
  weight_0 = 7,
  lod_morph = 8
};

vertex_attribute_id to_vertex_attribute_id( const std::string &src );
void to_json( nlohmann::json &dest, const vertex_attribute_id &src );
void from_json( const nlohmann::json &src, vertex_attribute_id &dest );

}

#endif

