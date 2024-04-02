#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/sampler_pool_create_info.hpp>

namespace gct {
void to_json( nlohmann::json &dest, const sampler_pool_create_info &src ) {
  dest = nlohmann::json::object();
  if( src.allocator ) {
    dest[ "allocator" ] = *src.allocator;
  }
  dest[ "max_sampler_count" ] = src.max_sampler_count;
  if( src.descriptor_set ) {
    dest[ "descriptor_set" ] = *src.descriptor_set;
  }
  dest[ "descriptor_name" ] = src.descriptor_name;
}
}

