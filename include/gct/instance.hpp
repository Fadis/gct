#ifndef LIBGCT_INCLUDE_GCT_INSTANCE_HPP
#define LIBGCT_INCLUDE_GCT_INSTANCE_HPP

#include <memory>
#include <nlohmann/json.hpp>
#include <gct/get_extensions.hpp>
#include <gct/physical_device.hpp>

namespace gct {
  class instance_t;

  bool is_valid_vulkan_version( std::uint32_t version );

  class instance_t : public std::enable_shared_from_this< instance_t > {
  public:
    instance_t(
      const std::string &application_name,
      uint32_t application_version,
      uint32_t api_version,
      const std::vector< const char* > &iext,
      const std::vector< const char* > &ilayers
    );
    device_groups_t get_physical_devices(
      const std::vector< const char* > &dlayers
    );
    std::uint32_t get_api_version() const { return api_version; }
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
  private:
    std::uint32_t api_version;
    layer_map_t activated_layers;
    extension_map_t activated_extensions;
    vk::UniqueHandle< vk::Instance, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
  };
}

#endif

