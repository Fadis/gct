#include <gct/image.hpp>
#include <gct/image_view.hpp>
#include <gct/framebuffer_create_info.hpp>

namespace gct {
  framebuffer_create_info_t &framebuffer_create_info_t::rebuild_chain() {
    raw_attachment.clear();
    raw_attachment.reserve( attachment.size() );
    std::transform(
      attachment.begin(),
      attachment.end(),
      std::back_inserter( raw_attachment ),
      []( const auto &v ) {
        return **v;
      }
    );
    basic
      .setAttachmentCount( raw_attachment.size() )
      .setPAttachments( raw_attachment.data() );
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( attachments )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  framebuffer_create_info_t &framebuffer_create_info_t::add_attachment( const std::shared_ptr< image_view_t > &v ) {
    attachment.push_back( v );
    chained = false;
    return *this;
  }
  framebuffer_create_info_t &framebuffer_create_info_t::clear_attachment() {
    attachment.clear();
    chained = false;
    return *this;
  }
  std::uint32_t framebuffer_create_info_t::get_width() const {
    if( attachment.empty() ) return 0u;
    return attachment[ 0 ]->get_factory()->get_props().get_basic().extent.width;
  }
  std::uint32_t framebuffer_create_info_t::get_height() const {
    if( attachment.empty() ) return 0u; 
    return attachment[ 0 ]->get_factory()->get_props().get_basic().extent.height;
  }
}

