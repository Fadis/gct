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
#ifdef VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME
#include <vulkan2json/ValidationFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_VALIDATION_FLAGS_EXTENSION_NAME
#include <vulkan2json/ValidationFlagsEXT.hpp>
#endif

namespace gct {
  instance_create_info_t &instance_create_info_t::rebuild_chain() {
    if( chained ) return *this;
    auto basic = get_basic();
    if( application_info ) {
      application_info->pEngineName = "gct";
      application_info->engineVersion = VK_MAKE_VERSION( 1, 0, 0 );
      basic.setPApplicationInfo( application_info.get() );
    }
    if( !layer.empty() )
      basic
        .setEnabledLayerCount( layer.size() )
        .setPpEnabledLayerNames( layer.data() );
    if( !extension.empty() )
      basic
        .setEnabledExtensionCount( extension.size() )
        .setPpEnabledExtensionNames( extension.data() );
    set_basic( std::move( basic ) );
#ifdef VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME
    {
      auto validation_features = vk::ValidationFeaturesEXT();
      if( has_validation_features() )
        validation_features = get_validation_features();
      bool modified = false;
      if( !validation_feature_enable.empty() ) {
        validation_features
          .setEnabledValidationFeatureCount( validation_feature_enable.size() )
          .setPEnabledValidationFeatures( validation_feature_enable.data() );
        modified = true;
      }
      if( !validation_feature_disable.empty() ) {
        validation_features
          .setDisabledValidationFeatureCount( validation_feature_disable.size() )
          .setPDisabledValidationFeatures( validation_feature_disable.data() );
        modified = true;
      }
      if( modified )
        set_validation_features( std::move( validation_features ) );
    }
#endif
#ifdef VK_EXT_VALIDATION_FLAGS_EXTENSION_NAME
    if( !validation_check.empty() ) {
      set_validation_flags(
        vk::ValidationFlagsEXT()
          .setDisabledValidationCheckCount( validation_check.size() )
          .setPDisabledValidationChecks( validation_check.data() )
      );
    }
#endif
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_FUCHSIA_BUFFER_COLLECTION_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( buffer_collection )
#endif
#ifdef VK_EXT_DEBUG_REPORT_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( debug_report )
#endif
#ifdef VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( debug_utils_messenger )
#endif
#ifdef VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( validation_features )
#endif
#ifdef VK_EXT_VALIDATION_FLAGS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( validation_flags )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  instance_create_info_t &instance_create_info_t::add_extension( const char *v ) {
    extension.push_back( v );
    chained = false;
    return *this;
  }
  instance_create_info_t &instance_create_info_t::clear_extension() {
    extension.clear();
    chained = false;
    return *this;
  }
  instance_create_info_t &instance_create_info_t::add_layer( const char *v ) {
    layer.push_back( v );
    chained = false;
    return *this;
  }
  instance_create_info_t &instance_create_info_t::clear_layer() {
    layer.clear();
    chained = false;
    return *this;
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
#ifdef VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME
  instance_create_info_t &instance_create_info_t::add_validation_feature_enable( const vk::ValidationFeatureEnableEXT &v ) {
    validation_feature_enable.push_back( v );
    chained = false;
    return *this;
  }
  instance_create_info_t &instance_create_info_t::clear_validation_feature_enable() {
    validation_feature_enable.clear();
    chained = false;
    return *this;
  }
  instance_create_info_t &instance_create_info_t::add_validation_feature_disable( const vk::ValidationFeatureDisableEXT &v ) {
    validation_feature_disable.push_back( v );
    chained = false;
    return *this;
  }
  instance_create_info_t &instance_create_info_t::clear_validation_feature_disable() {
    validation_feature_disable.clear();
    chained = false;
    return *this;
  }
#endif
#ifdef VK_EXT_VALIDATION_FLAGS_EXTENSION_NAME
  instance_create_info_t &instance_create_info_t::add_validation_check( const vk::ValidationCheckEXT &v ) {
    validation_check.push_back( v );
    chained = false;
    return *this;
  }
  instance_create_info_t &instance_create_info_t::clear_validation_check() {
    validation_check.clear();
    chained = false;
    return *this;
  }
#endif
  void to_json( nlohmann::json &root, const instance_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
    if( v.has_application_info() )
      root[ "basic" ][ "pApplicationInfo" ] = v.get_application_info();
    if( !v.get_layer().empty() )
      root[ "basic" ][ "ppEnabledLayerNames" ] = v.get_layer();
    if( !v.get_extension().empty() )
      root[ "basic" ][ "ppEnabledExtensionNames" ] = v.get_extension();
#ifdef VK_EXT_DEBUG_REPORT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( debug_report )
#endif
#ifdef VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( debug_utils_messenger )
#endif
#ifdef VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( validation_features )
    if( root.find( "validation_features" ) == root.end() ) {
      root[ "validation_features" ] = nlohmann::json::object();
    }
    if( !v.get_validation_feature_enable().empty() )
      root[ "validation_features" ][ "pEnabledValidationFeatures" ] = v.get_validation_feature_enable();
    if( !v.get_validation_feature_disable().empty() )
      root[ "validation_features" ][ "pDisabledValidationFeatures" ] = v.get_validation_feature_disable();
#endif
#ifdef VK_EXT_VALIDATION_FLAGS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( validation_flags )
    if( root.find( "validation_flags" ) == root.end() ) {
      root[ "validation_flags" ] = nlohmann::json::object();
    }
    if( !v.get_validation_check().empty() )
      root[ "validation_features" ][ "pEnabledValidationFeatures" ] = v.get_validation_check();
#endif
  }
}


