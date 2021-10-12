#include <gct/device_create_info.hpp>

namespace gct {
  device_create_info_t &device_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#if defined(VK_VERSION_1_1) || defined(VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( device_group ) 
#endif
#ifdef VK_EXT_DEVICE_MEMORY_REPORT_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( device_memory_reporting ) 
#endif
#ifdef VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( diagnostic_config ) 
#endif
#ifdef VK_AMD_MEMORY_OVERALLOCATION_BEHAVIOR_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( memory_over_allocation ) 
#endif
#ifdef VK_EXT_PRIVATE_DATA_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( private_data ) 
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
    return *this;
  }
}

