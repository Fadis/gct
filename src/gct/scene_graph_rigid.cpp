#include <nlohmann/json.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/scene_graph_rigid.hpp>

namespace gct::scene_graph {

void to_json( nlohmann::json &dest, const rigid_descriptor &src ) {
  dest = nlohmann::json::object();
  if( src.trs ) {
    dest[ "trs" ] = *src.trs;
  }
  if( src.trs_previous ) {
    dest[ "trs_previous" ] = *src.trs_previous;
  }
  if( src.collision_constraint ) {
    dest[ "collision_constraint" ] = *src.collision_constraint;
  }
};

void to_json( nlohmann::json &dest, const rigid &src ) {
  dest = nlohmann::json::object();
  dest[ "descriptor" ] = src.descriptor;
}

}


