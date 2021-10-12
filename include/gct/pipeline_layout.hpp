#ifndef GCT_PIPELINE_LAYOUT_HPP
#define GCT_PIPELINE_LAYOUT_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/pipeline_layout_create_info.hpp>

namespace gct {
  struct device_t;
  class pipeline_layout_t : public created_from< device_t >, public std::enable_shared_from_this< pipeline_layout_t > {
  public:
    pipeline_layout_t(
      const std::shared_ptr< device_t >&,
      const pipeline_layout_create_info_t&
    );
    pipeline_layout_t( const pipeline_layout_t& ) = delete;
    pipeline_layout_t( pipeline_layout_t&& ) = default;
    pipeline_layout_t &operator=( const pipeline_layout_t& ) = delete;
    pipeline_layout_t &operator=( pipeline_layout_t&& ) = default;
    const pipeline_layout_create_info_t &get_props() const { return props; }
    vk::PipelineLayout &operator*() {
      return *handle;
    }
    const vk::PipelineLayout &operator*() const {
      return *handle;
    }
    vk::PipelineLayout *operator->() {
      return &handle.get();
    }
    const vk::PipelineLayout *operator->() const {
      return &handle.get();
    }
  private:
    pipeline_layout_create_info_t props;
    vk::UniqueHandle< vk::PipelineLayout, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
  };
}

#endif

