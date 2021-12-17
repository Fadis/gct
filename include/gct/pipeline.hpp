#ifndef GCT_PIPELINE_HPP
#define GCT_PIPELINE_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>

namespace gct {
  class pipeline_cache_t;
  class pipeline_t : public created_from< pipeline_cache_t > {
  public:
    pipeline_t(
      const std::shared_ptr< pipeline_cache_t > &cache
    ) : created_from< pipeline_cache_t >( cache ) {}
    pipeline_t( const pipeline_t& ) = delete;
    pipeline_t( pipeline_t&& ) = default;
    pipeline_t &operator=( const pipeline_t& ) = delete;
    pipeline_t &operator=( pipeline_t&& ) = default;
    vk::Pipeline &operator*() {
      return *handle;
    }
    const vk::Pipeline &operator*() const {
      return *handle;
    }
    vk::Pipeline *operator->() {
      return &handle.get();
    }
    const vk::Pipeline *operator->() const {
      return &handle.get();
    }
  protected:
    vk::UniqueHandle< vk::Pipeline, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
  };
}

#endif

