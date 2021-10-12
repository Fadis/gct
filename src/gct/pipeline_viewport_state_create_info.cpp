#include <gct/shader_module.hpp>
#include <gct/pipeline_viewport_state_create_info.hpp>

namespace gct {
  pipeline_viewport_state_create_info_t &pipeline_viewport_state_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( coarse_sample_order ) 
    LIBGCT_EXTENSION_REBUILD_CHAIN( shading_rate_image ) 
#endif
#ifdef VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( exclusive_scissor ) 
#endif
#ifdef VK_NV_VIEWPORT_SWIZZLE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( swizzle ) 
#endif
#ifdef VK_NV_CLIP_SPACE_W_SCALING_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( w_scaling ) 
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

