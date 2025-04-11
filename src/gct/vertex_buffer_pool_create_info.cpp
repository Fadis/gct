#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/matrix_pool.hpp>
#include <gct/buffer.hpp>
#include <gct/color_space.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/vertex_buffer_pool_create_info.hpp>

namespace gct {
  std::shared_ptr< allocator_t > allocator;
  std::uint32_t max_vertex_buffer_count = 65536u;
  std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_layout_t > > descriptor_set_layout;
  std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_t > > external_descriptor_set;
  std::shared_ptr< pipeline_layout_t > external_pipeline_layout;
  std::uint32_t vertex_buffer_descriptor_set_id = 4u;
  std::string descriptor_name = "vertices";

void to_json( nlohmann::json &dest, const vertex_buffer_pool_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "max_vertex_buffer_count" ] = src.max_vertex_buffer_count;
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
  dest[ "vertex_buffer_descriptor_set_id" ] = src.vertex_buffer_descriptor_set_id;
  dest[ "descriptor_name" ] = src.descriptor_name;
  dest[ "resources" ] = src.resources;
}

}


