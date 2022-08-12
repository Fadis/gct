#include <gct/descriptor_set_layout.hpp>
#include <gct/pipeline_vertex_input_state_create_info.hpp>
#include <vulkan2json/PipelineVertexInputStateCreateInfo.hpp>
#include <vulkan2json/VertexInputBindingDescription.hpp>
#include <vulkan2json/VertexInputAttributeDescription.hpp>
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
#include <vulkan2json/PipelineVertexInputDivisorStateCreateInfoEXT.hpp>
#endif

namespace gct {
  void to_json( nlohmann::json &root, const pipeline_vertex_input_state_create_info_t &v ) {
     root = nlohmann::json::object();
     root[ "basic" ] = v.get_basic();
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( divisor ) 
#endif
     root[ "vertex_input_binding" ] = nlohmann::json::array();
     for( const auto &v: v.get_vertex_input_binding_description() ) {
       root[ "vertex_input_binding" ].push_back( v );
     }
     root[ "vertex_input_attribute" ] = nlohmann::json::array();
     for( const auto &v: v.get_vertex_input_attribute_description() ) {
       root[ "vertex_input_attribute" ].push_back( v );
     }
  }
  void from_json( const nlohmann::json &root, pipeline_vertex_input_state_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to pipeline_vertex_input_state_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( divisor ) 
#endif
    for( const auto &e: root[ "vertex_input_binding" ] ) {
      v.add_vertex_input_binding_description( e );
    }
    for( const auto &e: root[ "vertex_input_attribute" ] ) {
      v.add_vertex_input_attribute_description( e );
    }
    v.rebuild_chain();
  }
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

