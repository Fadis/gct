#ifndef LIBGCT_INCLUDE_GCT_DESCRIPTOR_POOL_HPP
#define LIBGCT_INCLUDE_GCT_DESCRIPTOR_POOL_HPP

#include <memory>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/created_from.hpp>
#include <gct/descriptor_pool_create_info.hpp>
#include <gct/descriptor_set.hpp>

namespace gct {
  class descriptor_set_allocate_info_t;
  class descriptor_set_layout_t;
  class descriptor_pool_t : public created_from< device_t >, public std::enable_shared_from_this< descriptor_pool_t > {
  public:
    descriptor_pool_t(
      const std::shared_ptr< device_t >&,
      const descriptor_pool_create_info_t&
    );
    vk::DescriptorPool &operator*() {
      return *handle;
    }
    const vk::DescriptorPool &operator*() const {
      return *handle;
    }
    vk::DescriptorPool* operator->() {
      return &handle.get();
    }
    const vk::DescriptorPool* operator->() const {
      return &handle.get();
    }
    const descriptor_pool_create_info_t &get_props() const {
      return props;
    }
    std::shared_ptr< descriptor_set_t > allocate(
      const descriptor_set_allocate_info_t&
    );
    std::shared_ptr< descriptor_set_t > allocate(
      const std::shared_ptr< descriptor_set_layout_t >&
    );
    std::shared_ptr< descriptor_set_t > allocate(
      const std::shared_ptr< descriptor_set_layout_t >&,
      std::uint32_t
    );
  private:
    descriptor_pool_create_info_t props;
    vk::UniqueHandle< vk::DescriptorPool, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
  };
  void to_json( nlohmann::json &dest, const descriptor_pool_t &src );
}

#endif
