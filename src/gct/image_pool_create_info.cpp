#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/matrix_pool.hpp>
#include <gct/buffer.hpp>
#include <gct/color_space.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/image_pool_create_info.hpp>

namespace gct {
image_pool_create_info &image_pool_create_info::set_shader(
  const std::filesystem::path &dir
) {
  set_rgba8_shader( dir / "rgba8.comp.spv" );
  set_rgba16_shader( dir / "rgba16.comp.spv" );
  set_rgba16f_shader( dir / "rgba16f.comp.spv" );
  set_rgba32f_shader( dir / "rgba32f.comp.spv" );
  return *this;
}

void to_json( nlohmann::json &dest, const image_pool_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "max_image_count" ] = src.max_image_count;
  dest[ "descriptor_set_layout" ] = nlohmann::json::object();
  for( const auto &v: src.descriptor_set_layout ) {
    if( v.second ) {
      dest[ "descriptor_set_layout" ][ std::to_string( v.first ) ] = *v.second;
    }
    else {
      dest[ "descriptor_set_layout" ][ std::to_string( v.first ) ] = nullptr;
    }
  }
  dest[ "descriptor_set" ] = nlohmann::json::object();
  for( const auto &v: src.external_descriptor_set ) {
    if( v.second ) {
      dest[ "descriptor_set" ][ std::to_string( v.first ) ] = *v.second;
    }
    else {
      dest[ "descriptor_set" ][ std::to_string( v.first ) ] = nullptr;
    }
  }
  dest[ "image_descriptor_set_id" ] = src.image_descriptor_set_id;
  dest[ "descriptor_name" ] = src.descriptor_name;
  dest[ "layout" ] = src.layout;
  if( src.matrix_pool ) {
    dest[ "matrix_pool" ] = *src.matrix_pool;
  }
  dest[ "matrix_buffer_name" ] = src.matrix_buffer_name;
  dest[ "enable_linear" ] = src.enable_linear;
  dest[ "csmat" ] = src.csmat;
  dest[ "rgba8_shader" ] = src.rgba8_shader;
  dest[ "rgba16_shader" ] = src.rgba16_shader;
  dest[ "rgba16f_shader" ] = src.rgba16f_shader;
  dest[ "rgba32f_shader" ] = src.rgba32f_shader;
  dest[ "resources" ] = src.resources;
}

}

