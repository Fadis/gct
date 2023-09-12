#include <nlohmann/json.hpp>
#include <gct/shader_module.hpp>
#include <gct/pipeline_color_blend_state_create_info.hpp>
#include <vulkan2json/PipelineColorBlendStateCreateInfo.hpp>
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
#include <vulkan2json/PipelineColorBlendAdvancedStateCreateInfoEXT.hpp>
#endif
#ifdef VK_EXT_COLOR_WRITE_ENABLE_EXTENSION_NAME
#include <vulkan2json/PipelineColorWriteCreateInfoEXT.hpp>
#endif

namespace gct {
  void to_json( nlohmann::json &root, const pipeline_color_blend_state_create_info_t &v ) {
     root = nlohmann::json::object();
     root[ "basic" ] = v.get_basic();
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( advanced )
#endif
#ifdef VK_EXT_COLOR_WRITE_ENABLE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( write )
#endif
  }
  void from_json( const nlohmann::json &root, pipeline_color_blend_state_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to pipeline_color_blend_state_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( advanced )
#endif
#ifdef VK_EXT_COLOR_WRITE_ENABLE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( write )
#endif
  }
  pipeline_color_blend_state_create_info_t &pipeline_color_blend_state_create_info_t::rebuild_chain() {
    basic
      .setAttachmentCount( attachment.size() )
      .setPAttachments( attachment.data() );
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( advanced )
#endif
#ifdef VK_EXT_COLOR_WRITE_ENABLE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( write )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  pipeline_color_blend_state_create_info_t &pipeline_color_blend_state_create_info_t::add_attachment( const vk::PipelineColorBlendAttachmentState &v ) {
    attachment.push_back( v );
    chained = false;
    return *this;
  }
  pipeline_color_blend_state_create_info_t &pipeline_color_blend_state_create_info_t::clear_attachment() {
    attachment.clear();
    chained = false;
    return *this;
  }
  pipeline_color_blend_state_create_info_t &pipeline_color_blend_state_create_info_t::add_attachment() {
    add_attachment(
      vk::PipelineColorBlendAttachmentState()
        .setBlendEnable( false )
        .setColorWriteMask(
          vk::ColorComponentFlagBits::eR |
          vk::ColorComponentFlagBits::eG |
          vk::ColorComponentFlagBits::eB |
          vk::ColorComponentFlagBits::eA
        )
    );
    chained = false;
    return *this;
  }
}

