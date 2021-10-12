#include <gct/descriptor_set_layout.hpp>
#include <gct/pipeline_vertex_input_state_create_info.hpp>

namespace gct {
  pipeline_vertex_input_state_create_info_t &pipeline_vertex_input_state_create_info_t::rebuild_chain() {
    basic
      .setVertexBindingDescriptionCount( vertex_input_binding_description.size() )
      .setPVertexBindingDescriptions( vertex_input_binding_description.data() )
      .setVertexAttributeDescriptionCount( vertex_input_attribute_description.size() )
      .setPVertexAttributeDescriptions( vertex_input_attribute_description.data() );
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( divisor ) 
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  pipeline_vertex_input_state_create_info_t &pipeline_vertex_input_state_create_info_t::add_vertex_input_binding_description( const vk::VertexInputBindingDescription &v ) {
    vertex_input_binding_description.push_back( v );
    chained = false;
    return *this;
  }
  pipeline_vertex_input_state_create_info_t &pipeline_vertex_input_state_create_info_t::clear_vertex_input_binding_description() {
    vertex_input_binding_description.clear();
    chained = false;
    return *this;
  }
  pipeline_vertex_input_state_create_info_t &pipeline_vertex_input_state_create_info_t::add_vertex_input_attribute_description( const vk::VertexInputAttributeDescription &v ) {
    vertex_input_attribute_description.push_back( v );
    chained = false;
    return *this;
  }
  pipeline_vertex_input_state_create_info_t &pipeline_vertex_input_state_create_info_t::clear_vertex_input_attribute_description() {
    vertex_input_attribute_description.clear();
    chained = false;
    return *this;
  }
}

