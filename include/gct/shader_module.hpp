#ifndef GCT_SHADER_MODULE_HPP
#define GCT_SHADER_MODULE_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/created_from.hpp>
#include <gct/shader_module_create_info.hpp>
#include <gct/vulkan_handle.hpp>
#include <gct/property.hpp>

namespace gct {
  struct allocator_t;
  class device_t;
  class shader_module_t :
    public vulkan_handle< vk::ShaderModule >,
    public property< shader_module_create_info_t >,
    public created_from< device_t >,
    public std::enable_shared_from_this< shader_module_t > {
  public:
    shader_module_t(
      const std::shared_ptr< device_t >&,
      const shader_module_create_info_t&
    );
    shader_module_t( const shader_module_t& ) = delete;
    shader_module_t( shader_module_t&& ) = default;
    shader_module_t &operator=( const shader_module_t& ) = delete;
    shader_module_t &operator=( shader_module_t&& ) = default;
  };
  void to_json( nlohmann::json &root, const shader_module_t &v );
}

#endif

