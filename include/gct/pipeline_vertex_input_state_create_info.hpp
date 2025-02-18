#ifndef GCT_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO_HPP
#define GCT_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO_HPP
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>
#include <gct/array_of.hpp>
namespace gct {
  class pipeline_vertex_input_state_create_info_t : public chained_t {
  public:
    using self_type = pipeline_vertex_input_state_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::PipelineVertexInputStateCreateInfo )
    LIBGCT_ARRAY_OF( vk::VertexInputBindingDescription, vertex_input_binding_description )
    LIBGCT_ARRAY_OF( vk::VertexInputAttributeDescription, vertex_input_attribute_description )
#ifdef VK_KHR_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineVertexInputDivisorStateCreateInfoKHR , divisor )
    LIBGCT_ARRAY_OF( vk::VertexInputBindingDivisorDescriptionKHR , divisor_description )
#elif defined(VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PipelineVertexInputDivisorStateCreateInfoEXT , divisor )
    LIBGCT_ARRAY_OF( vk::VertexInputBindingDivisorDescriptionEXT , divisor_description )
#endif
  };
  void to_json( nlohmann::json &root, const pipeline_vertex_input_state_create_info_t &v );
  void from_json( const nlohmann::json &root, pipeline_vertex_input_state_create_info_t &v );
}

#endif

