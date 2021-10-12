#ifndef LIBGCT_INCLUDE_GCT_DESCRIPTOR_SET_HPP
#define LIBGCT_INCLUDE_GCT_DESCRIPTOR_SET_HPP

#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/descriptor_set_allocate_info.hpp>
#include <gct/write_descriptor_set.hpp>

namespace gct {
  class descriptor_pool_t;
  class descriptor_set_t : public created_from< descriptor_pool_t > {
  public:
    descriptor_set_t(
      const std::shared_ptr< descriptor_pool_t >&,
      const descriptor_set_allocate_info_t&
    );
    vk::DescriptorSet &operator*() {
      return *handle;
    }
    const vk::DescriptorSet &operator*() const {
      return *handle;
    }
    vk::DescriptorSet* operator->() {
      return &handle.get();
    }
    const vk::DescriptorSet* operator->() const {
      return &handle.get();
    }
    const descriptor_set_allocate_info_t &get_props() const {
      return props;
    }
    void update(
      const std::vector< write_descriptor_set_t >&
    );
  private:
    descriptor_set_allocate_info_t props;
    vk::UniqueHandle< vk::DescriptorSet, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
  };

}

#endif
