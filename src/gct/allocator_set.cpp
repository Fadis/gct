#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/allocator_set.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const allocator_set_t &src ) {
  dest = nlohmann::json::object();
  if( src.allocator ) {
    dest[ "allocator" ] = *src.allocator;
  }
  if( src.descriptor_pool ) {
    dest[ "descriptor_pool" ] = *src.descriptor_pool;
  }
  if( src.pipeline_cache ) {
    dest[ "pipeline_cache" ] = *src.pipeline_cache;
  }
}

}
