#include <gct/shader_module.hpp>
#include <gct/pipeline_multisample_state_create_info.hpp>

namespace gct {
  pipeline_multisample_state_create_info_t &pipeline_multisample_state_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_NV_FRAMEBUFFER_MIXED_SAMPLES_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( coverage_modulation )
#endif
#ifdef VK_NV_COVERAGE_REDUCTION_MODE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( coverage_reduction )
#endif
#ifdef VK_NV_FRAGMENT_COVERAGE_TO_COLOR_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( coverage_to_color )
#endif
#ifdef VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( sample_locations )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

