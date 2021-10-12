#include <gct/shader_module.hpp>
#include <gct/pipeline_color_blend_state_create_info.hpp>

namespace gct {
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
}

