#include <gct/shader_module.hpp>
#include <gct/pipeline_tessellation_state_create_info.hpp>

namespace gct {
  pipeline_tessellation_state_create_info_t &pipeline_tessellation_state_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( domain_origin ) 
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

