#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/texture_pool_create_info.hpp>
#include <gct/sampler_pool.hpp>
#include <gct/image_pool.hpp>

namespace gct {

texture_pool_create_info &texture_pool_create_info::set_shader(
  const std::filesystem::path &dir
) {
  set_dummy_shader( dir / "dummy.comp.spv" );
  return *this;
}

bool texture_pool_create_info::shader_exists() const {
  return
    std::filesystem::exists( dummy_shader );
}

void to_json( nlohmann::json &dest, const texture_pool_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "dummy_shader" ] = src.dummy_shader.string();
  dest[ "max_texture_count" ] = src.max_texture_count;
  dest[ "max_request_count" ] = src.max_request_count;
  if( src.descriptor_set ) {
    dest[ "descriptor_set" ] = *src.descriptor_set;
  }
  if( src.sampler ) {
    dest[ "sampler" ] = *src.sampler;
  }
  if( src.image ) {
    dest[ "image" ] = *src.image;
  }
  dest[ "csmat" ] = src.csmat;
  dest[ "metadata_buffer_name" ] = src.metadata_buffer_name;
  dest[ "metadata_layout" ] = src.metadata_layout;
  dest[ "resources" ] = src.resources;
}

}


