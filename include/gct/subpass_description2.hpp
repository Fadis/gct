#ifndef GCT_SUBPASS_DESCRIPTION2_HPP
#define GCT_SUBPASS_DESCRIPTION2_HPP
#include <memory>
#include <vector>
#include <cstdint>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>
#include <gct/array_of.hpp>
#include <gct/attachment_reference2.hpp>

namespace gct {
#if defined(VK_VERSION_1_2) || defined(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME)
  class subpass_description2_t : public chained_t {
  public:
    using self_type = subpass_description2_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
#ifdef VK_VERSION_1_2
    LIBGCT_BASIC_SETTER( vk::SubpassDescription2 )
    LIBGCT_ARRAY_OF_WRAPPED( attachment_reference2_t, vk::AttachmentReference2, input_attachment )
    LIBGCT_ARRAY_OF_WRAPPED( attachment_reference2_t, vk::AttachmentReference2, color_attachment )
    LIBGCT_ARRAY_OF_SMALL( std::uint32_t, preserve_attachment )
#else
    LIBGCT_BASIC_SETTER( vk::SubpassDescription2KHR )
    LIBGCT_ARRAY_OF_WRAPPED( attachment_reference2_t, vk::AttachmentReference2KHR, input_attachment )
    LIBGCT_ARRAY_OF_WRAPPED( attachment_reference2_t, vk::AttachmentReference2KHR, color_attachment )
    LIBGCT_ARRAY_OF_SMALL( std::uint32_t, preserve_attachment )
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::FragmentShadingRateAttachmentInfoKHR , fragment_shading_rate )
  private:
    deep_copy_unique_ptr< attachment_reference2_t > fragment_shading_rate_attachment;
  public:
    subpass_description2_t &set_fragment_shading_rate_attachment( uint32_t, vk::ImageLayout );
    subpass_description2_t &set_fragment_shading_rate_attachment( const attachment_reference2_t& );
    subpass_description2_t &clear_fragment_shading_rate_attachment();
#endif
#ifdef VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::MultisampledRenderToSingleSampledInfoEXT , multisample )
#endif
#ifdef VK_EXT_SUBPASS_MERGE_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::RenderPassCreationControlEXT , creation_control )
    LIBGCT_EXTENSION_SETTER( vk::RenderPassSubpassFeedbackCreateInfoEXT , feedback )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::SubpassDescriptionDepthStencilResolve , depth_stencil_resolve )
#elif defined(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::SubpassDescriptionDepthStencilResolveKHR , depth_stencil_resolve )
#endif
  private:
    deep_copy_unique_ptr< attachment_reference2_t > resolve_attachment;
    deep_copy_unique_ptr< attachment_reference2_t > depth_stencil_attachment;
  public:
    subpass_description2_t &add_input_attachment( uint32_t, vk::ImageLayout );
    subpass_description2_t &add_color_attachment( uint32_t, vk::ImageLayout );
    subpass_description2_t &set_resolve_attachment( uint32_t, vk::ImageLayout );
    subpass_description2_t &set_resolve_attachment( const attachment_reference2_t& );
    subpass_description2_t &clear_resolve_attachment();
    subpass_description2_t &set_depth_stencil_attachment( uint32_t, vk::ImageLayout );
    subpass_description2_t &set_depth_stencil_attachment( const attachment_reference2_t& );
    subpass_description2_t &clear_depth_stencil_attachment();
  };
  void to_json( nlohmann::json &root, const subpass_description2_t &v );
  void from_json( const nlohmann::json &root, subpass_description2_t &v );
}
#endif

#endif

