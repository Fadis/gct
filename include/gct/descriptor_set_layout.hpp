#ifndef LIBGCT_INCLUDE_GCT_DESCRIPTOR_SET_LAYOUT_HPP
#define LIBGCT_INCLUDE_GCT_DESCRIPTOR_SET_LAYOUT_HPP

#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/descriptor_set_layout_create_info.hpp>
#include <gct/vulkan_handle.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/property.hpp>

namespace gct {
  class device_t;
  class descriptor_set_layout_t :
    public vulkan_handle< vk::DescriptorSetLayout >,
    public property< descriptor_set_layout_create_info_t >,
    public created_from< device_t >,
    std::enable_shared_from_this< descriptor_set_layout_t > {
  public:
    descriptor_set_layout_t(
      const std::shared_ptr< device_t >&,
      const descriptor_set_layout_create_info_t&
    );
  };
  void to_json( nlohmann::json &root, const descriptor_set_layout_t &v );
}

#endif
