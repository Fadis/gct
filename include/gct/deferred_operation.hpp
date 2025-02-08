#ifndef GCT_DEFERRED_OPERATION_HPP
#define GCT_DEFERRED_OPERATION_HPP
#include <vulkan/vulkan.hpp>
#ifdef VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME
#include <gct/created_from.hpp>
#include <gct/sampler_create_info.hpp>
#include <gct/vulkan_handle.hpp>

namespace gct {
  class device_t;
  class deferred_operation_t :
    public vulkan_handle< vk::DeferredOperationKHR >,
    public created_from< device_t >,
    public std::enable_shared_from_this< deferred_operation_t > {
  public:
    deferred_operation_t(
      const std::shared_ptr< device_t >&
    );
    deferred_operation_t( const deferred_operation_t& ) = delete;
    deferred_operation_t( deferred_operation_t&& ) = default;
    deferred_operation_t &operator=( const deferred_operation_t& ) = delete;
    deferred_operation_t &operator=( deferred_operation_t&& ) = default;
  };
}

#endif

#endif


