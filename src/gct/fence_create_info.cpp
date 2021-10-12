#include <gct/fence_create_info.hpp>

namespace gct {
  fence_create_info_t &fence_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#if defined(VK_VERSION_1_1) || defined(VK_KHR_EXTERNAL_FENCE_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( export_fence ) 
#endif
#ifdef VK_KHR_EXTERNAL_FENCE_WIN32_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( export_fence_win32_handle ) 
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

