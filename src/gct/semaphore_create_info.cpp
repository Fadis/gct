#include <gct/semaphore_create_info.hpp>

namespace gct {
  semaphore_create_info_t &semaphore_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#if defined(VK_VERSION_1_1) || defined(VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( export_semaphore ) 
#endif
#ifdef VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( export_semaphore_win32_handle ) 
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( type ) 
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

