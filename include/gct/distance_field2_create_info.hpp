#ifndef GCT_DISTANCE_FIELD2_CREATE_INFO_HPP
#define GCT_DISTANCE_FIELD2_CREATE_INFO_HPP

#include <vector>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/allocator_set.hpp>

namespace gct {

class image_view_t;
struct distance_field2_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
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
  allocator_set_t allocator_set;
  std::filesystem::path voronoi_shader;
  std::filesystem::path distance_field_shader;
  std::string working_image_name = "voxel";
  std::string distance_field_name = "distance_field";
  unsigned int size_factor = 9u;
  std::vector< named_resource > resources;
};

void to_json( nlohmann::json &dest, const distance_field2_create_info &src );

}

#endif

