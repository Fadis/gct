#include <gct/descriptor_set_layout_create_info.hpp>

namespace gct {
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

