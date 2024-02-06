#include <vulkan/vulkan.hpp>
#if defined(VK_VERSION_1_2) || defined(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME)
#include <nlohmann/json.hpp>
#ifdef VK_VERSION_1_2
#include <vulkan2json/RenderPassCreateInfo2.hpp>
#else
#include <vulkan2json/RenderPassCreateInfo2KHR.hpp>
#endif
#ifdef VK_EXT_SUBPASS_MERGE_FEEDBACK_EXTENSION_NAME
#include <vulkan2json/RenderPassCreationControlEXT.hpp>
#include <vulkan2json/RenderPassSubpassFeedbackCreateInfoEXT.hpp>
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
#include <vulkan2json/RenderPassFragmentDensityMapCreateInfoEXT.hpp>
#endif
#include <gct/render_pass_create_info2.hpp>

namespace gct {
  void to_json( nlohmann::json &root, const render_pass_create_info2_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
    LIBGCT_ARRAY_OF_TO_JSON_WRAPPED( basic, pAttachments, attachment )
    LIBGCT_ARRAY_OF_TO_JSON_WRAPPED( basic, pSubpasses, subpass )
    LIBGCT_ARRAY_OF_TO_JSON_WRAPPED( basic, pDependencies, dependency )
    LIBGCT_ARRAY_OF_TO_JSON( basic, pCorrelatedViewMasks, correlative_view_mask )
#ifdef VK_EXT_SUBPASS_MERGE_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( creation_control )
    LIBGCT_EXTENSION_TO_JSON( feedback )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_density_map )
#endif
  }
  void from_json( const nlohmann::json &root, render_pass_create_info2_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to subpass_dependency2_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
    LIBGCT_ARRAY_OF_FROM_JSON_WRAPPED( basic, pAttachments, attachment )
    LIBGCT_ARRAY_OF_FROM_JSON_WRAPPED( basic, pSubpasses, subpass )
    LIBGCT_ARRAY_OF_FROM_JSON_WRAPPED( basic, pDependencies, dependency )
    LIBGCT_ARRAY_OF_FROM_JSON( basic, pCorrelatedViewMasks, correlative_view_mask )
#ifdef VK_EXT_SUBPASS_MERGE_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( creation_control )
    LIBGCT_EXTENSION_FROM_JSON( feedback )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( fragment_density_map )
#endif
  }
  render_pass_create_info2_t &render_pass_create_info2_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_ARRAY_OF_REBUILD_CHAIN_WRAPPED( basic, AttachmentCount, PAttachments, attachment )
    LIBGCT_ARRAY_OF_REBUILD_CHAIN_WRAPPED( basic, SubpassCount, PSubpasses, subpass )
    LIBGCT_ARRAY_OF_REBUILD_CHAIN_WRAPPED( basic, DependencyCount, PDependencies, dependency )
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( basic, CorrelatedViewMaskCount, PCorrelatedViewMasks, correlative_view_mask )
#ifdef VK_EXT_SUBPASS_MERGE_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( creation_control )
    LIBGCT_EXTENSION_REBUILD_CHAIN( feedback )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_density_map )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

#endif

