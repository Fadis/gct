#ifndef GCT_SHADER_HPP
#define GCT_SHADER_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/created_from.hpp>
#include <gct/shader_create_info.hpp>
#include <gct/vulkan_handle.hpp>
#include <gct/property.hpp>

#if defined(VK_VERSION_1_3) || defined(VK_EXT_SHADER_OBJECT_EXTENSION_NAME)
namespace gct {
  struct allocator_t;
  class device_t;
  class shader_t :
    public vulkan_handle< vk::ShaderEXT >,
    public property< shader_create_info_t >,
    public created_from< device_t >,
    public std::enable_shared_from_this< shader_t > {
  public:
    shader_t(
      const std::shared_ptr< device_t >&,
      const shader_create_info_t&
    );
    shader_t( const shader_t& ) = delete;
    shader_t( shader_t&& ) = default;
    shader_t &operator=( const shader_t& ) = delete;
    shader_t &operator=( shader_t&& ) = default;
    [[nodiscard]] vk::ShaderStageFlagBits get_stage() const;
    [[nodiscard]] bool is_compute() const;
    [[nodiscard]] bool is_graphics() const;
    [[nodiscard]] bool is_ray_trace() const;
  };
  void to_json( nlohmann::json &root, const shader_t &v );
}

#endif

#endif

