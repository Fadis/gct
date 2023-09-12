#include <nlohmann/json.hpp>
#include <gct/image.hpp>
#include <gct/image_view.hpp>
#include <gct/framebuffer_create_info.hpp>
#include <vulkan2json/FramebufferCreateInfo.hpp>
#ifdef VK_VERSION_1_2
#include <vulkan2json/FramebufferAttachmentsCreateInfo.hpp>
#elif defined(VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME)
#include <vulkan2json/FramebufferAttachmentsCreateInfoKHR.hpp>
#endif
namespace gct {
  void to_json( nlohmann::json &root, const framebuffer_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( attachments )
#endif
  }
  void from_json( const nlohmann::json &root, framebuffer_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to framebuffer_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( attachments )
#endif
  }
  framebuffer_create_info_t &framebuffer_create_info_t::rebuild_chain() {
    if( chained ) return *this;
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
      .setPAttachments( raw_attachment.data() )
      .setWidth( get_width() )
      .setHeight( get_height() )
      .setLayers( get_layer() );
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
    if( attachment.empty() ) return get_basic().width;
    return attachment[ 0 ]->get_factory()->get_props().get_basic().extent.width;
  }
  std::uint32_t framebuffer_create_info_t::get_height() const {
    if( attachment.empty() ) return get_basic().height;
    return attachment[ 0 ]->get_factory()->get_props().get_basic().extent.height;
  }
  std::uint32_t framebuffer_create_info_t::get_layer() const {
    if( attachment.empty() ) return get_basic().layers;
    return attachment[ 0 ]->get_props().get_basic().subresourceRange.layerCount;
  }
}

