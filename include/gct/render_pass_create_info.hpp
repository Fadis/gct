#ifndef GCT_RENDER_PASS_CREATE_INFO_HPP
#define GCT_RENDER_PASS_CREATE_INFO_HPP
#include <memory>
#include <unordered_map>
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>
#include <gct/subpass_description.hpp>

namespace gct {
  class render_pass_create_info_t : public chained_t {
  public:
    using self_type = render_pass_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::RenderPassCreateInfo )
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::RenderPassFragmentDensityMapCreateInfoEXT , fragment_density_map )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::RenderPassInputAttachmentAspectCreateInfo , input_attachment_aspect )
#elif defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::RenderPassInputAttachmentAspectCreateInfoKHR , input_attachment_aspect )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::RenderPassMultiviewCreateInfo , multiview )
#elif defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::RenderPassMultiviewCreateInfoKHR , multiview )
#endif
  private:
    std::vector< vk::AttachmentDescription > attachment;
    std::vector< subpass_description_t > subpass;
    std::vector< vk::SubpassDescription > subpass_sequential;
    std::vector< vk::SubpassDependency > dependency;
  public:
    render_pass_create_info_t &add_attachment( const vk::AttachmentDescription& );
    render_pass_create_info_t &add_subpass( const subpass_description_t& );
    render_pass_create_info_t &add_dependency( const vk::SubpassDependency& );
    render_pass_create_info_t &clear_subpass();
  };
  void to_json( nlohmann::json &root, const render_pass_create_info_t &v );
  void from_json( const nlohmann::json &root, render_pass_create_info_t &v );
}

#endif

