#ifndef GCT_COMPUTE_CREATE_INFO_HPP
#define GCT_COMPUTE_CREATE_INFO_HPP

#include <memory>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <glm/vec3.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/specialization_map.hpp>
#include <gct/allocator_set.hpp>

namespace gct {

class descriptor_set_layout_t;
class descriptor_set_t;
class pipeline_layout_t;

namespace scene_graph {
  class scene_graph_resource;
}

struct compute_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( shader )
  LIBGCT_SETTER( descriptor_set_layout )
  LIBGCT_SETTER( external_descriptor_set )
  LIBGCT_SETTER( external_pipeline_layout )
  LIBGCT_SETTER( resources )
  LIBGCT_NAMED_RESOURCE_SETTER( resources )
  LIBGCT_SETTER( swapchain_image_count )
  LIBGCT_SETTER( ignore_unused_descriptor )
  LIBGCT_SETTER( specs )
  LIBGCT_SETTER( dim )
  compute_create_info &set_external_descriptor_set(
    const std::shared_ptr< descriptor_set_t > &v
  ) {
    external_descriptor_set.insert( std::make_pair( 0u, v ) );
    return *this;
  }
  compute_create_info &add_external_descriptor_set(
    unsigned int id,
    const std::shared_ptr< descriptor_set_t > &v
  ) {
    external_descriptor_set.insert( std::make_pair( id, v ) );
    return *this;
  }
  compute_create_info &add_spec(
    std::uint32_t id,
    const specialization_value_type &v
  ) {
    specs.insert( std::make_pair( id, v ) );
    return *this;
  }
  compute_create_info &set_dim(
    std::int32_t x,
    std::int32_t y,
    std::int32_t z
  ) {
    dim = glm::ivec3( x, y, z );
    return *this;
  }
  compute_create_info &set_scene_graph(
     const std::shared_ptr< scene_graph::scene_graph_resource >&
  );
  allocator_set_t allocator_set;
  std::filesystem::path shader;
  std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_layout_t > > descriptor_set_layout;
  std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_t > > external_descriptor_set;
  std::shared_ptr< pipeline_layout_t > external_pipeline_layout;
  std::vector< named_resource > resources;
  unsigned int swapchain_image_count = 1u;
  bool ignore_unused_descriptor = false;
  specialization_map specs;
  glm::ivec3 dim = glm::ivec3( 0, 0, 0 );
};

void to_json( nlohmann::json&, const compute_create_info& );

}

#endif

