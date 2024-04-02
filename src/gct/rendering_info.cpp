#include <vulkan/vulkan.hpp>
#if defined(VK_VERSION_1_3) || defined(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME)
#include <nlohmann/json.hpp>
#ifdef VK_VERSION_1_3
#include <vulkan2json/RenderingInfo.hpp>
#else
#include <vulkan2json/RenderingInfoKHR.hpp>
#endif
#ifdef VK_VERSION_1_1
#include <vulkan2json/DeviceGroupRenderPassBeginInfo.hpp>
#include <vulkan2json/Rect2D.hpp>
#elif defined(VK_KHR_DEVICE_GROUP_EXTENSION_NAME)
#include <vulkan2json/DeviceGroupRenderPassBeginInfoKHR.hpp>
#include <vulkan2json/Rect2D.hpp>
#endif
#ifdef VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_EXTENSION_NAME
#include <vulkan2json/MultisampledRenderToSingleSampledInfoEXT.hpp>
#endif
#ifdef VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES_EXTENSION_NAME
#include <vulkan2json/MultiviewPerViewAttributesInfoNVX.hpp>
#endif
#ifdef VK_QCOM_MULTIVIEW_PER_VIEW_RENDER_AREAS_EXTENSION_NAME
#include <vulkan2json/MultiviewPerViewRenderAreasRenderPassBeginInfoQCOM.hpp>
#include <vulkan2json/Rect2D.hpp>
#endif
#ifdef VK_ARM_RENDER_PASS_STRIPED_EXTENSION_NAME
#include <vulkan2json/RenderPassStripeBeginInfoARM.hpp>
#include <vulkan2json/RenderPassStripeInfoARM.hpp>
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
#include <vulkan2json/RenderingFragmentDensityMapAttachmentInfoEXT.hpp>
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
#include <vulkan2json/RenderingFragmentShadingRateAttachmentInfoKHR.hpp>
#endif
#include <gct/rendering_info.hpp>

namespace gct {
  void to_json( nlohmann::json &root, const rendering_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_TO_JSON( device_group )
    LIBGCT_ARRAY_OF_TO_JSON( device_group, pDeviceRenderAreas, device_render_area )
#elif defined(VK_KHR_DEVICE_GROUP_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( device_group )
    LIBGCT_ARRAY_OF_TO_JSON( device_group, pDeviceRenderAreas, device_render_area )
#endif
#ifdef VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( multisampled_render_to_single_sampled )
#endif
#ifdef VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( multiview_per_view_attributes )
#endif
#ifdef VK_QCOM_MULTIVIEW_PER_VIEW_RENDER_AREAS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( multiview_per_view_render_areas )
    LIBGCT_ARRAY_OF_TO_JSON( multiview_per_view_render_areas, pPerViewRenderAreas, per_view_render_area )
#endif
#ifdef VK_ARM_RENDER_PASS_STRIPED_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( stripe )
    LIBGCT_ARRAY_OF_TO_JSON( stripe, pStripeInfos, stripe_info )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_density_map_attachment )
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_shading_rate_attachment )
#endif
  }
  void from_json( const nlohmann::json &root, rendering_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to rendering_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_FROM_JSON( device_group )
    LIBGCT_ARRAY_OF_FROM_JSON( device_group, pDeviceRenderAreas, device_render_area )
#elif defined(VK_KHR_DEVICE_GROUP_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( device_group )
    LIBGCT_ARRAY_OF_FROM_JSON( device_group, pDeviceRenderAreas, device_render_area )
#endif
#ifdef VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( multisampled_render_to_single_sampled )
#endif
#ifdef VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( multiview_per_view_attributes )
#endif
#ifdef VK_QCOM_MULTIVIEW_PER_VIEW_RENDER_AREAS_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( multiview_per_view_render_areas )
    LIBGCT_ARRAY_OF_FROM_JSON( multiview_per_view_render_areas, pPerViewRenderAreas, per_view_render_area )
#endif
#ifdef VK_ARM_RENDER_PASS_STRIPED_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( stripe )
    LIBGCT_ARRAY_OF_FROM_JSON( stripe, pStripeInfos, stripe_info )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( fragment_density_map_attachment )
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( fragment_shading_rate_attachment )
#endif
  }
  rendering_info_t &rendering_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_VERSION_1_1
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( device_group, DeviceRenderAreaCount, PDeviceRenderAreas, device_render_area )
    LIBGCT_EXTENSION_REBUILD_CHAIN( device_group )
#elif defined(VK_KHR_DEVICE_GROUP_EXTENSION_NAME)
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( device_group, DeviceRenderAreaCount, PDeviceRenderAreas, device_render_area )
    LIBGCT_EXTENSION_REBUILD_CHAIN( device_group )
#endif
#ifdef VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( multisampled_render_to_single_sampled )
#endif
#ifdef VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( multiview_per_view_attributes )
#endif
#ifdef VK_QCOM_MULTIVIEW_PER_VIEW_RENDER_AREAS_EXTENSION_NAME
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( multiview_per_view_render_areas, PerViewRenderAreaCount, PPerViewRenderAreas, per_view_render_area )
    LIBGCT_EXTENSION_REBUILD_CHAIN( multiview_per_view_render_areas )
#endif
#ifdef VK_ARM_RENDER_PASS_STRIPED_EXTENSION_NAME
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( stripe, StripeInfoCount, PStripeInfos, stripe_info )
    LIBGCT_EXTENSION_REBUILD_CHAIN( stripe )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_density_map_attachment )
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_shading_rate_attachment )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}
#endif


