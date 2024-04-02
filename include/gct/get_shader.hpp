#ifndef GCT_GET_SHADER_HPP
#define GCT_GET_SHADER_HPP

#include <memory>
#include <optional>
#include <vector>
#include <unordered_map>
#include <gct/shader_module.hpp>
#include <gct/shader_flag.hpp>

namespace gct {
  std::shared_ptr< shader_module_t > get_suboptimal_shader(
    const std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > &shader,
    shader_flag_t flag,
    int depth
  );
  std::shared_ptr< shader_module_t > get_shader(
    const std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > &shader,
    shader_flag_t flag
  );
  std::optional< std::filesystem::path > get_suboptimal_shader(
    const std::unordered_map< shader_flag_t, std::filesystem::path > &shader,
    shader_flag_t flag,
    int depth
  );
  std::optional< std::filesystem::path > get_shader(
    const std::unordered_map< shader_flag_t, std::filesystem::path > &shader,
    shader_flag_t flag
  );
  std::vector< std::shared_ptr< shader_module_t > > get_shader(
    const std::vector< std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > > &shader,
    shader_flag_t flag
  );
  std::vector< std::shared_ptr< shader_module_t > > get_shader_optional(
    const std::vector< std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > > &shader,
    shader_flag_t flag
  );
}

#endif

