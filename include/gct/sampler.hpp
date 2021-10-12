#ifndef GCT_SAMPLER_HPP
#define GCT_SAMPLER_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/sampler_create_info.hpp>

namespace gct {
  struct allocator_t;
  class device_t;
  class sampler_t : public created_from< device_t >, public std::enable_shared_from_this< sampler_t > {
  public:
    sampler_t(
      const std::shared_ptr< device_t >&,
      const sampler_create_info_t&
    );
    sampler_t( const sampler_t& ) = delete;
    sampler_t( sampler_t&& ) = default;
    sampler_t &operator=( const sampler_t& ) = delete;
    sampler_t &operator=( sampler_t&& ) = default;
    const sampler_create_info_t &get_props() const { return props; }
    vk::Sampler &operator*() {
      return *handle;
    }
    const vk::Sampler &operator*() const {
      return *handle;
    }
    vk::Sampler *operator->() {
      return &handle.get();
    }
    const vk::Sampler *operator->() const {
      return &handle.get();
    }
  private:
    sampler_create_info_t props;
    vk::UniqueHandle< vk::Sampler, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
  };
}

#endif

