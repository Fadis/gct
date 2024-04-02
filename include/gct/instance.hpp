#ifndef LIBGCT_INCLUDE_GCT_INSTANCE_HPP
#define LIBGCT_INCLUDE_GCT_INSTANCE_HPP

#include <memory>
#include <functional>
#include <nlohmann/json_fwd.hpp>
#include <gct/get_extensions.hpp>
#include <gct/physical_device.hpp>
#include <gct/instance_create_info.hpp>

namespace gct {
  class instance_t;
  bool is_valid_vulkan_version( std::uint32_t version );
  class display_surface_create_info_t;
#if defined(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)
  using debug_callback_t = std::function<
    void(
      vk::DebugUtilsMessageSeverityFlagBitsEXT,
      vk::DebugUtilsMessageTypeFlagsEXT,
      const vk::DebugUtilsMessengerCallbackDataEXT&
    )
  >;
#endif
  class instance_t : public std::enable_shared_from_this< instance_t > {
  public:
    instance_t(
      const instance_create_info_t&
    );
    device_groups_t get_physical_devices(
      const std::vector< const char* > &dlayers
    );
    std::uint32_t get_api_version() const;
    const layer_map_t &get_activated_layers() const { return activated_layers; }
    const extension_map_t &get_activated_extensions() const { return activated_extensions; }
    vk::Instance &operator*() {
      return *handle;
    }
    const vk::Instance &operator*() const {
      return *handle;
    }
    vk::Instance* operator->() {
      return &handle.get();
    }
    const vk::Instance* operator->() const {
      return &handle.get();
    }
    const instance_create_info_t &get_props() const { return props; }
#if defined(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)
    void set_debug_callback(
      vk::DebugUtilsMessageSeverityFlagsEXT severity,
      vk::DebugUtilsMessageTypeFlagsEXT type,
      debug_callback_t &&cb
    );
#endif
    void abort_on_validation_failure( bool debug = false );
  private:
    instance_create_info_t props;
    layer_map_t activated_layers;
    extension_map_t activated_extensions;
    vk::UniqueHandle< vk::Instance, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
#if defined(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)
    vk::UniqueHandle< vk::DebugUtilsMessengerEXT, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > debug_message;
    debug_callback_t debug_callback;
    static VkBool32 call_debug_callback(
      VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
      VkDebugUtilsMessageTypeFlagsEXT messageTypes,
      const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
      void *pUserData
    );
#endif
  };
}

#endif

