#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/buffer_pool_create_info.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const buffer_pool_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator" ] = *src.allocator;
  dest[ "descriptor_pool" ] = *src.descriptor_pool;
  dest[ "pipeline_cache" ] = *src.pipeline_cache;
  dest[ "write_shader" ] = src.write_shader.string();
  dest[ "read_shader" ] = src.read_shader.string();
  dest[ "max_buffer_count" ] = src.max_buffer_count;
  dest[ "staging_buffer_name" ]  = src.staging_buffer_name;
  dest[ "buffer_name" ] = src.buffer_name;
  dest[ "write_request_buffer_name" ] = src.write_request_buffer_name;
  dest[ "read_request_buffer_name" ] = src.read_request_buffer_name;
  dest[ "layout" ] = src.layout;
  dest[ "resources" ] = src.resources;
}

}
