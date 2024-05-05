#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/matrix_pool_create_info.hpp>

namespace gct {

matrix_pool_create_info &matrix_pool_create_info::add_resource(
  const named_resource &n
) {
  resources.push_back( n );
  return *this;
}
matrix_pool_create_info &matrix_pool_create_info::clear_resource() {
  resources.clear();
  return *this;
}
matrix_pool_create_info &matrix_pool_create_info::set_shader(
  const std::filesystem::path &dir
) {
  set_write_shader( dir / "write.comp.spv" );
  set_read_shader( dir / "read.comp.spv" );
  set_update_shader( dir / "update.comp.spv" );
  return *this;
}

void to_json( nlohmann::json &dest, const matrix_pool_create_info &src ) {
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
  dest[ "write_shader" ] = src.write_shader.string();
  dest[ "read_shader" ] = src.read_shader.string();
  dest[ "update_shader" ] = src.update_shader.string();
  dest[ "max_matrix_count" ] = src.max_matrix_count;
  dest[ "staging_matrix_buffer_name" ]  = src.staging_matrix_buffer_name;
  dest[ "matrix_buffer_name" ] = src.matrix_buffer_name;
  dest[ "write_request_buffer_name" ] = src.write_request_buffer_name;
  dest[ "read_request_buffer_name" ] = src.read_request_buffer_name;
  dest[ "update_request_buffer_name" ] = src.update_request_buffer_name;
  dest[ "resources" ] = src.resources;
}

}

