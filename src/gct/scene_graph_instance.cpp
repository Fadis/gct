#include <nlohmann/json.hpp>
#include <vulkan2json/IndexType.hpp>
#include <vulkan2json/VertexInputBindingDescription.hpp>
#include <vulkan2json/VertexInputAttributeDescription.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/scene_graph_instance.hpp>

namespace gct::scene_graph {

void to_json( nlohmann::json &dest, const instance_descriptor &src ) {
  dest = nlohmann::json::object();
  if( src.matrix ) {
    dest[ "matrix" ] = *src.matrix;
  }
  if( src.aabb ) {
    dest[ "aabb" ] = *src.aabb;
  }
  if( src.resource_index ) {
    dest[ "resource_index" ] = *src.resource_index;
  }
  if( src.prim ) {
    dest[ "primitive" ] = *src.prim;
  }
  if( src.visibility ) {
    dest[ "visibility" ] = *src.visibility;
  }
  if( src.meshlet_index ) {
    dest[ "meshlet_index" ] = *src.meshlet_index;
  }
};

void to_json( nlohmann::json &dest, const instance &src ) {
  dest = nlohmann::json::object();
  dest[ "descriptor" ] = src.descriptor;
}

}

