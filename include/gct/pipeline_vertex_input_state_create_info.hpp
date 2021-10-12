#ifndef GCT_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO_HPP
#define GCT_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO_HPP
#include <memory>
#include <optional>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class pipeline_vertex_input_state_create_info_t {
  public:
    using self_type = pipeline_vertex_input_state_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::PipelineVertexInputStateCreateInfo )
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineVertexInputDivisorStateCreateInfoEXT , divisor )
#endif
  private:
    std::vector< vk::VertexInputBindingDescription > vertex_input_binding_description;
    std::vector< vk::VertexInputAttributeDescription > vertex_input_attribute_description;
  public:
    pipeline_vertex_input_state_create_info_t &add_vertex_input_binding_description( const vk::VertexInputBindingDescription& );
    pipeline_vertex_input_state_create_info_t &clear_vertex_input_binding_description();
    pipeline_vertex_input_state_create_info_t &add_vertex_input_attribute_description( const vk::VertexInputAttributeDescription& );
    pipeline_vertex_input_state_create_info_t &clear_vertex_input_attribute_description();
  };
}

#endif

