#include <vulkan/vulkan.hpp>
#if defined(VK_VERSION_1_2) || defined(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME)

#include <nlohmann/json.hpp>
#ifdef VK_VERSION_1_2
#include <vulkan2json/SubpassDescription2.hpp>
#else
#include <vulkan2json/SubpassDescription2KHR.hpp>
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
#include <vulkan2json/FragmentShadingRateAttachmentInfoKHR.hpp>
#endif
#ifdef VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_EXTENSION_NAME
#include <vulkan2json/MultisampledRenderToSingleSampledInfoEXT.hpp>
#endif
#ifdef VK_EXT_SUBPASS_MERGE_FEEDBACK_EXTENSION_NAME
#include <vulkan2json/RenderPassCreationControlEXT.hpp>
#include <vulkan2json/RenderPassSubpassFeedbackCreateInfoEXT.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/SubpassDescriptionDepthStencilResolve.hpp>
#elif defined(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME)
#include <vulkan2json/SubpassDescriptionDepthStencilResolveKHR.hpp>
#endif
#include <gct/exception.hpp>
#include <gct/subpass_description2.hpp>

namespace gct {
  void to_json( nlohmann::json &root, const subpass_description2_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_shading_rate )
#endif
#ifdef VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( multisample )
#endif
#ifdef VK_EXT_SUBPASS_MERGE_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( creation_control )
    LIBGCT_EXTENSION_TO_JSON( feedback )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( depth_stencil_resolve )
#endif
  }
  void from_json( const nlohmann::json &root, subpass_description2_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to subpass_description2_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( fragment_shading_rate )
#endif
#ifdef VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( multisample )
#endif
#ifdef VK_EXT_SUBPASS_MERGE_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( creation_control )
    LIBGCT_EXTENSION_FROM_JSON( feedback )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( depth_stencil_resolve )
#endif
  }
  subpass_description2_t &subpass_description2_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_shading_rate )
#endif
#ifdef VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( multisample )
#endif
#ifdef VK_EXT_SUBPASS_MERGE_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( creation_control )
    LIBGCT_EXTENSION_REBUILD_CHAIN( feedback )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( depth_stencil_resolve )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  subpass_description2_t &subpass_description2_t::add_input_attachment( std::uint32_t attachment, vk::ImageLayout layout ) {
    return add_input_attachment(
      attachment_reference2_t()
        .set_basic(
#ifdef VK_VERSION_1_2
          vk::AttachmentReference2()
#else
          vk::AttachmentReference2KHR()
#endif
            .setAttachment( attachment )
            .setLayout( layout )
        )
    );
  }
  subpass_description2_t &subpass_description2_t::add_color_attachment( std::uint32_t attachment, vk::ImageLayout layout ) {
    return add_color_attachment(
      attachment_reference2_t()
        .set_basic(
#ifdef VK_VERSION_1_2
          vk::AttachmentReference2()
#else
          vk::AttachmentReference2KHR()
#endif
            .setAttachment( attachment )
            .setLayout( layout )
        )
    );
  }
  subpass_description2_t &subpass_description2_t::set_resolve_attachment( std::uint32_t attachment, vk::ImageLayout layout ) {
    return set_resolve_attachment(
      attachment_reference2_t()
        .set_basic(
#ifdef VK_VERSION_1_2
          vk::AttachmentReference2()
#else
          vk::AttachmentReference2KHR()
#endif
            .setAttachment( attachment )
            .setLayout( layout )
        )
    );
  }
  subpass_description2_t &subpass_description2_t::set_resolve_attachment( const attachment_reference2_t &v ) {
    resolve_attachment.reset(
      new attachment_reference2_t( v ) 
    );
    chained = false;
    return *this;
  }
  subpass_description2_t &subpass_description2_t::clear_resolve_attachment() {
    resolve_attachment.reset();
    chained = false;
    return *this;
  }
  subpass_description2_t &subpass_description2_t::set_depth_stencil_attachment( std::uint32_t attachment, vk::ImageLayout layout ) {
    return set_depth_stencil_attachment(
      attachment_reference2_t()
        .set_basic(
#ifdef VK_VERSION_1_2
          vk::AttachmentReference2()
#else
          vk::AttachmentReference2KHR()
#endif
            .setAttachment( attachment )
            .setLayout( layout )
      )
    );
  }
  subpass_description2_t &subpass_description2_t::set_depth_stencil_attachment( const attachment_reference2_t &v ) {
    depth_stencil_attachment.reset(
      new attachment_reference2_t( v ) 
    );
    chained = false;
    return *this;
  }
  subpass_description2_t &subpass_description2_t::clear_depth_stencil_attachment() {
    depth_stencil_attachment.reset();
    chained = false;
    return *this;
  }
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
  subpass_description2_t &subpass_description2_t::set_fragment_shading_rate_attachment( uint32_t attachment, vk::ImageLayout layout ) {
    return set_fragment_shading_rate_attachment(
      attachment_reference2_t()
        .set_basic(
#ifdef VK_VERSION_1_2
          vk::AttachmentReference2()
#else
          vk::AttachmentReference2KHR()
#endif
            .setAttachment( attachment )
            .setLayout( layout )
      )
    );
  }
  subpass_description2_t &subpass_description2_t::set_fragment_shading_rate_attachment( const attachment_reference2_t &v ) {
    fragment_shading_rate_attachment.reset(
      new attachment_reference2_t( v ) 
    );
    chained = false;
    return *this;
  }
  subpass_description2_t &subpass_description2_t::clear_fragment_shading_rate_attachment() {
    fragment_shading_rate_attachment.reset();
    chained = false;
    return *this;
  }
#endif
}

#endif

