#ifndef GCT_HIERARCHICAL_RASTER_OCCLUSION_CULLING_CREATE_INFO_HPP
#define GCT_HIERARCHICAL_RASTER_OCCLUSION_CULLING_CREATE_INFO_HPP

#include <memory>
#include <vector>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/onesweep_create_info.hpp>
#include <gct/allocator_set.hpp>

namespace gct {

class shader_module_t;
struct hierarchical_raster_occlusion_culling_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( setup_sort_shader )
  LIBGCT_SETTER( sort )
  LIBGCT_SETTER( setup_leaf_shader )
  LIBGCT_SETTER( setup_node_shader )
  LIBGCT_SETTER( mark_leaf_shader )
  LIBGCT_SETTER( mark_node_shader )
  LIBGCT_SETTER( shaders )
  LIBGCT_SETTER( width )
  LIBGCT_SETTER( height )
  LIBGCT_SETTER( color_attachment_count )
  LIBGCT_SETTER( dynamic_cull_mode )
  LIBGCT_SETTER( external_depth )
  LIBGCT_SETTER( resources )
  hierarchical_raster_occlusion_culling_create_info &add_resource(
    const named_resource &n
  );
  hierarchical_raster_occlusion_culling_create_info &clear_resource();
  hierarchical_raster_occlusion_culling_create_info &set_shader(
    const std::filesystem::path &
  );
  hierarchical_raster_occlusion_culling_create_info &add_shader(
    const std::filesystem::path&
  );
  hierarchical_raster_occlusion_culling_create_info &add_shader(
    const std::vector< std::filesystem::path >&
  );
  hierarchical_raster_occlusion_culling_create_info &clear_shader();
  allocator_set_t allocator_set;
  std::filesystem::path setup_sort_shader;
  onesweep_create_info sort;
  std::filesystem::path setup_leaf_shader;
  std::filesystem::path setup_node_shader;
  std::filesystem::path mark_leaf_shader;
  std::filesystem::path mark_node_shader;
  std::vector< std::filesystem::path > shaders;
  std::uint32_t width = 1920u;
  std::uint32_t height = 1080u;
  std::uint32_t color_attachment_count = 0u;
  bool dynamic_cull_mode = false;
  std::vector< std::shared_ptr< image_view_t > > external_depth;
  std::vector< named_resource > resources;
};

}

#endif



