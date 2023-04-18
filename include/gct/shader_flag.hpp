#ifndef LIBGCT_INCLUDE_GCT_SHADER_FLAG_HPP
#define LIBGCT_INCLUDE_GCT_SHADER_FLAG_HPP
#include <optional>
#include <memory>
#include <unordered_map>
#include <filesystem>

namespace gct {
  class shader_module_t;
  enum class shader_flag_t {
    base_color = ( 1 << 0 ),
    metallic_roughness = ( 1 << 1 ),
    normal = ( 1 << 2 ),
    occlusion = ( 1 << 3 ),
    emissive = ( 1 << 4 ),
    tangent = ( 1 << 5 ),
    skin = ( 1 << 6 ),
    shadow = ( 1 << 7 ),
    vertex = ( 1 << 8 ),
    tesselation_control = ( 1 << 9 ),
    tesselation_evaluation = ( 1 << 10 ),
    geometry = ( 1 << 11 ),
    fragment = ( 1 << 12 ),
    compute = ( 1 << 13 ),
    special = ( 1 << 14 )
  };
  using shaders_t = std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > >;
  std::optional< shader_flag_t > get_shader_flag( const std::filesystem::path &path );
}

#endif

