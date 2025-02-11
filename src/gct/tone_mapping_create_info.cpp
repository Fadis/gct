#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/image_view.hpp>
#include <gct/named_resource.hpp>
#include <gct/tone_mapping_create_info.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const tone_mapping_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "shader" ] = src.shader;
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
  dest[ "output_name" ] = src.output_name;
  dest[ "resources" ] = src.resources;
}

}

