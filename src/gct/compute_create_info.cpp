#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
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
  dest[ "allocator" ] = *src.allocator;
  dest[ "descriptor_pool" ] = *src.descriptor_pool;
  dest[ "pipeline_cache" ] = *src.pipeline_cache;
  dest[ "shader" ] = src.shader.string();
  dest[ "resources" ] = src.resources;
}

}

