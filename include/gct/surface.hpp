#ifndef LIBGCT_INCLUDE_GCT_SURFACE_HPP
#define LIBGCT_INCLUDE_GCT_SURFACE_HPP

#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/surface_capabilities.hpp>

namespace gct {

#ifdef VK_KHR_SURFACE_EXTENSION_NAME
  class instance_t;
  struct surface_t;
  class physical_device_t;
  void to_json( nlohmann::json&, const surface_t &v );
  struct surface_t : public created_from< instance_t > {
  public:
    surface_t(
      const physical_device_t&,
      vk::SurfaceKHR
    );
    vk::SurfaceKHR &operator*() {
      return *handle;
    }
    const vk::SurfaceKHR &operator*() const {
      return *handle;
    }
    vk::SurfaceKHR* operator->() {
      return &handle.get();
    }
    const vk::SurfaceKHR* operator->() const {
      return &handle.get();
    }
    const surface_capabilities_t &get_caps() const {
      return caps;
    }
  private:
    vk::UniqueHandle< vk::SurfaceKHR, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
    surface_capabilities_t caps;
  };
#endif

}

#endif
