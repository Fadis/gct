#ifndef GCT_DESCRIPTOR_SET_LAYOUT_CREATE_INFO_HPP
#define GCT_DESCRIPTOR_SET_LAYOUT_CREATE_INFO_HPP
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>
#include <gct/array_of.hpp>
#include <gct/setter.hpp>
#include <gct/shader_module_create_info.hpp>

namespace gct {
  class descriptor_set_layout_create_info_t : public chained_t {
  public:
    using self_type = descriptor_set_layout_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::DescriptorSetLayoutCreateInfo )
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::DescriptorSetLayoutBindingFlagsCreateInfo , binding_flags )
    LIBGCT_ARRAY_OF_SMALL( vk::DescriptorBindingFlags, binding_flag )
#elif defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::DescriptorSetLayoutBindingFlagsCreateInfoEXT , binding_flags )
    LIBGCT_ARRAY_OF_SMALL( vk::DescriptorBindingFlags, binding_flag )
#endif
#ifdef VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::MutableDescriptorTypeCreateInfoVALVE , mutable_descriptor_type )
#endif
  private:
    std::vector< vk::DescriptorSetLayoutBinding > binding;
    std::vector< bool > indexable;
    std::unordered_map< std::string, std::uint32_t > name_to_binding;
    std::unordered_set< std::string > non_unique_name;
    bool indexing = false;
  public:
    descriptor_set_layout_create_info_t &add_binding( const vk::DescriptorSetLayoutBinding &v, bool = false );
    descriptor_set_layout_create_info_t &add_binding( const shader_module_reflection_t &v, std::uint32_t set_id = 0u );
    descriptor_set_layout_create_info_t &clear_binding();
    [[nodiscard]] const std::vector< vk::DescriptorSetLayoutBinding > &get_binding() const { return binding; }
    [[nodiscard]] const std::vector< bool > &get_indexable() const { return indexable; }
    [[nodiscard]] const std::unordered_map< std::string, std::uint32_t > &get_name_to_binding() const { return name_to_binding; }
    descriptor_set_layout_create_info_t &set_indexing( bool );
    [[nodiscard]] bool get_indexing() const { return indexing; }
  };
  void to_json( nlohmann::json &root, const descriptor_set_layout_create_info_t &v );
  void from_json( const nlohmann::json &root, descriptor_set_layout_create_info_t &v );
}

#endif

