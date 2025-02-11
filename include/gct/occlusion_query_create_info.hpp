#ifndef GCT_OCCLUSION_QUERY_CREATE_INFO_HPP
#define GCT_OCCLUSION_QUERY_CREATE_INFO_HPP

#include <memory>
#include <vector>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/allocator_set.hpp>

namespace gct {

class shader_module_t;
struct occlusion_query_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( color_attachment_count )
  LIBGCT_SETTER( depth_image )
  LIBGCT_SETTER( shaders )
  LIBGCT_SETTER( dynamic_cull_mode )
  LIBGCT_SETTER( max_query_count )
  LIBGCT_SETTER( query )
  LIBGCT_SETTER( resources )
  LIBGCT_NAMED_RESOURCE_SETTER( resources )
  occlusion_query_create_info &set_shader(
    const std::filesystem::path &
  );
  occlusion_query_create_info &add_shader(
    const std::filesystem::path&
  );
  occlusion_query_create_info &add_shader(
    const std::vector< std::filesystem::path >&
  );
  occlusion_query_create_info &clear_shader();
  allocator_set_t allocator_set;
  std::uint32_t color_attachment_count = 0u;
  std::shared_ptr< image_view_t > depth_image;
  std::vector< std::filesystem::path > shaders;
  bool dynamic_cull_mode = false;
  std::uint32_t max_query_count = 128u;
  bool query = true;
  std::vector< named_resource > resources;
};

void to_json( nlohmann::json&, const occlusion_query_create_info& );

}

#endif


