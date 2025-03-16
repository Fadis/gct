#ifndef GCT_FORMAT_PROPERTIES_HPP
#define GCT_FORMAT_PROPERTIES_HPP
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
class instance_t;
class physical_device_properties_t;
class format_properties : public chained_t {
public:
  format_properties(
    instance_t &instance,
    vk::PhysicalDevice &pdev,
    const physical_device_properties_t &props,
    vk::Format format
  );
  using self_type = format_properties;
  LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
#if defined(VK_VERSION_1_1) || defined(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)
private:
    using basic_t = vk::FormatProperties;
#ifdef VK_VERSION_1_1
    using basic2_t = vk::FormatProperties2;
#else
    using basic2_t = vk::FormatProperties2KHR;
#endif
    basic2_t basic; 
public:
    LIBGCT_BASIC_SETTER_INDIRECT( formatProperties )
#if defined(VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME) && ( defined(VK_KHR_FORMAT_FEATURE_FLAGS_2_EXTENSION_NAME) || defined(VK_VERSION_1_3) )
    LIBGCT_EXTENSION_SETTER( vk::DrmFormatModifierPropertiesList2EXT , drm_format_modifier2 )
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::DrmFormatModifierPropertiesListEXT, drm_format_modifier )
#endif
#if defined(VK_KHR_FORMAT_FEATURE_FLAGS_2_EXTENSION_NAME) || defined(VK_VERSION_1_3)
#ifdef VK_VERSION_1_3
    LIBGCT_EXTENSION_SETTER( vk::FormatProperties3, format3 )
#else
    LIBGCT_EXTENSION_SETTER( vk::FormatProperties3KHR, format3 )
#endif
#endif
#ifdef VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::SubpassResolvePerformanceQueryEXT, subpass_resolve_performance_query )
#endif
#else
    LIBGCT_BASIC_SETTER( vk::FormatProperties )
#endif
  };
  void to_json( nlohmann::json &root, const format_properties &v );
}

#endif


