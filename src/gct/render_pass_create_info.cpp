#include <gct/surface.hpp>
#include <gct/render_pass_create_info.hpp>

namespace gct {
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

