#include <gct/surface.hpp>
#include <gct/render_pass_create_info.hpp>
#include <vulkan2json/RenderPassCreateInfo.hpp>
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
#include <vulkan2json/RenderPassFragmentDensityMapCreateInfoEXT.hpp>
#endif
#ifdef VK_VERSION_1_1
#include <vulkan2json/RenderPassInputAttachmentAspectCreateInfo.hpp>
#elif defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
#include <vulkan2json/RenderPassInputAttachmentAspectCreateInfoKHR.hpp>
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
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_density_map )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( input_attachment_aspect )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( multiview )
#endif
  }
  void from_json( const nlohmann::json &root, render_pass_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to render_pass_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( fragment_density_map )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( input_attachment_aspect )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( multiview )
#endif
  }
  render_pass_create_info_t &render_pass_create_info_t::rebuild_chain() {
    subpass_sequential.clear(); 
    std::transform(
      subpass.begin(),
      subpass.end(),
      std::back_inserter( subpass_sequential ),
      []( const auto &v ) {
        return v.get_basic();
      }
    );
    basic
      .setAttachmentCount( attachment.size() )
      .setPAttachments( attachment.data() )
      .setSubpassCount( subpass_sequential.size() )
      .setPSubpasses( subpass_sequential.data() )
      .setDependencyCount( dependency.size() )
      .setPDependencies( dependency.data() );
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_density_map )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( input_attachment_aspect )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( multiview )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  render_pass_create_info_t &render_pass_create_info_t::add_attachment( const vk::AttachmentDescription &v ) {
    attachment.push_back( v );
    chained = false;
    return *this;
  }
  render_pass_create_info_t &render_pass_create_info_t::add_subpass( const subpass_description_t &v ) {
    subpass.push_back( v );
    chained = false;
    return *this;
  }
  render_pass_create_info_t &render_pass_create_info_t::add_dependency( const vk::SubpassDependency &v ) {
    dependency.push_back( v );
    chained = false;
    return *this;
  }
  render_pass_create_info_t &render_pass_create_info_t::clear_subpass() {
    attachment.clear();
    subpass.clear();
    dependency.clear();
    return *this;
  }
}

