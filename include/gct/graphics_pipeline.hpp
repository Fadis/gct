#ifndef GCT_GRAPHICS_PIPELINE_HPP
#define GCT_GRAPHICS_PIPELINE_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/graphics_pipeline_create_info.hpp>

namespace gct {
  class device_t;
  class pipeline_cache_t;
  class graphics_pipeline_t : public created_from< pipeline_cache_t >, public std::enable_shared_from_this< graphics_pipeline_t > {
  public:
    graphics_pipeline_t(
      const std::shared_ptr< pipeline_cache_t >&,
      const graphics_pipeline_create_info_t&
    );
    graphics_pipeline_t( const graphics_pipeline_t& ) = delete;
    graphics_pipeline_t( graphics_pipeline_t&& ) = default;
    graphics_pipeline_t &operator=( const graphics_pipeline_t& ) = delete;
    graphics_pipeline_t &operator=( graphics_pipeline_t&& ) = default;
    const graphics_pipeline_create_info_t &get_props() const { return props; }
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
  private:
    graphics_pipeline_create_info_t props;
    vk::UniqueHandle< vk::Pipeline, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
  };
}

#endif

