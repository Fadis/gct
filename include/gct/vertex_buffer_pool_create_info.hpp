#ifndef GCT_VERTEX_BUFFER_POOL_CREATE_INFO_HPP
#define GCT_VERTEX_BUFFER_POOL_CREATE_INFO_HPP
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <nlohmann/json_fwd.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/color_space.hpp>
#include <gct/allocator_set.hpp>

namespace gct {

class allocator_t;
class descriptor_set_t;
struct vertex_buffer_pool_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_SETTER( max_vertex_buffer_count )
  LIBGCT_SETTER( descriptor_set_layout )
  LIBGCT_SETTER( external_descriptor_set )
  LIBGCT_SETTER( external_pipeline_layout )
  LIBGCT_SETTER( vertex_buffer_descriptor_set_id )
  LIBGCT_SETTER( descriptor_name )
  LIBGCT_SETTER( resources )
  vertex_buffer_pool_create_info &set_external_descriptor_set(
    const std::shared_ptr< descriptor_set_t > &v
  ) {
    external_descriptor_set.insert( std::make_pair( 0u, v ) );
    return *this;
  }
  vertex_buffer_pool_create_info &add_external_descriptor_set(
    unsigned int id,
    const std::shared_ptr< descriptor_set_t > &v
  ) {
    external_descriptor_set.insert( std::make_pair( id, v ) );
    return *this;
  }
  allocator_set_t allocator_set;
  std::uint32_t max_vertex_buffer_count = 65536u;
  std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_layout_t > > descriptor_set_layout;
  std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_t > > external_descriptor_set;
  std::shared_ptr< pipeline_layout_t > external_pipeline_layout;
  std::uint32_t vertex_buffer_descriptor_set_id = 4u;
  std::string descriptor_name = "vertex_buffer_i32";
  std::vector< named_resource > resources;
};
void to_json( nlohmann::json &dest, const vertex_buffer_pool_create_info &src );

}

#endif

