#ifndef LIBGCT_INCLUDE_GCT_DESCRIPTOR_POOL_HPP
#define LIBGCT_INCLUDE_GCT_DESCRIPTOR_POOL_HPP

#include <memory>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/created_from.hpp>
#include <gct/descriptor_pool_create_info.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/vulkan_handle.hpp>
#include <gct/property.hpp>

namespace gct {
  class descriptor_set_allocate_info_t;
  class descriptor_set_layout_t;
  class descriptor_pool_t :
    public vulkan_handle< vk::DescriptorPool >,
    public property< descriptor_pool_create_info_t >,
    public created_from< device_t >,
    public std::enable_shared_from_this< descriptor_pool_t > {
  public:
    descriptor_pool_t(
      const std::shared_ptr< device_t >&,
      const descriptor_pool_create_info_t&
    );
    [[nodiscard]] std::shared_ptr< descriptor_set_t > allocate(
      const descriptor_set_allocate_info_t&
    );
    [[nodiscard]] std::shared_ptr< descriptor_set_t > allocate(
      const std::shared_ptr< descriptor_set_layout_t >&
    );
    [[nodiscard]] std::shared_ptr< descriptor_set_t > allocate(
      const std::shared_ptr< descriptor_set_layout_t >&,
      std::uint32_t
    );
  };
  void to_json( nlohmann::json &dest, const descriptor_pool_t &src );
}

#endif
