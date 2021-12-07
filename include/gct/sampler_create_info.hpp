#ifndef GCT_SAMPLER_CREATE_INFO_HPP
#define GCT_SAMPLER_CREATE_INFO_HPP
#include <memory>
#include <optional>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class sampler_create_info_t {
  public:
    using self_type = sampler_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::SamplerCreateInfo )
#ifdef VK_EXT_BORDER_COLOR_SWIZZLE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::SamplerBorderColorComponentMappingCreateInfoEXT , border_color_component_mapping )
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::SamplerCustomBorderColorCreateInfoEXT , custom_border_color )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::SamplerReductionModeCreateInfo , reduction_mode )
#elif defined(VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::SamplerReductionModeCreateInfoEXT , reduction_mode )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::SamplerYcbcrConversionInfo , ycbcr_conversion )
#elif defined(VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::SamplerYcbcrConversionInfoKHR , ycbcr_conversion )
#endif
  };
  void to_json( nlohmann::json &root, const sampler_create_info_t &v );
  void from_json( const nlohmann::json &root, sampler_create_info_t &v );
}

#endif

