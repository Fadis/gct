#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/image_view.hpp>
#include <gct/named_resource.hpp>
#include <gct/epipolar_light_scattering_create_info.hpp>

namespace gct {

epipolar_light_scattering_create_info &epipolar_light_scattering_create_info::add_resource(
  const named_resource &n
) {
  resources.push_back( n );
  return *this;
}
epipolar_light_scattering_create_info &epipolar_light_scattering_create_info::clear_resource() {
  resources.clear();
  return *this;
}
epipolar_light_scattering_create_info &epipolar_light_scattering_create_info::add_draw_mesh_shader(
  const std::filesystem::path &p
) {
  draw_mesh_shaders.push_back( p );
  return *this;
}
epipolar_light_scattering_create_info &epipolar_light_scattering_create_info::clear_draw_mesh_shader() {
  draw_mesh_shaders.clear();
  return *this;
}

void to_json( nlohmann::json &dest, const epipolar_light_scattering_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "pole_count" ] = src.pole_count;
  dest[ "vertex_count" ] = src.vertex_count;
  dest[ "input" ] = nlohmann::json::array();
  for( const auto &i: src.input ) {
    if( i ) {
      dest[ "input" ].push_back( *i );
    }
    else {
      dest[ "input" ].push_back( nullptr );
    }
  }
  dest[ "shadow" ] = nlohmann::json::array();
  for( const auto &i: src.shadow ) {
    if( i ) {
      dest[ "shadow" ].push_back( *i );
    }
    else {
      dest[ "shadow" ].push_back( nullptr );
    }
  }
  dest[ "texel_per_sample" ] = src.texel_per_sample;
  dest[ "draw_mesh_shaders" ] = nlohmann::json::array();
  for( const auto &p: src.draw_mesh_shaders ) {
    dest[ "draw_mesh_shaders" ].push_back( p.string() );
  }
  dest[ "generate_mesh2_shader" ] = src.generate_mesh2_shader.string();
  dest[ "generate_mesh3_shader" ] = src.generate_mesh3_shader.string();
  dest[ "generate_mesh4_shader" ] = src.generate_mesh4_shader.string();
  dest[ "volumetric_light_shader" ] = src.volumetric_light_shader.string();
  dest[ "clear_texcoord_shader" ] = src.clear_texcoord_shader.string();
  dest[ "generate_texcoord_shader" ] = src.generate_texcoord_shader.string();
  dest[ "fill_texcoord_shader" ] = src.fill_texcoord_shader.string();
  dest[ "hgauss_shader" ] = src.hgauss_shader.string();
  dest[ "vgauss_shader" ] = src.vgauss_shader.string();
  dest[ "resources" ] = src.resources;
}

}

