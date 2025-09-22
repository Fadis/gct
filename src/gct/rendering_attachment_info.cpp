#include <vulkan/vulkan.hpp>
#if defined(VK_VERSION_1_3) || defined(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME)
#include <nlohmann/json.hpp>
#ifdef VK_VERSION_1_3
#include <vulkan2json/RenderingAttachmentInfo.hpp>
#else
#include <vulkan2json/RenderingAttachmentInfoKHR.hpp>
#endif
#ifdef VK_EXT_ATTACHMENT_FEEDBACK_LOOP_LAYOUT_EXTENSION_NAME
#include <vulkan2json/AttachmentFeedbackLoopInfoEXT.hpp>
#endif
#include <gct/image_view.hpp>
#include <gct/rendering_attachment_info.hpp>

namespace gct {
void to_json( nlohmann::json &root, const rendering_attachment_info_t &v ) {
  root = nlohmann::json::object();
  root[ "basic" ] = v.get_basic();
  if( v.get_view() ) {
    root[ "basic" ][ "imageView" ] = *v.get_view();
  }
#ifdef VK_EXT_ATTACHMENT_FEEDBACK_LOOP_LAYOUT_EXTENSION_NAME
  LIBGCT_EXTENSION_TO_JSON( feedback_loop )
#endif
}
void from_json( const nlohmann::json &root, rendering_attachment_info_t &v ) {
  if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to rendering_attachment_info_t", __FILE__, __LINE__ );
  LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_EXT_ATTACHMENT_FEEDBACK_LOOP_LAYOUT_EXTENSION_NAME
  LIBGCT_EXTENSION_FROM_JSON( feedback_loop )
#endif
}
rendering_attachment_info_t &rendering_attachment_info_t::rebuild_chain() {
  LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
  if( view ) {
    basic.imageView = **view;
  }
#ifdef VK_EXT_ATTACHMENT_FEEDBACK_LOOP_LAYOUT_EXTENSION_NAME
  LIBGCT_EXTENSION_REBUILD_CHAIN( feedback_loop )
#endif
  LIBGCT_EXTENSION_END_REBUILD_CHAIN
}
rendering_attachment_info_t &rendering_attachment_info_t::set_view( const std::shared_ptr< image_view_t > &v ) {
  view = v;
  return *this;
}
rendering_attachment_info_t &rendering_attachment_info_t::clear_view() {
  view.reset();
  return *this;
}

}
#endif


