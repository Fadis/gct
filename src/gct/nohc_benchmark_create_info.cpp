#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/named_resource.hpp>
#include <gct/nohc_benchmark_create_info.hpp>

namespace gct {

nohc_benchmark_create_info &nohc_benchmark_create_info::add_resource(
  const named_resource &n
) {
  resources.push_back( n );
  return *this;
}
nohc_benchmark_create_info &nohc_benchmark_create_info::clear_resource() {
  resources.clear();
  return *this;
}

void to_json( nlohmann::json &dest, const nohc_benchmark_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "shader" ] = src.shader.string();
  dest[ "width" ] = src.width;
  dest[ "height" ] = src.height;
  dest[ "color_buffer_count" ] = src.color_buffer_count;
  dest[ "resources" ] = src.resources;
}

}


