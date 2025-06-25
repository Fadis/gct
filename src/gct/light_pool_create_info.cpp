#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/buffer.hpp>
#include <gct/light_pool_create_info.hpp>

namespace gct {

light_pool_create_info &light_pool_create_info::set_shader(
  const std::filesystem::path &dir
) {
  set_write_shader( dir / "write.comp.spv" );
  set_read_shader( dir / "read.comp.spv" );
  set_update_shader( dir / "update.comp.spv" );
  return *this;
}

bool light_pool_create_info::shader_exists() const {
  return
    std::filesystem::exists( read_shader ) &&
    std::filesystem::exists( write_shader ) &&
    std::filesystem::exists( update_shader );
}

void to_json( nlohmann::json &dest, const light_pool_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  if( src.matrix_pool ) {
    dest[ "matrix_pool" ] = *src.matrix_pool;
  }
  dest[ "write_shader" ] = src.write_shader.string();
  dest[ "read_shader" ] = src.read_shader.string();
  dest[ "update_shader" ] = src.update_shader.string();
  dest[ "max_light_count" ] = src.max_light_count;
  dest[ "staging_light_buffer_name" ]  = src.staging_light_buffer_name;
  dest[ "matrix_buffer_name" ] = src.matrix_buffer_name;
  dest[ "light_buffer_name" ] = src.light_buffer_name;
  dest[ "active_light_buffer_name" ] = src.active_light_buffer_name;
  dest[ "write_request_buffer_name" ] = src.write_request_buffer_name;
  dest[ "read_request_buffer_name" ] = src.read_request_buffer_name;
  dest[ "update_request_buffer_name" ] = src.update_request_buffer_name;
  dest[ "resources" ] = src.resources;
}

}


