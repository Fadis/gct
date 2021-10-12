#include <gct/shader_module.hpp>
#include <gct/pipeline_rasterization_state_create_info.hpp>

namespace gct {
  pipeline_rasterization_state_create_info_t &pipeline_rasterization_state_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( conservative )
#endif
#ifdef VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( depth_clip )
#endif
#ifdef VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( line )
#endif
#ifdef VK_EXT_PROVOKING_VERTEX_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( provoking_vertex )
#endif
#ifdef VK_AMD_RASTERIZATION_ORDER_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( rasterization_order )
#endif
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( stream )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

