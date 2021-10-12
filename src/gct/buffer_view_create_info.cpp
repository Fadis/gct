#include <gct/buffer_view_create_info.hpp>

namespace gct {
  buffer_view_create_info_t &buffer_view_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

