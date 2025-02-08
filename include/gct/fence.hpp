#ifndef GCT_FENCE_HPP
#define GCT_FENCE_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/fence_create_info.hpp>
#include <gct/vulkan_handle.hpp>
#include <gct/property.hpp>

namespace gct {
  struct device_t;
  class fence_t :
    public vulkan_handle< vk::Fence >,
    public property< fence_create_info_t >,
    public created_from< device_t >,
    public std::enable_shared_from_this< fence_t > {
  public:
    fence_t(
      const std::shared_ptr< device_t >&,
      const fence_create_info_t&
    );
    fence_t( const fence_t& ) = delete;
    fence_t( fence_t&& ) = default;
    fence_t &operator=( const fence_t& ) = delete;
    fence_t &operator=( fence_t&& ) = default;
  };
}

#endif

