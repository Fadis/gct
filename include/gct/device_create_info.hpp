#ifndef GCT_DEVICE_CREATE_INFO_H
#define GCT_DEVICE_CREATE_INFO_H

#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>

namespace gct {
  struct device_create_info_t : public chained_t {
    using self_type = device_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::DeviceCreateInfo )
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::DeviceGroupDeviceCreateInfo , device_group )
#elif defined(VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::DeviceGroupDeviceCreateInfoKHR , device_group )
#endif
#ifdef VK_EXT_DEVICE_MEMORY_REPORT_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::DeviceDeviceMemoryReportCreateInfoEXT , device_memory_reporting )
#endif
#ifdef VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::DeviceDiagnosticsConfigCreateInfoNV , diagnostic_config )
#endif
#ifdef VK_AMD_MEMORY_OVERALLOCATION_BEHAVIOR_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::DeviceMemoryOverallocationCreateInfoAMD , memory_over_allocation )
#endif
#ifdef VK_EXT_PRIVATE_DATA_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::DevicePrivateDataCreateInfoEXT , private_data )
#endif
  };
  void to_json( nlohmann::json &root, const device_create_info_t &v );
  void from_json( const nlohmann::json &root, device_create_info_t &v );
}

#endif

