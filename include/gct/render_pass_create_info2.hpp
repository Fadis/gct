#ifndef GCT_RENDER_PASS_CREATE_INFO2_HPP
#define GCT_RENDER_PASS_CREATE_INFO2_HPP
#include <memory>
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>
#include <gct/attachment_description2.hpp>
#include <gct/subpass_description2.hpp>
#include <gct/subpass_dependency2.hpp>
#include <gct/array_of.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace gct {
#if defined(VK_VERSION_1_2) || defined(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME)
  class render_pass_create_info2_t : public chained_t {
  public:
    using self_type = render_pass_create_info2_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
#ifdef VK_VERSION_1_2
    LIBGCT_BASIC_SETTER( vk::RenderPassCreateInfo2 )
    LIBGCT_ARRAY_OF_WRAPPED( attachment_description2_t, vk::AttachmentDescription2, attachment )
    LIBGCT_ARRAY_OF_WRAPPED( subpass_description2_t, vk::SubpassDescription2, subpass )
    LIBGCT_ARRAY_OF_WRAPPED( subpass_dependency2_t, vk::SubpassDependency2, dependency )
#else
    LIBGCT_BASIC_SETTER( vk::RenderPassCreateInfo2KHR )
    LIBGCT_ARRAY_OF_WRAPPED( attachment_description2_t, vk::AttachmentDescription2KHR, attachment )
    LIBGCT_ARRAY_OF_WRAPPED( subpass_description2_t, vk::SubpassDescription2KHR, subpass )
    LIBGCT_ARRAY_OF_WRAPPED( subpass_dependency2_t, vk::SubpassDependency2KHR, dependency )
#endif
    LIBGCT_ARRAY_OF_SMALL( std::uint32_t, correlative_view_mask )
#ifdef VK_EXT_SUBPASS_MERGE_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::RenderPassCreationControlEXT , creation_control )
    LIBGCT_EXTENSION_SETTER( vk::RenderPassSubpassFeedbackCreateInfoEXT , feedback )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::RenderPassFragmentDensityMapCreateInfoEXT , fragment_density_map )
#endif
  };
  void to_json( nlohmann::json &root, const render_pass_create_info2_t &v );
  void from_json( const nlohmann::json &root, render_pass_create_info2_t &v );
}
#endif

#endif

