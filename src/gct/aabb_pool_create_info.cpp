#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/aabb_pool_create_info.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const aabb_pool_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator" ] = *src.allocator;
  dest[ "descriptor_pool" ] = *src.descriptor_pool;
  dest[ "pipeline_cache" ] = *src.pipeline_cache;
  dest[ "write_shader" ] = src.write_shader.string();
  dest[ "read_shader" ] = src.read_shader.string();
  dest[ "update_shader" ] = src.update_shader.string();
  dest[ "max_aabb_count" ] = src.max_aabb_count;
  dest[ "staging_aabb_buffer_name" ]  = src.staging_aabb_buffer_name;
  dest[ "matrix_buffer_name" ] = src.matrix_buffer_name;
  dest[ "aabb_buffer_name" ] = src.aabb_buffer_name;
  dest[ "write_request_buffer_name" ] = src.write_request_buffer_name;
  dest[ "read_request_buffer_name" ] = src.read_request_buffer_name;
  dest[ "update_request_buffer_name" ] = src.update_request_buffer_name;
  dest[ "resources" ] = src.resources;
}

}

