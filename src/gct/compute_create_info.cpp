#include <nlohmann/json.hpp>
#include <gct/named_resource.hpp>
#include <gct/compute_create_info.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const compute_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "shader" ] = src.shader.string();
  dest[ "resources" ] = src.resources;
}

}

