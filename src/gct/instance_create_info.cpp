#include <gct/instance_create_info.hpp>
#include <memory>
#include <nlohmann/json.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/get_extensions.hpp>
#include <vulkan2json/InstanceCreateInfo.hpp>
#include <vulkan2json/ApplicationInfo.hpp>
#ifdef VK_EXT_DEBUG_REPORT_EXTENSION_NAME
#include <vulkan2json/DebugReportCallbackCreateInfoEXT.hpp>
#endif
#ifdef VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#include <vulkan2json/DebugUtilsMessengerCreateInfoEXT.hpp>
#endif
#ifdef VK_LUNARG_DIRECT_DRIVER_LOADING_EXTENSION_NAME
#include <vulkan2json/DirectDriverLoadingListLUNARG.hpp>
#include <vulkan2json/DirectDriverLoadingInfoLUNARG.hpp>
#endif
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
#include <vulkan2json/ExportMetalObjectCreateInfoEXT.hpp>
#endif
#ifdef VK_EXT_LAYER_SETTINGS_EXTENSION_NAME
#include <vulkan2json/LayerSettingsCreateInfoEXT.hpp>
#include <vulkan2json/LayerSettingEXT.hpp>
#endif
#ifdef VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME
#include <vulkan2json/ValidationFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_VALIDATION_FLAGS_EXTENSION_NAME
#include <vulkan2json/ValidationFlagsEXT.hpp>
#endif

namespace gct {
  instance_create_info_t &instance_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    if( application_info ) {
      application_info->pEngineName = "gct";
      application_info->engineVersion = VK_MAKE_VERSION( 1, 0, 0 );
      basic.setPApplicationInfo( application_info.get() );
    }
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( basic, EnabledLayerCount, PpEnabledLayerNames, layer )
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( basic, EnabledExtensionCount, PpEnabledExtensionNames, extension )
#ifdef VK_LUNARG_DIRECT_DRIVER_LOADING_EXTENSION_NAME
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( direct_driver_loading, DriverCount, PDrivers, direct_driver )
#endif
#ifdef VK_EXT_LAYER_SETTINGS_EXTENSION_NAME
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( layer_settings, SettingCount, PSettings, layer_setting )
#endif
#ifdef VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( validation_features, EnabledValidationFeatureCount, PEnabledValidationFeatures, validation_feature_enable )
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( validation_features, DisabledValidationFeatureCount, PDisabledValidationFeatures, validation_feature_disable )
#endif
#ifdef VK_EXT_VALIDATION_FLAGS_EXTENSION_NAME
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( validation_flags, DisabledValidationCheckCount, PDisabledValidationChecks, validation_check );
#endif
#ifdef VK_FUCHSIA_BUFFER_COLLECTION_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( buffer_collection )
#endif
#ifdef VK_EXT_DEBUG_REPORT_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( debug_report )
#endif
#ifdef VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( debug_utils_messenger )
#endif
#ifdef VK_LUNARG_DIRECT_DRIVER_LOADING_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( direct_driver_loading )
#endif
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
    LIBGCT_EXTENSION_REUBILD_CHAIN( metal_object )
#endif
#ifdef VK_EXT_LAYER_SETTINGS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( layer_settings )
#endif
#ifdef VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( validation_features )
#endif
#ifdef VK_EXT_VALIDATION_FLAGS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( validation_flags )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  instance_create_info_t &instance_create_info_t::set_application_info( const vk::ApplicationInfo &v ) {
    application_info.reset( new vk::ApplicationInfo( v ) );
    chained = false;
    return *this;
  }
  instance_create_info_t &instance_create_info_t::clear_pplication_info() {
    application_info.reset();
    chained = false;
    return *this;
  }
  void to_json( nlohmann::json &root, const instance_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
    if( v.has_application_info() )
      root[ "basic" ][ "pApplicationInfo" ] = v.get_application_info();
    LIBGCT_ARRAY_OF_TO_JSON( basic, ppEnabledLayerNames, layer );
    LIBGCT_ARRAY_OF_TO_JSON( basic, ppEnabledExtensionNames, extension );
#ifdef VK_EXT_DEBUG_REPORT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( debug_report )
#endif
#ifdef VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( debug_utils_messenger )
#endif
#ifdef VK_LUNARG_DIRECT_DRIVER_LOADING_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( direct_driver_loading )
    LIBGCT_ARRAY_OF_TO_JSON( direct_driver_loading, pDrivers, direct_driver );
#endif
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( metal_object )
#endif
#ifdef VK_EXT_LAYER_SETTINGS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( layer_settings )
    LIBGCT_ARRAY_OF_TO_JSON( layer_settings, pSettings, layer_setting )
#endif
#ifdef VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( validation_features )
    LIBGCT_ARRAY_OF_TO_JSON( validation_features, pEnabledValidationFeatures, validation_feature_enable );
    LIBGCT_ARRAY_OF_TO_JSON( validation_features, pDisabledValidationFeatures, validation_feature_disable );
#endif
#ifdef VK_EXT_VALIDATION_FLAGS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( validation_flags )
    LIBGCT_ARRAY_OF_TO_JSON( validation_flags, pEnabledValidationFeatures, validation_check );
#endif
  }
}


