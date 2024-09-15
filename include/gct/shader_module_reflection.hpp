#ifndef GCT_SHADER_MODULE_REFLECTION_HPP
#define GCT_SHADER_MODULE_REFLECTION_HPP
#include <unordered_set>
#include <string>
#include <optional>
#include <memory>
#include <vector>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>
#include <gct/spv_member_pointer.hpp>
#include <gct/spirv_reflect.h>

namespace gct {
  class shader_module_reflection_t {
  public:
    shader_module_reflection_t(
      const std::filesystem::path &
    );
    shader_module_reflection_t(
      const std::vector< std::uint8_t > &
    );
    ~shader_module_reflection_t();
    shader_module_reflection_t( const shader_module_reflection_t& ) = delete;
    shader_module_reflection_t( shader_module_reflection_t&& ) = default;
    shader_module_reflection_t &operator=( const shader_module_reflection_t& ) = delete;
    shader_module_reflection_t &operator=( shader_module_reflection_t&& ) = default;
    const SpvReflectShaderModule &operator*() const { return reflect; }
    const SpvReflectShaderModule *operator->() const { return &reflect; }
    spv_member_pointer get_member_pointer( const std::string &name, memory_layout layout ) const;
    spv_member_pointer get_push_constant_member_pointer( const std::string &name ) const;
    std::optional< spv_member_pointer > get_push_constant_member_pointer_maybe( const std::string &name ) const;
    std::vector< std::uint32_t > get_descriptor_set_id() const;
  private:
    SpvReflectShaderModule reflect;
    std::unordered_set< unsigned int > descriptor_set_ids;
  };
}
#endif

