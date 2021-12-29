#ifndef GCT_SHADER_MODULE_HPP
#define GCT_SHADER_MODULE_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/created_from.hpp>
#include <gct/shader_module_create_info.hpp>

namespace gct {
  struct allocator_t;
  class device_t;
  class shader_module_t : public created_from< device_t >, public std::enable_shared_from_this< shader_module_t > {
  public:
    shader_module_t(
      const std::shared_ptr< device_t >&,
      const shader_module_create_info_t&
    );
    shader_module_t( const shader_module_t& ) = delete;
    shader_module_t( shader_module_t&& ) = default;
    shader_module_t &operator=( const shader_module_t& ) = delete;
    shader_module_t &operator=( shader_module_t&& ) = default;
    const shader_module_create_info_t &get_props() const { return props; }
    vk::ShaderModule &operator*() {
      return *handle;
    }
    const vk::ShaderModule &operator*() const {
      return *handle;
    }
    vk::ShaderModule *operator->() {
      return &handle.get();
    }
    const vk::ShaderModule *operator->() const {
      return &handle.get();
    }
  private:
    shader_module_create_info_t props;
    vk::UniqueHandle< vk::ShaderModule, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
  };
  void to_json( nlohmann::json &root, const shader_module_t &v );
}

#endif

