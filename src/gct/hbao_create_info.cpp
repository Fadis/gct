#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/image_view.hpp>
#include <gct/named_resource.hpp>
#include <gct/hbao_create_info.hpp>

namespace gct {

hbao_create_info &hbao_create_info::set_shader(
  const std::filesystem::path &dir
) {
  set_ao0_shader( dir / "hbao_0.comp.spv" );
  set_ao45_shader( dir / "hbao_45.comp.spv" );
  set_ao90_shader( dir / "hbao_90.comp.spv" );
  set_ao135_shader( dir / "hbao_135.comp.spv" );
  set_ao135_shader( dir / "hbao_135.comp.spv" );
  set_sum_shader( dir / "hbao.comp.spv" );
  return *this;
}

void to_json( nlohmann::json &dest, const hbao_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "ao0_shader" ] = src.ao0_shader.string();
  dest[ "ao45_shader" ] = src.ao45_shader.string();
  dest[ "ao90_shader" ] = src.ao90_shader.string();
  dest[ "ao135_shader" ] = src.ao135_shader.string();
  dest[ "sum_shader" ] = src.sum_shader.string();
  dest[ "input" ] = nlohmann::json::array();
  for( const auto &p: src.input ) {
    if( p ) {
      dest[ "input" ].push_back( *p );
    }
    else {
      dest[ "input" ].push_back( nullptr );
    }
  }
  dest[ "input_name" ] = src.input_name;
  dest[ "output" ] = nlohmann::json::array();
  for( const auto &p: src.output ) {
    if( p ) {
      dest[ "output" ].push_back( *p );
    }
    else {
      dest[ "output" ].push_back( nullptr );
    }
  }
  dest[ "output_name" ] = src.output_name;
  dest[ "temp" ] = nlohmann::json::array();
  for( const auto &p: src.temp ) {
    if( p ) {
      dest[ "temp" ].push_back( *p );
    }
    else {
      dest[ "temp" ].push_back( nullptr );
    }
  }
  dest[ "temp_name" ] = src.temp_name;
  dest[ "resources" ] = src.resources;
}

}

