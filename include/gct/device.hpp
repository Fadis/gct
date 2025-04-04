#ifndef GCT_DEVICE_HPP
#define GCT_DEVICE_HPP
#include <memory>
#include <vector>
#include <cstdint>
#include <string>
#include <unordered_set>
#include <filesystem>
#include <vulkan/vulkan.hpp>
#include <gct/physical_device.hpp>
#include <gct/queue.hpp>
#include <gct/created_from.hpp>
#include <gct/vulkan_handle.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <gct/acceleration_structure_build_sizes_info.hpp>
#endif
struct VmaAllocatorCreateInfo;
namespace gct {
  struct device_group_t;
  struct device_create_info_t;
  struct instance_t;
  class allocator_t;
  class surface_t;
  class swapchain_t;
  class swapchain_create_info_t;
  class descriptor_pool_create_info_t;
  class descriptor_pool_t;
  class descriptor_set_layout_create_info_t;
  class descriptor_set_layout_t;
  class query_pool_create_info_t;
  class query_pool_t;
  class pipeline_cache_t;
  class pipeline_cache_create_info_t;
  class pipeline_layout_t;
  class pipeline_layout_create_info_t;
  class render_pass_create_info_t;
  class render_pass_t;
#if defined(VK_VERSION_1_2) || defined(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME)
  class render_pass_create_info2_t;
  class render_pass2_t;
#endif
  class shader_module_create_info_t;
  class shader_module_t;
  class shader_module_reflection_t;
#if defined(VK_VERSION_1_3) || defined(VK_EXT_SHADER_OBJECT_EXTENSION_NAME)
  class shader_create_info_t;
  class shader_t;
#endif
  class sampler_create_info_t;
  class sampler_t;
  class semaphore_create_info_t;
  class semaphore_t;
  class fence_create_info_t;
  class fence_t;
  class queue_t;
  class command_pool_t;
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
  class acceleration_structure_build_geometry_info_t;
#endif
#ifdef VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME
  class deferred_operation_t;
#endif
#ifdef VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
  class validation_cache_t;
  class validation_cache_create_info_t;
#endif
  class device_t :
    public vulkan_handle< vk::Device >,
    public created_from< instance_t >,
    public std::enable_shared_from_this< device_t > {
  public:
    device_t(
      const device_group_t &group_,
      const activated_queue_mappings_t &queue_mappings_,
      const activated_queue_family_counts_t &queue_family_counts_,
      const device_create_info_t &create_info_
    );
    [[nodiscard]] std::shared_ptr< queue_t > get_queue( std::uint32_t );
    [[nodiscard]] std::shared_ptr< allocator_t > get_allocator();
    [[nodiscard]] std::shared_ptr< allocator_t > get_allocator( const VmaAllocatorCreateInfo& );
    [[nodiscard]] std::shared_ptr< swapchain_t > get_swapchain( const swapchain_create_info_t& );
    [[nodiscard]] std::shared_ptr< swapchain_t > get_swapchain( const std::shared_ptr< surface_t > &surface );
    [[nodiscard]] std::shared_ptr< descriptor_pool_t > get_descriptor_pool( const descriptor_pool_create_info_t& );
    [[nodiscard]] std::shared_ptr< descriptor_set_layout_t > get_descriptor_set_layout( const descriptor_set_layout_create_info_t& );
    [[nodiscard]] std::shared_ptr< descriptor_set_layout_t > get_descriptor_set_layout( const shader_module_reflection_t &reflection, std::uint32_t set_id = 0u );
    [[nodiscard]] std::shared_ptr< descriptor_set_layout_t > get_descriptor_set_layout( const std::vector< std::filesystem::path > &path, std::uint32_t set_id = 0u );
    [[nodiscard]] std::shared_ptr< descriptor_set_layout_t > get_descriptor_set_layout();
    [[nodiscard]] std::shared_ptr< query_pool_t > get_query_pool( const query_pool_create_info_t& );
    [[nodiscard]] std::shared_ptr< pipeline_cache_t > get_pipeline_cache( const pipeline_cache_create_info_t& );
    [[nodiscard]] std::shared_ptr< pipeline_cache_t > get_pipeline_cache();
    [[nodiscard]] std::shared_ptr< pipeline_layout_t > get_pipeline_layout( const pipeline_layout_create_info_t& );
    [[nodiscard]] std::shared_ptr< render_pass_t > get_render_pass( const render_pass_create_info_t& );
#if defined(VK_VERSION_1_2) || defined(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME)
    [[nodiscard]] std::shared_ptr< render_pass2_t > get_render_pass( const render_pass_create_info2_t& );
#endif
    [[nodiscard]] std::shared_ptr< render_pass_t > get_render_pass(
      vk::Format color_format,
      vk::Format depth_format
    );
    [[nodiscard]] std::shared_ptr< shader_module_t > get_shader_module( const shader_module_create_info_t& );
    [[nodiscard]] std::shared_ptr< shader_module_t > get_shader_module( const std::string& );
#if defined(VK_VERSION_1_3) || defined(VK_EXT_SHADER_OBJECT_EXTENSION_NAME)
    [[nodiscard]] std::shared_ptr< shader_t > get_shader( const shader_create_info_t &create_info );
#endif
    [[nodiscard]] std::shared_ptr< sampler_t > get_sampler( const sampler_create_info_t& );
    [[nodiscard]] std::shared_ptr< semaphore_t > get_semaphore( const semaphore_create_info_t& );
    [[nodiscard]] std::shared_ptr< semaphore_t > get_semaphore();
    [[nodiscard]] std::shared_ptr< fence_t > get_fence( const fence_create_info_t& );
    [[nodiscard]] std::shared_ptr< fence_t > get_fence();
#ifdef VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME
    [[nodiscard]] std::shared_ptr< deferred_operation_t > get_deferred_operation();
#endif
    [[nodiscard]] const device_group_t &get_physical_device_group() const { return group; }
    [[nodiscard]] std::vector< std::uint32_t > to_queue_family_list(
      const std::vector< std::uint32_t > &logical_queue_index
    );
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
    [[nodiscard]] acceleration_structure_build_sizes_info_t get_acceleration_structure_build_size(
      vk::AccelerationStructureBuildTypeKHR build_type,
      const acceleration_structure_build_geometry_info_t &build_info,
      const std::vector< std::uint32_t > &max_primitive_counts
    );
#endif
    [[nodiscard]] std::uint32_t get_api_version() const;
    [[nodiscard]] const extension_map_t &get_activated_extensions() const;
    [[nodiscard]] const std::unordered_set< vk::Format > &get_vertex_buffer_formats() const {
      return group.devices[ 0 ]->get_vertex_buffer_formats();
    }
#ifdef VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
    [[nodiscard]] std::shared_ptr< validation_cache_t > get_validation_cache(
      const validation_cache_create_info_t &ci
    );
#endif
    [[nodiscard]] format_properties get_format_properties( vk::Format f ) const {
      return group.get_format_properties( f );
    }
  private:
    void create_command_pools( std::uint32_t activated_queue_family_index );
    device_group_t group;
    activated_queue_mappings_t queue_mappings;
    activated_queue_family_counts_t queue_family_counts;
    std::vector< std::shared_ptr< command_pool_t > > command_pools;
    std::unordered_set< vk::Format > vertex_buffer_formats;
  };

}

#endif

