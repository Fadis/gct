#ifndef LIBGCT_INCLUDE_GCT_SURFACE_HPP
#define LIBGCT_INCLUDE_GCT_SURFACE_HPP

#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/surface_capabilities.hpp>
#include <gct/vulkan_handle.hpp>

namespace gct {

#if defined(VK_KHR_SURFACE_EXTENSION_NAME) && defined(VK_KHR_DISPLAY_EXTENSION_NAME)
  class display_surface_create_info_t;
#endif
#ifdef VK_KHR_SURFACE_EXTENSION_NAME
  class instance_t;
  struct surface_t;
  class physical_device_t;
  void to_json( nlohmann::json&, const surface_t &v );
  struct surface_t :
    public vulkan_handle< vk::SurfaceKHR >,
    public created_from< instance_t > {
  public:
    surface_t(
      const physical_device_t&,
      vk::SurfaceKHR
    );
#if defined(VK_KHR_SURFACE_EXTENSION_NAME) && defined(VK_KHR_DISPLAY_EXTENSION_NAME)
    surface_t(
      const physical_device_t&,
      vk::UniqueHandle< vk::SurfaceKHR, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > &&
    );
#endif
    [[nodiscard]] const surface_capabilities_t &get_caps() const {
      return caps;
    }
  private:
    surface_capabilities_t caps;
  };
#endif

}

#endif
