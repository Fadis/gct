#include <nlohmann/json.hpp>
#include <gct/named_resource.hpp>
#include <gct/compute_create_info.hpp>

namespace gct {

compute_create_info &compute_create_info::add_resource(
  const named_resource &n
) {
  resources.push_back( n );
  return *this;
}
compute_create_info &compute_create_info::clear_resource(
  const named_resource &n
) {
  resources.clear();
  return *this;
}
void to_json( nlohmann::json &dest, const compute_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "shader" ] = src.shader.string();
  dest[ "resources" ] = src.resources;
}

}

