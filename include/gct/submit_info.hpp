#ifndef GCT_SUBMIT_INFO_HPP
#define GCT_SUBMIT_INFO_HPP
#include <memory>
#include <optional>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class semaphore_t;
  class submit_info_t {
  public:
    using self_type = submit_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::SubmitInfo )
#ifdef VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::D3D12FenceSubmitInfoKHR, d3d12_fence )
#endif
#ifdef VK_VERSION1_1
    LIBGCT_EXTENSION_SETTER( vk::DeviceGroupSubmitInfo, device_group )
#elif defined(VK_KHR_DEVICE_GROUP_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::DeviceGroupSubmitInfoKHR, device_group )
#endif
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PerformanceQuerySubmitInfoKHR, performance_query )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::ProtectedSubmitInfo, protected_ )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::TimelineSemaphoreSubmitInfo, timeline_semaphore )
#elif defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::TimelineSemaphoreSubmitInfoKHR, timeline_semaphore )
#endif
#ifdef VK_KHR_WIN32_KEYED_MUTEX_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::Win32KeyedMutexAcquireReleaseInfoKHR, win32_keyed_mutex_acquire_release )
#endif
#ifdef VK_NV_WIN32_KEYED_MUTEX_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::Win32KeyedMutexAcquireReleaseInfoNV, win32_keyed_mutex_acquire_release_nv )
#endif
  private:
    std::vector< std::shared_ptr< semaphore_t > > wait;
    std::vector< vk::Semaphore > wait_raw;
    std::vector< vk::PipelineStageFlags > stage;
    std::vector< std::shared_ptr< semaphore_t > > signal;
    std::vector< vk::Semaphore > signal_raw;
  public:
    submit_info_t &add_wait_for( const std::shared_ptr< semaphore_t > &s, vk::PipelineStageFlags );
    submit_info_t &clear_wait_for();
    submit_info_t &add_signal_to( const std::shared_ptr< semaphore_t > &s );
    submit_info_t &clear_signal_to();
  };
}

#endif

