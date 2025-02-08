#ifndef GCT_SEMAPHORE_HPP
#define GCT_SEMAPHORE_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/semaphore_create_info.hpp>
#include <gct/vulkan_handle.hpp>
#include <gct/property.hpp>

namespace gct {
  struct device_t;
  class semaphore_t :
    public vulkan_handle< vk::Semaphore >,
    public property< semaphore_create_info_t >,
    public created_from< device_t >,
    public std::enable_shared_from_this< semaphore_t > {
  public:
    semaphore_t(
      const std::shared_ptr< device_t >&,
      const semaphore_create_info_t&
    );
    semaphore_t( const semaphore_t& ) = delete;
    semaphore_t( semaphore_t&& ) = default;
    semaphore_t &operator=( const semaphore_t& ) = delete;
    semaphore_t &operator=( semaphore_t&& ) = default;
  };
}

#endif

