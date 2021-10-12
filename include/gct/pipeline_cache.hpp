#ifndef LIBGCT_INCLUDE_GCT_PIPELINE_CACHE_HPP
#define LIBGCT_INCLUDE_GCT_PIPELINE_CACHE_HPP

#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/pipeline_cache_create_info.hpp>

namespace gct {
  class compute_pipeline_t;
  class compute_pipeline_create_info_t;
  class graphics_pipeline_t;
  class graphics_pipeline_create_info_t;
  class pipeline_cache_t : public created_from< device_t >, public std::enable_shared_from_this< pipeline_cache_t > {
  public:
    pipeline_cache_t(
      const std::shared_ptr< device_t >&,
      const pipeline_cache_create_info_t&
    );
    vk::PipelineCache &operator*() {
      return *handle;
    }
    const vk::PipelineCache &operator*() const {
      return *handle;
    }
    vk::PipelineCache* operator->() {
      return &handle.get();
    }
    const vk::PipelineCache* operator->() const {
      return &handle.get();
    }
    const pipeline_cache_create_info_t &get_props() const {
      return props;
    }
    std::shared_ptr< compute_pipeline_t > get_pipeline(
      const compute_pipeline_create_info_t&
    );
    std::shared_ptr< graphics_pipeline_t > get_pipeline(
      const graphics_pipeline_create_info_t&
    );
  private:
    pipeline_cache_create_info_t props;
    vk::UniqueHandle< vk::PipelineCache, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
  };

}

#endif
