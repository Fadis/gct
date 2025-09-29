#ifndef GCT_GRAPHICS_CREATE_INFO_HPP
#define GCT_GRAPHICS_CREATE_INFO_HPP

#include <memory>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/graphics_pipeline_create_info.hpp>
#include <gct/allocator_set.hpp>
#include <gct/common_color_blend_mode.hpp>

namespace gct {

class render_pass_t;
class shader_module_t;
class descriptor_set_t;
class pipeline_layout_t;

namespace scene_graph {
  class scene_graph_resource;
}

struct graphics_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( shaders )
  LIBGCT_SETTER( descriptor_set_layout )
  LIBGCT_SETTER( shader_module )
  LIBGCT_SETTER( pipeline_create_info )
  LIBGCT_SETTER( external_descriptor_set )
  LIBGCT_SETTER( external_pipeline_layout )
  LIBGCT_SETTER( resources )
  LIBGCT_NAMED_RESOURCE_SETTER( resources )
  LIBGCT_SETTER( swapchain_image_count )
  LIBGCT_SETTER( ignore_unused_descriptor )
  graphics_create_info &add_shader(
    const std::filesystem::path&
  );
  graphics_create_info &add_shader(
    const std::vector< std::filesystem::path >&
  );
  graphics_create_info &add_shader(
    const std::shared_ptr< shader_module_t >&
  );
  graphics_create_info &set_external_descriptor_set(
    const std::shared_ptr< descriptor_set_t > &v
  ) {
    external_descriptor_set.insert( std::make_pair( 0u, v ) );
    return *this;
  }
  graphics_create_info &add_external_descriptor_set(
    unsigned int id,
    const std::shared_ptr< descriptor_set_t > &v
  ) {
    external_descriptor_set.insert( std::make_pair( id, v ) );
    return *this;
  }
  [[deprecated]] graphics_create_info &set_scene_graph(
     const scene_graph::scene_graph_resource &r
  );
  graphics_create_info &set_scene_graph(
     const std::shared_ptr< scene_graph::scene_graph_resource > &r
  );
  graphics_create_info &clear_shader();
  graphics_create_info &use_dynamic_rendering(
    const std::vector< vk::Format > &color_attachment_format,
    vk::Format depth_attachment_format,
    vk::Format stencil_attachment_format
  );
  graphics_create_info &use_dynamic_rendering(
    vk::Format color_attachment_format,
    vk::Format depth_attachment_format,
    vk::Format stencil_attachment_format
  );
  graphics_create_info &use_color_blend( common_color_blend_mode );
  graphics_create_info &disable_depth_test();
  graphics_create_info &disable_depth_write();
  allocator_set_t allocator_set;
  std::vector< std::filesystem::path > shaders;
  std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_layout_t > > descriptor_set_layout;
  std::vector< std::shared_ptr< shader_module_t > > shader_module;
  graphics_pipeline_create_info_t pipeline_create_info;
  std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_t > > external_descriptor_set;
  std::shared_ptr< pipeline_layout_t > external_pipeline_layout;
  std::vector< named_resource > resources;
  unsigned int swapchain_image_count = 1u;
  bool ignore_unused_descriptor = false;
};

void to_json( nlohmann::json&, const graphics_create_info& );

}

#endif


