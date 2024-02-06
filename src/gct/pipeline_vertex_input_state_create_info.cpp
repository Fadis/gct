#include <nlohmann/json.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/pipeline_vertex_input_state_create_info.hpp>
#include <vulkan2json/PipelineVertexInputStateCreateInfo.hpp>
#include <vulkan2json/VertexInputBindingDescription.hpp>
#include <vulkan2json/VertexInputAttributeDescription.hpp>
#ifdef VK_KHR_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
#include <vulkan2json/PipelineVertexInputDivisorStateCreateInfoKHR.hpp>
#include <vulkan2json/VertexInputBindingDivisorDescriptionKHR.hpp>
#elif defined(VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME)
#include <vulkan2json/PipelineVertexInputDivisorStateCreateInfoEXT.hpp>
#include <vulkan2json/VertexInputBindingDivisorDescriptionEXT.hpp>
#endif

namespace gct {
  void to_json( nlohmann::json &root, const pipeline_vertex_input_state_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
    LIBGCT_ARRAY_OF_TO_JSON( basic, pVertexBindingDescriptions, vertex_input_binding_description )
    LIBGCT_ARRAY_OF_TO_JSON( basic, pVertexAttributeDescriptions, vertex_input_attribute_description )
#if defined(VK_KHR_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME) || defined(VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( divisor ) 
    LIBGCT_ARRAY_OF_TO_JSON( divisor , pVertexBindingDivisors, divisor_description )
#endif
  }
  void from_json( const nlohmann::json &root, pipeline_vertex_input_state_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to pipeline_vertex_input_state_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
    LIBGCT_ARRAY_OF_FROM_JSON( basic, pVertexBindingDescriptions, vertex_input_binding_description )
    LIBGCT_ARRAY_OF_FROM_JSON( basic, pVertexAttributeDescriptions, vertex_input_attribute_description )
#if defined(VK_KHR_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME) || defined(VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( divisor ) 
    LIBGCT_ARRAY_OF_FROM_JSON( divisor , pVertexBindingDivisors, divisor_description )
#endif
    v.rebuild_chain();
  }
  pipeline_vertex_input_state_create_info_t &pipeline_vertex_input_state_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( basic, VertexBindingDescriptionCount, PVertexBindingDescriptions, vertex_input_binding_description )
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( basic, VertexAttributeDescriptionCount, PVertexAttributeDescriptions, vertex_input_attribute_description )
#if defined(VK_KHR_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME) || defined(VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( divisor ) 
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( divisor, VertexBindingDivisorCount, PVertexBindingDivisors, divisor_description )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

