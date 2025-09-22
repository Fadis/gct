#ifndef GCT_RENDERING_INFO_HPP
#define GCT_RENDERING_INFO_HPP
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>
#include <gct/array_of.hpp>
#include <gct/rendering_attachment_info.hpp>

#if defined(VK_VERSION_1_3) || defined(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME)
namespace gct {
  class rendering_info_t : public chained_t {
  public:
    using self_type = rendering_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
#ifdef VK_VERSION_1_3
    LIBGCT_BASIC_SETTER( vk::RenderingInfo )
#else
    LIBGCT_BASIC_SETTER( vk::RenderingInfoKHR )
#endif
    LIBGCT_ARRAY_OF_WRAPPED( rendering_attachment_info_t, vk::RenderingAttachmentInfo, color_attachment )
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::DeviceGroupRenderPassBeginInfo, device_group )
    LIBGCT_ARRAY_OF( vk::Rect2D, device_render_area )
#elif defined(VK_KHR_DEVICE_GROUP_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::DeviceGroupRenderPassBeginInfoKHR, device_group )
    LIBGCT_ARRAY_OF( vk::Rect2D, device_render_area )
#endif
#ifdef VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::MultisampledRenderToSingleSampledInfoEXT, multisampled_render_to_single_sampled )
#endif
#ifdef VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::MultiviewPerViewAttributesInfoNVX, multiview_per_view_attributes )
#endif
#ifdef VK_QCOM_MULTIVIEW_PER_VIEW_RENDER_AREAS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::MultiviewPerViewRenderAreasRenderPassBeginInfoQCOM, multiview_per_view_render_areas )
    LIBGCT_ARRAY_OF( vk::Rect2D, per_view_render_area )
#endif
#ifdef VK_ARM_RENDER_PASS_STRIPED_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::RenderPassStripeBeginInfoARM, stripe )
    LIBGCT_ARRAY_OF( vk::RenderPassStripeInfoARM, stripe_info )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::RenderingFragmentDensityMapAttachmentInfoEXT, fragment_density_map_attachment )
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::RenderingFragmentShadingRateAttachmentInfoKHR, fragment_shading_rate_attachment )
#endif
    rendering_info_t &set_depth_attachment( const std::shared_ptr< rendering_attachment_info_t > &v ) {
      depth_attachment = v;
      chained = false;
      return *this;
    }
    rendering_info_t &set_depth_attachment( const rendering_attachment_info_t &v ) {
      depth_attachment.reset( new rendering_attachment_info_t( v ) );
      chained = false;
      return *this;
    }
    rendering_info_t &clear_depth_attachment() {
      depth_attachment.reset();
      chained = false;
      return *this;
    }
    const std::shared_ptr< rendering_attachment_info_t > &get_depth_attachment() const {
      return depth_attachment;
    }
    rendering_info_t &set_stencil_attachment( const std::shared_ptr< rendering_attachment_info_t > &v ) {
      stencil_attachment = v;
      chained = false;
      return *this;
    }
    rendering_info_t &set_stencil_attachment( const rendering_attachment_info_t &v ) {
      stencil_attachment.reset( new rendering_attachment_info_t( v ) );
      chained = false;
      return *this;
    }
    rendering_info_t &clear_stencil_attachment() {
      stencil_attachment.reset();
      chained = false;
      return *this;
    }
    const std::shared_ptr< rendering_attachment_info_t > &get_stencil_attachment() const {
      return stencil_attachment;
    }
  private:
    std::shared_ptr< rendering_attachment_info_t > depth_attachment;
    std::shared_ptr< rendering_attachment_info_t > stencil_attachment;
    bool extent_generated = false;
  };
  void to_json( nlohmann::json &root, const rendering_info_t &v );
  void from_json( const nlohmann::json &root, rendering_info_t &v );
}
#endif

#endif

