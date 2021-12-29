#ifndef LIBGCT_INCLUDE_GCT_DESCRIPTOR_SET_LAYOUT_HPP
#define LIBGCT_INCLUDE_GCT_DESCRIPTOR_SET_LAYOUT_HPP

#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/descriptor_set_layout_create_info.hpp>
#include <nlohmann/json_fwd.hpp>

namespace gct {
  class device_t;
  class descriptor_set_layout_t : public created_from< device_t >, std::enable_shared_from_this< descriptor_set_layout_t > {
  public:
    descriptor_set_layout_t(
      const std::shared_ptr< device_t >&,
      const descriptor_set_layout_create_info_t&
    );
    vk::DescriptorSetLayout &operator*() {
      return *handle;
    }
    const vk::DescriptorSetLayout &operator*() const {
      return *handle;
    }
    vk::DescriptorSetLayout* operator->() {
      return &handle.get();
    }
    const vk::DescriptorSetLayout* operator->() const {
      return &handle.get();
    }
    const descriptor_set_layout_create_info_t &get_props() const {
      return props;
    }
  private:
    descriptor_set_layout_create_info_t props;
    vk::UniqueHandle< vk::DescriptorSetLayout, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
  };
  void to_json( nlohmann::json &root, const descriptor_set_layout_t &v );

}

#endif
