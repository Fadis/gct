#ifndef GCT_DISTANCE_FIELD2_CREATE_INFO_HPP
#define GCT_DISTANCE_FIELD2_CREATE_INFO_HPP

#include <memory>
#include <vector>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>

namespace gct {

class allocator_t;
class descriptor_pool_t;
class pipeline_cache_t;
class image_view_t;
struct distance_field2_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
  LIBGCT_SETTER( voronoi_shader )
  LIBGCT_SETTER( distance_field_shader )
  LIBGCT_SETTER( size_factor )
  LIBGCT_SETTER( working_image_name )
  LIBGCT_SETTER( distance_field_name )
  LIBGCT_SETTER( resources )
  distance_field2_create_info &add_resource(
    const named_resource &n
  );
  distance_field2_create_info &clear_resource(
    const named_resource &n
  );
  distance_field2_create_info &set_shader(
    const std::filesystem::path &
  );
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::filesystem::path voronoi_shader;
  std::filesystem::path distance_field_shader;
  std::string working_image_name = "voxel";
  std::string distance_field_name = "distance_field";
  unsigned int size_factor = 9u;
  std::vector< named_resource > resources;
};

}

#endif

