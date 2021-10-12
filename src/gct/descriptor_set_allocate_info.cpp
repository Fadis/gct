#include <gct/descriptor_set_layout.hpp>
#include <gct/descriptor_set_allocate_info.hpp>

namespace gct {
  descriptor_set_allocate_info_t &descriptor_set_allocate_info_t::rebuild_chain() {
    if( chained ) return *this;
    raw_layout.clear();
    raw_layout.reserve( layout.size() );
    std::transform(
      layout.begin(),
      layout.end(),
      std::back_inserter( raw_layout ),
      []( auto &v ) {
        return **v;
      }
    );
    basic
      .setDescriptorSetCount( raw_layout.size() )
      .setPSetLayouts( raw_layout.data() );
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#if defined(VK_VERSION_1_2) || defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( variable_descriptor_count )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  descriptor_set_allocate_info_t &descriptor_set_allocate_info_t::add_layout( const std::shared_ptr< descriptor_set_layout_t > &v ) {
    layout.push_back( v );
    chained = false;
    return *this;
  }
  descriptor_set_allocate_info_t &descriptor_set_allocate_info_t::clear_layout() {
    layout.clear();
    chained = false;
    return *this;
  }
}

