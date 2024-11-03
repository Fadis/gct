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

namespace gct {

class allocator_t;
class descriptor_pool_t;
class pipeline_cache_t;
class descriptor_set_layout_t;
class descriptor_set_t;
class pipeline_layout_t;
struct compute_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
  LIBGCT_SETTER( shader )
  LIBGCT_SETTER( descriptor_set_layout )
  LIBGCT_SETTER( external_descriptor_set )
  LIBGCT_SETTER( external_pipeline_layout )
  LIBGCT_SETTER( resources )
  LIBGCT_SETTER( swapchain_image_count )
  LIBGCT_SETTER( ignore_unused_descriptor )
  LIBGCT_SETTER( specs )
  LIBGCT_SETTER( dim )
  compute_create_info &add_resource(
    const named_resource &n
  );
  compute_create_info &clear_resource(
    const named_resource &n
  );
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
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::filesystem::path shader;
  std::vector< std::shared_ptr< descriptor_set_layout_t > > descriptor_set_layout;
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

