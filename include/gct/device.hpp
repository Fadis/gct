#ifndef GCT_DEVICE_HPP
#define GCT_DEVICE_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/physical_device.hpp>
#include <gct/command_pool.hpp>
#include <gct/created_from.hpp>
#include <gct/swapchain.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/render_pass.hpp>

namespace gct {
  struct device_group_t;
  struct device_create_info_t;
  struct instance_t;
  class allocator_t;
  class surface_t;
  class swapchain_create_info_t;
  class descriptor_pool_create_info_t;
  class descriptor_set_layout_create_info_t;
  class pipeline_cache_t;
  class pipeline_cache_create_info_t;
  class pipeline_layout_t;
  class pipeline_layout_create_info_t;
  class render_pass_t;
  class shader_module_create_info_t;
  class shader_module_t;
  class sampler_create_info_t;
  class sampler_t;
  class semaphore_create_info_t;
  class semaphore_t;
  class fence_create_info_t;
  class fence_t;
  class queue_t;
  class device_t : public created_from< instance_t >, public std::enable_shared_from_this< device_t > {
  public:
    device_t(
      const device_group_t &group_,
      const activated_queue_mappings_t &queue_mappings_,
      const activated_queue_family_counts_t &queue_family_counts_,
      const device_create_info_t &create_info_
    );
    vk::Device &operator*() {
      return *handle;
    }
    const vk::Device &operator*() const {
      return *handle;
    }
    vk::Device* operator->() {
      return &handle.get();
    }
    const vk::Device* operator->() const {
      return &handle.get();
    }
    std::shared_ptr< queue_t > get_queue( std::uint32_t );
    std::shared_ptr< allocator_t > get_allocator();
    std::shared_ptr< swapchain_t > get_swapchain( const swapchain_create_info_t& );
    std::shared_ptr< swapchain_t > get_swapchain( const std::shared_ptr< surface_t > &surface );
    std::shared_ptr< descriptor_pool_t > get_descriptor_pool( const descriptor_pool_create_info_t& );
    std::shared_ptr< descriptor_set_layout_t > get_descriptor_set_layout( const descriptor_set_layout_create_info_t& );
    std::shared_ptr< pipeline_cache_t > get_pipeline_cache( const pipeline_cache_create_info_t& );
    std::shared_ptr< pipeline_cache_t > get_pipeline_cache();
    std::shared_ptr< pipeline_layout_t > get_pipeline_layout( const pipeline_layout_create_info_t& );
    std::shared_ptr< render_pass_t > get_render_pass( const render_pass_create_info_t& );
    std::shared_ptr< shader_module_t > get_shader_module( const shader_module_create_info_t& );
    std::shared_ptr< sampler_t > get_sampler( const sampler_create_info_t& );
    std::shared_ptr< semaphore_t > get_semaphore( const semaphore_create_info_t& );
    std::shared_ptr< semaphore_t > get_semaphore();
    std::shared_ptr< fence_t > get_fence( const fence_create_info_t& );
    std::shared_ptr< fence_t > get_fence();
    const device_group_t &get_physical_device_group() const { return group; }
    std::vector< std::uint32_t > to_queue_family_list(
      const std::vector< std::uint32_t > &logical_queue_index
    );
  private:
    void create_command_pools( std::uint32_t activated_queue_family_index );
    device_group_t group;
    activated_queue_mappings_t queue_mappings;
    activated_queue_family_counts_t queue_family_counts;
    vk::UniqueHandle< vk::Device, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
    std::vector< std::shared_ptr< command_pool_t > > command_pools;
  };
}

#endif

