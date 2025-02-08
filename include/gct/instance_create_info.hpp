#ifndef LIBGCT_INCLUDE_GCT_INSTANCE_CREATE_INFO_HPP
#define LIBGCT_INCLUDE_GCT_INSTANCE_CREATE_INFO_HPP

#include <memory>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>
#include <gct/array_of.hpp>

namespace gct {
  class instance_create_info_t : public chained_t {
  public:
    using self_type = instance_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::InstanceCreateInfo )
#ifdef VK_EXT_DEBUG_REPORT_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::DebugReportCallbackCreateInfoEXT, debug_report )
#endif
#ifdef VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::DebugUtilsMessengerCreateInfoEXT, debug_utils_messenger )
#endif
#ifdef VK_LUNARG_DIRECT_DRIVER_LOADING_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::DirectDriverLoadingListLUNARG, direct_driver_loading )
#endif
#ifdef VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::ValidationFeaturesEXT, validation_features )
#endif
#ifdef VK_EXT_VALIDATION_FLAGS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::ValidationFlagsEXT, validation_flags )
#endif
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::ExportMetalObjectCreateInfoEXT, metal_object )
#endif
#ifdef VK_EXT_LAYER_SETTINGS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::LayerSettingsCreateInfoEXT, layer_settings )
    LIBGCT_ARRAY_OF( vk::LayerSettingEXT, layer_setting )
#endif
    LIBGCT_ARRAY_OF_SMALL( const char *, extension )
    LIBGCT_ARRAY_OF_SMALL( const char *, layer )
#ifdef VK_LUNARG_DIRECT_DRIVER_LOADING_EXTENSION_NAME
    LIBGCT_ARRAY_OF( vk::DirectDriverLoadingInfoLUNARG, direct_driver )
#endif
#ifdef VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME
    LIBGCT_ARRAY_OF( vk::ValidationFeatureEnableEXT, validation_feature_enable )
    LIBGCT_ARRAY_OF( vk::ValidationFeatureDisableEXT, validation_feature_disable )
#endif
#ifdef VK_EXT_VALIDATION_FLAGS_EXTENSION_NAME
    LIBGCT_ARRAY_OF( vk::ValidationCheckEXT, validation_check )
#endif
  private:
    deep_copy_unique_ptr< vk::ApplicationInfo > application_info;
  public:
    instance_create_info_t &set_application_info( const vk::ApplicationInfo &v );
    instance_create_info_t &clear_pplication_info();
    [[nodiscard]] bool has_application_info() const { return application_info.get(); }
    [[nodiscard]] const vk::ApplicationInfo &get_application_info() const { return *application_info; }
  };
  void to_json( nlohmann::json&, const instance_create_info_t& );
  void from_json( const nlohmann::json&, instance_create_info_t& );
}

#endif

