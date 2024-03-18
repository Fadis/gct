#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/texture_pool_create_info.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const texture_pool_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator" ] = *src.allocator;
  dest[ "descriptor_pool" ] = *src.descriptor_pool;
  dest[ "pipeline_cache" ] = *src.pipeline_cache;
  dest[ "max_texture_count" ] = src.max_texture_count;
}

}


