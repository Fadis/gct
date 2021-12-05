#include <gct/descriptor_set_layout_create_info.hpp>
#include <vulkan2json/DescriptorSetLayoutCreateInfo.hpp>
#ifdef VK_VERSION_1_2
#include <vulkan2json/DescriptorSetLayoutBindingFlagsCreateInfo.hpp>
#elif defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
#include <vulkan2json/DescriptorSetLayoutBindingFlagsCreateInfoEXT.hpp>
#endif
#ifdef VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
#include <vulkan2json/MutableDescriptorTypeCreateInfoVALVE.hpp>
#endif

namespace gct {
  void to_json( nlohmann::json &root, const descriptor_set_layout_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
#if defined(VK_VERSION_1_2) || defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( binding_flags )
#endif
#ifdef VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( mutable_descriptor_type )
#endif
  }
  void from_json( const nlohmann::json &root, descriptor_set_layout_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to descriptor_set_layout_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#if defined(VK_VERSION_1_2) || defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( binding_flags )
#endif
#ifdef VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( mutable_descriptor_type )
#endif
  }
  descriptor_set_layout_create_info_t &descriptor_set_layout_create_info_t::rebuild_chain() {
    basic
      .setBindingCount( binding.size() )
      .setPBindings( binding.empty() ? nullptr : binding.data() );
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#if defined(VK_VERSION_1_2) || defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( binding_flags )
#endif
#ifdef VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( mutable_descriptor_type )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  descriptor_set_layout_create_info_t &descriptor_set_layout_create_info_t::add_binding( const vk::DescriptorSetLayoutBinding &v ) {
    binding.push_back( v );
    chained = false;
    return *this;
  }
  descriptor_set_layout_create_info_t &descriptor_set_layout_create_info_t::clear_binding() {
    binding.clear();
    chained = false;
    return *this;
  }
}

