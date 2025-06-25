#include <nlohmann/json.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/buffer_pool_create_info.hpp>

namespace gct {

buffer_pool_create_info &buffer_pool_create_info::set_shader(
  const std::filesystem::path &dir
) {
  set_write_shader( dir / "write.comp.spv" );
  set_read_shader( dir / "read.comp.spv" );
  return *this;
}

bool buffer_pool_create_info::shader_exists() const {
  return
    std::filesystem::exists( read_shader ) &&
    std::filesystem::exists( write_shader );
}

void to_json( nlohmann::json &dest, const buffer_pool_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
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

