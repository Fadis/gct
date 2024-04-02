#include <nlohmann/json.hpp>
#include <gct/surface.hpp>
#include <gct/render_pass_create_info.hpp>
#include <vulkan2json/RenderPassCreateInfo.hpp>
#include <vulkan2json/AttachmentDescription.hpp>
#include <vulkan2json/SubpassDependency.hpp>
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
#include <vulkan2json/RenderPassFragmentDensityMapCreateInfoEXT.hpp>
#endif
#ifdef VK_VERSION_1_1
#include <vulkan2json/RenderPassInputAttachmentAspectCreateInfo.hpp>
#include <vulkan2json/InputAttachmentAspectReference.hpp>
#elif defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
#include <vulkan2json/RenderPassInputAttachmentAspectCreateInfoKHR.hpp>
#include <vulkan2json/InputAttachmentAspectReferenceKHR.hpp>
#endif
#ifdef VK_VERSION_1_1
#include <vulkan2json/RenderPassMultiviewCreateInfo.hpp>
#elif defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
#include <vulkan2json/RenderPassMultiviewCreateInfoKHR.hpp>
#endif

namespace gct {
  void to_json( nlohmann::json &root, const render_pass_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
    LIBGCT_ARRAY_OF_TO_JSON( basic, pAttachments, attachment )
    LIBGCT_ARRAY_OF_TO_JSON_WRAPPED( basic, pSubpasses, subpass )
    LIBGCT_ARRAY_OF_TO_JSON( basic, pDependencies, dependency )
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_density_map )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( input_attachment_aspect )
    LIBGCT_ARRAY_OF_TO_JSON( input_attachment_aspect, pAspectReferences, aspect_reference )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( multiview )
    LIBGCT_ARRAY_OF_TO_JSON( multiview, pViewMasks, view_mask )
    LIBGCT_ARRAY_OF_TO_JSON( multiview, pViewOffsets, view_offset )
    LIBGCT_ARRAY_OF_TO_JSON( multiview, pCorrelationMasks, correlation_mask )
#endif
  }
  void from_json( const nlohmann::json &root, render_pass_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to render_pass_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
    LIBGCT_ARRAY_OF_FROM_JSON( basic, pAttachments, attachment )
    LIBGCT_ARRAY_OF_FROM_JSON_WRAPPED( basic, pSubpasses, subpass )
    LIBGCT_ARRAY_OF_FROM_JSON( basic, pDependencies, dependency )
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( fragment_density_map )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( input_attachment_aspect )
    LIBGCT_ARRAY_OF_FROM_JSON( input_attachment_aspect, pAspectReferences, aspect_reference )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( multiview )
    LIBGCT_ARRAY_OF_FROM_JSON( multiview, pViewMasks, view_mask )
    LIBGCT_ARRAY_OF_FROM_JSON( multiview, pViewOffsets, view_offset )
    LIBGCT_ARRAY_OF_FROM_JSON( multiview, pCorrelationMasks, correlation_mask )
#endif
  }
  render_pass_create_info_t &render_pass_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( basic, AttachmentCount, PAttachments, attachment )
    LIBGCT_ARRAY_OF_REBUILD_CHAIN_WRAPPED( basic, SubpassCount, PSubpasses, subpass )
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( basic, DependencyCount, PDependencies, dependency )
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_density_map )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( input_attachment_aspect, AspectReferenceCount, PAspectReferences, aspect_reference )
    LIBGCT_EXTENSION_REBUILD_CHAIN( input_attachment_aspect )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( multiview, SubpassCount, PViewMasks, view_mask )
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( multiview, DependencyCount, PViewOffsets, view_offset )
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( multiview, CorrelationMaskCount, PCorrelationMasks, correlation_mask )
    LIBGCT_EXTENSION_REBUILD_CHAIN( multiview )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

