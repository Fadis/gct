#ifndef LIBGCT_INCLUDE_GCT_INSTANCE_CREATE_INFO_HPP
#define LIBGCT_INCLUDE_GCT_INSTANCE_CREATE_INFO_HPP

#include <memory>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>

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
#ifdef VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::ValidationFeaturesEXT, validation_features )
#endif
#ifdef VK_EXT_VALIDATION_FLAGS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::ValidationFlagsEXT, validation_flags )
#endif
  private:
    deep_copy_unique_ptr< vk::ApplicationInfo > application_info;
    std::vector< const char * > extension;
    std::vector< const char * > layer;
  public:
    instance_create_info_t &set_application_info( const vk::ApplicationInfo &v );
    instance_create_info_t &clear_pplication_info();
    bool has_application_info() const { return application_info.get(); }
    const vk::ApplicationInfo &get_application_info() const { return *application_info; }
    instance_create_info_t &add_extension( const char* );
    template< typename Iterator >
    instance_create_info_t &add_extension( Iterator begin, Iterator end ) {
      extension.assign( begin, end );
      chained = true;
      return *this;
    }
    instance_create_info_t &clear_extension();
    instance_create_info_t &add_layer( const char* );
    template< typename Iterator >
    instance_create_info_t &add_layer( Iterator begin, Iterator end ) {
      layer.assign( begin, end );
      chained = true;
      return *this;
    }
    instance_create_info_t &clear_layer();
    const std::vector< const char * > &get_extension() const { return extension; }
    const std::vector< const char * > &get_layer() const { return layer; }
#ifdef VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME
  private:
    std::vector< vk::ValidationFeatureEnableEXT > validation_feature_enable;
    std::vector< vk::ValidationFeatureDisableEXT > validation_feature_disable;
  public:
    instance_create_info_t &add_validation_feature_enable( const vk::ValidationFeatureEnableEXT &v );
    instance_create_info_t &clear_validation_feature_enable();
    instance_create_info_t &add_validation_feature_disable( const vk::ValidationFeatureDisableEXT &v );
    instance_create_info_t &clear_validation_feature_disable();
    const std::vector< vk::ValidationFeatureEnableEXT > &get_validation_feature_enable() const { return validation_feature_enable; };
    const std::vector< vk::ValidationFeatureDisableEXT > &get_validation_feature_disable() const { return validation_feature_disable; };
#endif
#ifdef VK_EXT_VALIDATION_FLAGS_EXTENSION_NAME
  private:
    std::vector< vk::ValidationCheckEXT > validation_check;
  public:
    instance_create_info_t &add_validation_check( const vk::ValidationCheckEXT &v );
    instance_create_info_t &clear_validation_check();
    const std::vector< vk::ValidationCheckEXT > &get_validation_check() const { return validation_check; };
#endif
  };
  void to_json( nlohmann::json&, const instance_create_info_t& );
  void from_json( const nlohmann::json&, instance_create_info_t& );
}

#endif

