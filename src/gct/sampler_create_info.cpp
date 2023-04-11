#include <fstream>
#include <iterator>
#include <gct/sampler_create_info.hpp>
#include <vulkan2json/SamplerCreateInfo.hpp>
#ifdef VK_EXT_BORDER_COLOR_SWIZZLE_EXTENSION_NAME
#include <vulkan2json/SamplerBorderColorComponentMappingCreateInfoEXT.hpp>
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
#include <vulkan2json/SamplerCustomBorderColorCreateInfoEXT.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/SamplerReductionModeCreateInfo.hpp>
#elif defined(VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME)
#include <vulkan2json/SamplerReductionModeCreateInfoEXT.hpp>
#endif
#ifdef VK_VERSION_1_1
#include <vulkan2json/SamplerYcbcrConversionInfo.hpp>
#elif defined(VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME)
#include <vulkan2json/SamplerYcbcrConversionInfoKHR.hpp>
#endif

namespace gct {
  void to_json( nlohmann::json &root, const sampler_create_info_t &v ) {
     root = nlohmann::json::object();
     root[ "basic" ] = v.get_basic();
#ifdef VK_EXT_BORDER_COLOR_SWIZZLE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( border_color_component_mapping ) 
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( custom_border_color ) 
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( reduction_mode ) 
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( ycbcr_conversion )
#endif
  }
  void from_json( const nlohmann::json &root, sampler_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to sampler_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_EXT_BORDER_COLOR_SWIZZLE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( border_color_component_mapping ) 
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( custom_border_color ) 
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( reduction_mode ) 
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( ycbcr_conversion )
#endif
  }
  sampler_create_info_t &sampler_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_EXT_BORDER_COLOR_SWIZZLE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( border_color_component_mapping ) 
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( custom_border_color ) 
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( reduction_mode ) 
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( ycbcr_conversion )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  const sampler_create_info_t &get_basic_linear_sampler_create_info() {
    static const sampler_create_info_t instance =
      sampler_create_info_t()
        .set_basic(
          vk::SamplerCreateInfo()
            .setMagFilter( vk::Filter::eLinear )
            .setMinFilter( vk::Filter::eLinear )
            .setMipmapMode( vk::SamplerMipmapMode::eLinear )
            .setAddressModeU( vk::SamplerAddressMode::eClampToEdge )
            .setAddressModeV( vk::SamplerAddressMode::eClampToEdge )
            .setAddressModeW( vk::SamplerAddressMode::eClampToEdge )
            .setAnisotropyEnable( false )
            .setCompareEnable( false )
            .setMipLodBias( 0.f )
            .setMinLod( 0.f )
            .setMaxLod( 0.f )
            .setBorderColor( vk::BorderColor::eFloatTransparentBlack )
            .setUnnormalizedCoordinates( false )
        );
    return instance;
  }
}

