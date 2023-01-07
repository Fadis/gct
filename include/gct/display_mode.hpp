#ifndef LIBGCT_INCLUDE_GCT_DISPLAY_MODE_HPP
#define LIBGCT_INCLUDE_GCT_DISPLAY_MODE_HPP

#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/display_mode_create_info.hpp>

namespace gct {

/*
 * This doesn't work due to lack of UniqueHandleTraits<DisplayModeKHR>
 */

#if 0
#ifdef VK_KHR_DISPLAY_EXTENSION_NAME
  class instance_t;
  struct surface_t;
  class physical_device_t;
  struct display_mode_t : public created_from< instance_t > {
  public:
    display_mode_t(
      const std::shared_ptr< instance_t > &,
      const physical_device_t&,
      vk::DisplayKHR,
      const display_mode_create_info_t &
    );
    vk::DisplayModeKHR &operator*() {
      return *handle;
    }
    const vk::DisplayModeKHR &operator*() const {
      return *handle;
    }
    vk::DisplayModeKHR* operator->() {
      return &handle.get();
    }
    const vk::DisplayModeKHR* operator->() const {
      return &handle.get();
    }
    const display_mode_create_info_t &get_props() const { return props; }
  private:
    vk::UniqueHandle< vk::DisplayModeKHR, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
    display_mode_create_info_t props;
  };
  void to_json( nlohmann::json&, const display_mode_t &v );
#endif
#endif
}

#endif

