#ifndef GCT_DRAW_LINE_CREATE_INFO_HPP
#define GCT_DRAW_LINE_CREATE_INFO_HPP

#include <vector>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/allocator_set.hpp>

namespace gct {

class shader_module_t;
struct draw_line_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( color_attachment_count )
  LIBGCT_SETTER( shaders )
  LIBGCT_SETTER( dynamic_cull_mode )
  LIBGCT_SETTER( max_line_count )
  LIBGCT_SETTER( line_width )
  LIBGCT_SETTER( width )
  LIBGCT_SETTER( height )
  LIBGCT_SETTER( resources )
  LIBGCT_NAMED_RESOURCE_SETTER( resources )
  draw_line_create_info &set_shader(
    const std::filesystem::path &
  );
  draw_line_create_info &add_shader(
    const std::filesystem::path&
  );
  draw_line_create_info &add_shader(
    const std::vector< std::filesystem::path >&
  );
  draw_line_create_info &clear_shader();
  allocator_set_t allocator_set;
  std::uint32_t color_attachment_count = 0u;
  std::vector< std::filesystem::path > shaders;
  bool dynamic_cull_mode = false;
  std::uint32_t max_line_count = 128u;
  float line_width = 1.f;
  std::uint32_t width = 1920u;
  std::uint32_t height = 1080u;
  std::vector< named_resource > resources;
};

void to_json( nlohmann::json &dest, const draw_line_create_info &src );

}

#endif



