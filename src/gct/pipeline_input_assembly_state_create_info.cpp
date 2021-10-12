#include <gct/descriptor_set_layout.hpp>
#include <gct/pipeline_input_assembly_state_create_info.hpp>

namespace gct {
  pipeline_input_assembly_state_create_info_t &pipeline_input_assembly_state_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

