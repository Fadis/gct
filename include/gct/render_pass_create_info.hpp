#ifndef GCT_RENDER_PASS_CREATE_INFO_HPP
#define GCT_RENDER_PASS_CREATE_INFO_HPP
#include <memory>
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>
#include <gct/subpass_description.hpp>
#include <gct/array_of.hpp>

namespace gct {
  class render_pass_create_info_t : public chained_t {
  public:
    using self_type = render_pass_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::RenderPassCreateInfo )
    LIBGCT_ARRAY_OF( vk::AttachmentDescription, attachment )
    LIBGCT_ARRAY_OF_WRAPPED( subpass_description_t,vk::SubpassDescription, subpass )
    LIBGCT_ARRAY_OF( vk::SubpassDependency, dependency )
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::RenderPassFragmentDensityMapCreateInfoEXT , fragment_density_map )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::RenderPassInputAttachmentAspectCreateInfo , input_attachment_aspect )
    LIBGCT_ARRAY_OF( vk::InputAttachmentAspectReference, aspect_reference )
#elif defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::RenderPassInputAttachmentAspectCreateInfoKHR , input_attachment_aspect )
    LIBGCT_ARRAY_OF( vk::InputAttachmentAspectReferenceKHR, aspect_reference )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::RenderPassMultiviewCreateInfo , multiview )
    LIBGCT_ARRAY_OF_SMALL( std::uint32_t, view_mask )
    LIBGCT_ARRAY_OF_SMALL( std::int32_t, view_offset )
    LIBGCT_ARRAY_OF_SMALL( std::uint32_t, correlation_mask )
#elif defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::RenderPassMultiviewCreateInfoKHR , multiview )
    LIBGCT_ARRAY_OF_SMALL( std::uint32_t, view_mask )
    LIBGCT_ARRAY_OF_SMALL( std::int32_t, view_offset )
    LIBGCT_ARRAY_OF_SMALL( std::uint32_t, correlation_mask )
#endif
  };
  void to_json( nlohmann::json &root, const render_pass_create_info_t &v );
  void from_json( const nlohmann::json &root, render_pass_create_info_t &v );
}

#endif

