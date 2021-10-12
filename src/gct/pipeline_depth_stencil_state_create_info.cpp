#include <gct/shader_module.hpp>
#include <gct/pipeline_depth_stencil_state_create_info.hpp>

namespace gct {
  pipeline_depth_stencil_state_create_info_t &pipeline_depth_stencil_state_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

