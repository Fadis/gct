#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/image_view.hpp>
#include <gct/named_resource.hpp>
#include <gct/image_filter_create_info.hpp>

namespace gct {

image_filter_create_info &image_filter_create_info::add_resource(
  const named_resource &n
) {
  resources.push_back( n );
  return *this;
}
image_filter_create_info &image_filter_create_info::clear_resource(
  const named_resource &n
) {
  resources.clear();
  return *this;
}

void to_json( nlohmann::json &dest, const image_filter_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "shader" ] = src.shader.string();
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
  if( src.output_create_info ) {
    dest[ "output_create_info" ] = *src.output_create_info;
  }
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
  dest[ "resources" ] = src.resources;
  dest[ "reusable" ] = src.reusable;
}

}

