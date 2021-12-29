#include <gct/allocator.hpp>
#include <gct/device_create_info.hpp>
#include <gct/instance.hpp>
#include <gct/swapchain.hpp>
#include <gct/surface.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/pipeline_cache_create_info.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/pipeline_layout_create_info.hpp>
#include <gct/render_pass.hpp>
#include <gct/render_pass_create_info.hpp>
#include <gct/shader_module.hpp>
#include <gct/shader_module_create_info.hpp>
#include <gct/sampler.hpp>
#include <gct/sampler_create_info.hpp>
#include <gct/semaphore.hpp>
#include <gct/semaphore_create_info.hpp>
#include <gct/fence.hpp>
#include <gct/fence_create_info.hpp>
#include <gct/queue.hpp>
#include <gct/device.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <gct/acceleration_structure_build_geometry_info.hpp>
#include <gct/acceleration_structure_geometry.hpp>
#endif
#ifdef VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME
#include <gct/deferred_operation.hpp>
#endif
namespace gct {

  device_t::device_t(
    const device_group_t &group_,
    const activated_queue_mappings_t &queue_mappings_,
    const activated_queue_family_counts_t &queue_family_counts_,
    const device_create_info_t &extra
  ) :
    created_from< instance_t >( group_.devices[ 0 ]->get_factory() ),
    group( group_ ),
    queue_mappings( queue_mappings_ ),
    queue_family_counts( queue_family_counts_ ) {
    std::vector< vk::DeviceQueueCreateInfo > queue_create_info;
#ifdef VK_EXT_GLOBAL_PRIORITY_EXTENSION_NAME
    std::vector< vk::DeviceQueueGlobalPriorityCreateInfoEXT > queue_global_priority_create_info;
#endif
    std::sort( queue_family_counts.begin(), queue_family_counts.end(), []( const auto &l, const auto &r ) { return l.activated_queue_family_index < r.activated_queue_family_index; } );
    std::transform(
      queue_family_counts.begin(),
      queue_family_counts.end(),
      std::back_inserter( queue_create_info ),
      [&]( const auto &q ) {
        vk::DeviceQueueCreateInfo ci;
        ci.queueFamilyIndex = q.available_queue_family_index;
        ci.queueCount = q.count;
        ci.pQueuePriorities = q.local_priority.data();
#ifdef VK_EXT_GLOBAL_PRIORITY_EXTENSION_NAME
        if( q.global_priority ) {
          vk::DeviceQueueGlobalPriorityCreateInfoEXT global_priority;
          global_priority.globalPriority = *q.global_priority;
          queue_global_priority_create_info.push_back( global_priority );
          ci.pNext = reinterpret_cast< const void* >( &queue_global_priority_create_info.back() );
        }
#endif
        return ci;
      }
    );
    auto create_info = extra;
    const auto &features = group_.devices[ 0 ]->get_features();
    const bool use_extra_features =
#if defined(VK_VERSION_1_1) || defined(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)
      group_.devices[ 0 ]->get_factory()->get_api_version() >= VK_MAKE_VERSION( 1, 1, 0 ) ||
      group_.devices[ 0 ]->get_factory()->get_activated_extensions().find( VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME ) != group_.devices[ 0 ]->get_factory()->get_activated_extensions().end();
#else
      false;
#endif
    create_info
      .set_basic(
        vk::DeviceCreateInfo()
          .setQueueCreateInfoCount( queue_create_info.size() )
          .setPQueueCreateInfos( queue_create_info.data() )
          .setEnabledLayerCount( group_.devices[ 0 ]->get_props().get_activated_layer_names().size() )
          .setPpEnabledLayerNames( group_.devices[ 0 ]->get_props().get_activated_layer_names().data() )
          .setEnabledExtensionCount( group_.devices[ 0 ]->get_props().get_activated_extension_names().size() )
          .setPpEnabledExtensionNames( group_.devices[ 0 ]->get_props().get_activated_extension_names().data() )
          .setPEnabledFeatures(
            !use_extra_features ?
            const_cast< vk::PhysicalDeviceFeatures* >( &features.get_basic() ) :
            static_cast< vk::PhysicalDeviceFeatures* >( nullptr )
          )
      );
#if defined(VK_VERSION_1_1) || defined(VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME)
    std::vector< vk::PhysicalDevice > devices;
    if(
      group_.devices[ 0 ]->get_factory()->get_api_version() >= VK_MAKE_VERSION( 1, 1, 0 ) ||
      group_.devices[ 0 ]->get_props().get_activated_extensions().find( "VK_KHR_device_group_creation" ) != group_.devices[ 0 ]->get_props().get_activated_extensions().end()
    ) {
      devices.reserve( group_.devices.size() );
      std::transform(
        group_.devices.begin(),
        group_.devices.end(),
        std::back_inserter( devices ),
        []( const auto &v ) { return **v; }
      );
      create_info
        .set_device_group(
          vk::DeviceGroupDeviceCreateInfo()
            .setPhysicalDeviceCount( devices.size() )
            .setPPhysicalDevices( devices.data() )
        );
    }
#endif
    create_info
      .rebuild_chain();
    auto tail = get_chain_tail( create_info.get_head() );
    if( use_extra_features )
      *tail = const_cast< void* >( static_cast< const void* >( &features.get_basic2() ) );
    handle = (*group_.devices[ 0 ])->createDeviceUnique(
      create_info.get_basic(),
      nullptr
    );
    command_pools.resize( queue_family_counts.size() );
  }
  void device_t::create_command_pools( std::uint32_t activated_queue_family_index ) {
    if( !command_pools[ activated_queue_family_index ] )
      command_pools[ activated_queue_family_index ].reset(
        new command_pool_t(
          shared_from_this(),
          queue_family_counts[ activated_queue_family_index ]
        )
      );
  }
  std::shared_ptr< queue_t > device_t::get_queue( std::uint32_t index ) {
    create_command_pools( queue_mappings[ index ].activated_queue_family_index );
    return std::shared_ptr< queue_t >(
      new queue_t(
        shared_from_this(),
        handle->getQueue(
          queue_mappings[ index ].available_queue_family_index,
          queue_mappings[ index ].queue_count_index
        ),
        command_pools[ queue_mappings[ index ].activated_queue_family_index ],
        queue_mappings[ index ].available_queue_family_index
      )
    );
  }
  std::shared_ptr< allocator_t > device_t::get_allocator( const VmaAllocatorCreateInfo &create_info ) {
    return std::shared_ptr< allocator_t >(
      new allocator_t(
        shared_from_this(),
        create_info
      )
    );
  }
  std::shared_ptr< allocator_t > device_t::get_allocator() {
    return get_allocator(
      VmaAllocatorCreateInfo{}
    );
  }
  std::shared_ptr< swapchain_t > device_t::get_swapchain( const swapchain_create_info_t &create_info ) {
    return std::shared_ptr< swapchain_t >(
      new swapchain_t(
        shared_from_this(),
        create_info
      )
    );
  }
  std::shared_ptr< swapchain_t > device_t::get_swapchain( const std::shared_ptr< surface_t > &surface ) {
    return get_swapchain(
      swapchain_create_info_t()
        .set_basic(
          vk::SwapchainCreateInfoKHR()
            .setImageSharingMode( vk::SharingMode::eExclusive )
            .setCompositeAlpha( vk::CompositeAlphaFlagBitsKHR::eOpaque )
            .setImageUsage( vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eColorAttachment )
            .setPresentMode( vk::PresentModeKHR::eFifo )
            .setClipped( true )
        )
        .set_surface( surface )
        .rebuild_chain()
    );
  }
  std::vector< std::uint32_t > device_t::to_queue_family_list(
    const std::vector< std::uint32_t > &logical_queue_index
  ) {
    std::vector< std::uint32_t > family_list;
    family_list.reserve( logical_queue_index.size() );
    std::transform(
      logical_queue_index.begin(),
      logical_queue_index.end(),
      std::back_inserter( family_list ),
      [&]( std::uint32_t l ) {
        return queue_mappings[ l ].available_queue_family_index;
      }
    );
    std::sort( family_list.begin(), family_list.end() );
    family_list.erase( std::unique( family_list.begin(), family_list.end() ), family_list.end() );
    return family_list;
  }
  std::shared_ptr< descriptor_pool_t > device_t::get_descriptor_pool( const descriptor_pool_create_info_t &create_info ) {
    return std::shared_ptr< descriptor_pool_t >(
      new descriptor_pool_t(
        shared_from_this(),
        create_info
      )
    );
  }
  std::shared_ptr< descriptor_set_layout_t > device_t::get_descriptor_set_layout( const descriptor_set_layout_create_info_t &create_info ) {
    return std::shared_ptr< descriptor_set_layout_t >(
      new descriptor_set_layout_t(
        shared_from_this(),
        create_info
      )
    );
  }
  std::shared_ptr< pipeline_cache_t > device_t::get_pipeline_cache( const pipeline_cache_create_info_t &create_info ) {
    return std::shared_ptr< pipeline_cache_t >(
      new pipeline_cache_t(
        shared_from_this(),
        create_info
      )
    );
  }
  std::shared_ptr< pipeline_cache_t > device_t::get_pipeline_cache() {
    return get_pipeline_cache(
      pipeline_cache_create_info_t()
        .rebuild_chain()
    );
  }
  std::shared_ptr< pipeline_layout_t > device_t::get_pipeline_layout( const pipeline_layout_create_info_t &create_info ) {
    return std::shared_ptr< pipeline_layout_t >(
      new pipeline_layout_t(
        shared_from_this(),
        create_info
      )
    );
  }
  std::shared_ptr< render_pass_t > device_t::get_render_pass( const render_pass_create_info_t &create_info ) {
    return std::shared_ptr< render_pass_t >(
      new render_pass_t(
        shared_from_this(),
        create_info
      )
    );
  }
  std::shared_ptr< shader_module_t > device_t::get_shader_module( const shader_module_create_info_t &create_info ) {
    return std::shared_ptr< shader_module_t >(
      new shader_module_t(
        shared_from_this(),
        create_info
      )
    );
  }
  std::shared_ptr< shader_module_t > device_t::get_shader_module( const std::string &filename ) {
    return get_shader_module(
      shader_module_create_info_t()
        .load( filename )
    );
  }
  std::shared_ptr< sampler_t > device_t::get_sampler( const sampler_create_info_t &create_info ) {
    return std::shared_ptr< sampler_t >(
      new sampler_t(
        shared_from_this(),
        create_info
      )
    );
  }
  std::shared_ptr< semaphore_t > device_t::get_semaphore( const semaphore_create_info_t &create_info ) {
    return std::shared_ptr< semaphore_t >(
      new semaphore_t(
        shared_from_this(),
        create_info
      )
    );
  }
  std::shared_ptr< semaphore_t > device_t::get_semaphore() {
    return get_semaphore(
      semaphore_create_info_t()
    );
  }
  std::shared_ptr< fence_t > device_t::get_fence( const fence_create_info_t &create_info ) {
    return std::shared_ptr< fence_t >(
      new fence_t(
        shared_from_this(),
        create_info
      )
    );
  }
  std::shared_ptr< fence_t > device_t::get_fence() {
    return get_fence(
      fence_create_info_t()
    );
  }
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
  acceleration_structure_build_sizes_info_t device_t::get_acceleration_structure_build_size(
    vk::AccelerationStructureBuildTypeKHR build_type,
    const acceleration_structure_build_geometry_info_t &build_info,
    const std::vector< std::uint32_t > &max_primitive_counts
  ) {
    auto copied = build_info;
    copied.rebuild_chain();
    acceleration_structure_build_sizes_info_t temp;
    if( copied.get_basic().geometryCount != max_primitive_counts.size() ) throw -1;
    temp.set_basic(
      handle->getAccelerationStructureBuildSizesKHR(
        build_type,
        copied.get_basic(),
        max_primitive_counts
      )
    );
    return temp;
  }
#endif
#ifdef VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME
  std::shared_ptr< deferred_operation_t > device_t::get_deferred_operation() {
    return std::shared_ptr< deferred_operation_t >(
      new deferred_operation_t(
        shared_from_this()
      )
    );
  }
#endif
  const extension_map_t &device_t::get_activated_extensions() const {
    return get_physical_device_group().devices[ 0 ]->get_props().get_activated_extensions();
  }
  std::uint32_t device_t::get_api_version() const {
    return get_physical_device_group().devices[ 0 ]->get_props().get_api_version();
  }
}

