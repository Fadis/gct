#ifndef GCT_DESCRIPTOR_SET_LAYOUT_CREATE_INFO_HPP
#define GCT_DESCRIPTOR_SET_LAYOUT_CREATE_INFO_HPP
#include <memory>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class descriptor_set_layout_create_info_t {
  public:
    using self_type = descriptor_set_layout_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::DescriptorSetLayoutCreateInfo )
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::DescriptorSetLayoutBindingFlagsCreateInfo , binding_flags )
#elif defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::DescriptorSetLayoutBindingFlagsCreateInfoEXT , binding_flags )
#endif
#ifdef VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::MutableDescriptorTypeCreateInfoVALVE , mutable_descriptor_type )
#endif
  private:
    std::vector< vk::DescriptorSetLayoutBinding > binding;
  public:
    descriptor_set_layout_create_info_t &add_binding( const vk::DescriptorSetLayoutBinding &v );
    descriptor_set_layout_create_info_t &clear_binding();
  };
  void to_json( nlohmann::json &root, const descriptor_set_layout_create_info_t &v );
  void from_json( const nlohmann::json &root, descriptor_set_layout_create_info_t &v );
}

#endif

