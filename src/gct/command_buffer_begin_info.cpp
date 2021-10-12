#include <gct/command_buffer_begin_info.hpp>

namespace gct {
  command_buffer_begin_info_t &command_buffer_begin_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#if defined(VK_VERSION_1_1) || defined(VK_STRUCTURE_TYPE_DEVICE_GROUP_PRESENT_CAPABILITIES_KHR)
    LIBGCT_EXTENSION_REBUILD_CHAIN( device_group )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

