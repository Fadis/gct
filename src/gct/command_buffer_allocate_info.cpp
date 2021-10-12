#include <gct/command_buffer_allocate_info.hpp>

namespace gct {
  command_buffer_allocate_info_t &command_buffer_allocate_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

