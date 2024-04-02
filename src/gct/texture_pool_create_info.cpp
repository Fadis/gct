#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/texture_pool_create_info.hpp>
#include <gct/sampler_pool.hpp>
#include <gct/image_pool.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const texture_pool_create_info &src ) {
  dest = nlohmann::json::object();
  if( src.allocator ) {
    dest[ "allocator" ] = *src.allocator;
  }
  dest[ "max_texture_count" ] = src.max_texture_count;
  if( src.descriptor_set ) {
    dest[ "descriptor_set" ] = *src.descriptor_set;
  }
  if( src.sampler ) {
    dest[ "sampler" ] = *src.sampler;
  }
  if( src.image ) {
    dest[ "image" ] = *src.image;
  }
}

}


