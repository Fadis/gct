#ifndef GCT_ASYNC_HPP
#define GCT_ASYNC_HPP
#include <future>
#include <vulkan/vulkan.hpp>
namespace gct {
  class deferred_operation_t;
  std::future< vk::Result > async(
    const std::shared_ptr< deferred_operation_t > &
  );
  void async(
    const std::shared_ptr< deferred_operation_t > &,
    std::function< void( vk::Result ) >&
  );
}
#endif

