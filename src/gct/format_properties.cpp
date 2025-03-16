#include <nlohmann/json.hpp>
#include <vulkan2json/FormatProperties.hpp>
#if defined(VK_VERSION_1_1) || defined(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)
#include <vulkan2json/FormatProperties2.hpp>
#if defined(VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME) && ( defined(VK_KHR_FORMAT_FEATURE_FLAGS_2_EXTENSION_NAME) || defined(VK_VERSION_1_3) )
#include <vulkan2json/DrmFormatModifierPropertiesList2EXT.hpp>
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
#include <vulkan2json/DrmFormatModifierPropertiesListEXT.hpp>
#endif
#if defined(VK_KHR_FORMAT_FEATURE_FLAGS_2_EXTENSION_NAME) || defined(VK_VERSION_1_3)
#ifdef VK_VERSION_1_3
#include <vulkan2json/FormatProperties3.hpp>
#else
#include <vulkan2json/FormatProperties3KHR.hpp>
#endif
#endif
#ifdef VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_EXTENSION_NAME
#include <vulkan2json/SubpassResolvePerformanceQueryEXT.hpp>
#endif
#endif
#include <gct/instance.hpp>
#include <gct/physical_device_properties.hpp>
#include <gct/format_properties.hpp>

namespace gct {
void to_json( nlohmann::json &root, const format_properties &v ) {
  root = nlohmann::json::object();
  root[ "basic" ] = v.get_basic();
#if defined(VK_VERSION_1_1) || defined(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)
#if defined(VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME) && ( defined(VK_KHR_FORMAT_FEATURE_FLAGS_2_EXTENSION_NAME) || defined(VK_VERSION_1_3) )
    LIBGCT_EXTENSION_TO_JSON( drm_format_modifier2 )
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( drm_format_modifier )
#endif
#if defined(VK_KHR_FORMAT_FEATURE_FLAGS_2_EXTENSION_NAME) || defined(VK_VERSION_1_3)
    LIBGCT_EXTENSION_TO_JSON( format3 )
#endif
#ifdef VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( subpass_resolve_performance_query )
#endif
#endif
}

format_properties::format_properties(
  instance_t &instance,
  vk::PhysicalDevice &pdev,
  const physical_device_properties_t &props,
  vk::Format format
) {
  const auto &ext = props.get_activated_extensions();
#if defined(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME) || defined(VK_VERSION_1_1)
  if(
    instance.get_api_version() >= VK_MAKE_VERSION( 1, 1, 0 ) ||
    instance.get_activated_extensions().find( "VK_KHR_get_physical_device_properties2" ) != instance.get_activated_extensions().end()
  ) {
#if defined(VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME) && ( defined(VK_KHR_FORMAT_FEATURE_FLAGS_2_EXTENSION_NAME) || defined(VK_VERSION_1_3) )
    LIBGCT_EXTENSION_CREATE_IF_EXT( drm_format_modifier2, VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME )
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_CREATE_IF_EXT( drm_format_modifier, VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME )
#endif
#if defined(VK_KHR_FORMAT_FEATURE_FLAGS_2_EXTENSION_NAME) || defined(VK_VERSION_1_3)
    LIBGCT_EXTENSION_CREATE_IF_EXT( format3, VK_KHR_FORMAT_FEATURE_FLAGS_2_EXTENSION_NAME )
#endif
#ifdef VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_EXTENSION_NAME
    LIBGCT_EXTENSION_CREATE_IF_EXT( subpass_resolve_performance_query, VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_EXTENSION_NAME )
#endif
    rebuild_chain();
#ifdef VK_VERSION_1_1
    pdev.getFormatProperties2(
      format,
      &basic
    );
#else
    pdev.getFormatProperties2KHR(
      format,
      &basic
    );
#endif
  }
  else {
    pdev.getFormatProperties(
      format,
      &basic.formatProperties
    );
  }
#else
  {
    pdev.getFormatProperties(
      format,
      &basic
    );
  }
#endif
}

format_properties &format_properties::rebuild_chain() {
#if defined(VK_VERSION_1_1) || defined(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)
  LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#if defined(VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME) && ( defined(VK_KHR_FORMAT_FEATURE_FLAGS_2_EXTENSION_NAME) || defined(VK_VERSION_1_3) )
    LIBGCT_EXTENSION_REBUILD_CHAIN( drm_format_modifier2 )
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( drm_format_modifier )
#endif
#if defined(VK_KHR_FORMAT_FEATURE_FLAGS_2_EXTENSION_NAME) || defined(VK_VERSION_1_3)
    LIBGCT_EXTENSION_REBUILD_CHAIN( format3 )
#endif
#ifdef VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( subpass_resolve_performance_query )
#endif
  LIBGCT_EXTENSION_END_REBUILD_CHAIN
#endif
}

}
