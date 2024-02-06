#ifndef GCT_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO_HPP
#define GCT_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO_HPP
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>
#include <gct/array_of.hpp>
namespace gct {
  class pipeline_color_blend_state_create_info_t : public chained_t {
  public:
    using self_type = pipeline_color_blend_state_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::PipelineColorBlendStateCreateInfo )
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineColorBlendAdvancedStateCreateInfoEXT, advanced )
#endif
#ifdef VK_EXT_COLOR_WRITE_ENABLE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineColorWriteCreateInfoEXT, write )
    LIBGCT_ARRAY_OF_SMALL( VkBool32, color_write_enable )
#endif
    LIBGCT_ARRAY_OF( vk::PipelineColorBlendAttachmentState, attachment )
  public:
    pipeline_color_blend_state_create_info_t &add_attachment();
  };
  void to_json( nlohmann::json &root, const pipeline_color_blend_state_create_info_t &v );
  void from_json( const nlohmann::json &root, pipeline_color_blend_state_create_info_t &v );
}

#endif

