#include <nlohmann/json.hpp>
#include <gct/device_create_info.hpp>
#include <vulkan2json/DeviceCreateInfo.hpp>
#ifdef VK_VERSION_1_1
#include <vulkan2json/DeviceGroupDeviceCreateInfo.hpp>
#elif defined(VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME)
#include <vulkan2json/DeviceGroupDeviceCreateInfoKHR.hpp>
#endif
#ifdef VK_EXT_DEVICE_MEMORY_REPORT_EXTENSION_NAME
#include <vulkan2json/DeviceDeviceMemoryReportCreateInfoEXT.hpp>
#endif
#ifdef VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME
#include <vulkan2json/DeviceDiagnosticsConfigCreateInfoNV.hpp>
#endif
#ifdef VK_AMD_MEMORY_OVERALLOCATION_BEHAVIOR_EXTENSION_NAME
#include <vulkan2json/DeviceMemoryOverallocationCreateInfoAMD.hpp>
#endif
#ifdef VK_EXT_PRIVATE_DATA_EXTENSION_NAME
#include <vulkan2json/DevicePrivateDataCreateInfoEXT.hpp>
#endif
#ifdef VK_ARM_SCHEDULING_CONTROLS_EXTENSION_NAME
#include <vulkan2json/DeviceQueueShaderCoreControlCreateInfoARM.hpp>
#endif

namespace gct {
  void to_json( nlohmann::json &root, const device_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
#if defined(VK_VERSION_1_1) || defined(VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( device_group )
#endif
#ifdef VK_EXT_DEVICE_MEMORY_REPORT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( device_memory_reporting ) 
#endif
#ifdef VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( diagnostic_config ) 
#endif
#ifdef VK_AMD_MEMORY_OVERALLOCATION_BEHAVIOR_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( memory_over_allocation ) 
#endif
#ifdef VK_EXT_PRIVATE_DATA_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( private_data ) 
#endif
#ifdef VK_ARM_SCHEDULING_CONTROLS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( queue_shader_core_control )
#endif
  }
  void from_json( const nlohmann::json &root, device_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to device_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#if defined(VK_VERSION_1_1) || defined(VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( device_group )
#endif
#ifdef VK_EXT_DEVICE_MEMORY_REPORT_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( device_memory_reporting ) 
#endif
#ifdef VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( diagnostic_config ) 
#endif
#ifdef VK_AMD_MEMORY_OVERALLOCATION_BEHAVIOR_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( memory_over_allocation ) 
#endif
#ifdef VK_EXT_PRIVATE_DATA_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( private_data ) 
#endif
#ifdef VK_ARM_SCHEDULING_CONTROLS_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( queue_shader_core_control )
#endif
  }
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
#ifdef VK_ARM_SCHEDULING_CONTROLS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( queue_shader_core_control )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
    return *this;
  }
}

