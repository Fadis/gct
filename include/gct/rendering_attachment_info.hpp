#ifndef GCT_RENDERING_ATTACHMENT_INFO_HPP
#define GCT_RENDERING_ATTACHMENT_INFO_HPP
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>
#include <gct/array_of.hpp>

#if defined(VK_VERSION_1_3) || defined(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME)
namespace gct {

class image_view_t;
class rendering_attachment_info_t : public chained_t {
public:
  using self_type = rendering_attachment_info_t;
  LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
#ifdef VK_VERSION_1_3
  LIBGCT_BASIC_SETTER( vk::RenderingAttachmentInfo )
#else
  LIBGCT_BASIC_SETTER( vk::RenderingAttachmentInfoKHR )
#endif
#ifdef VK_EXT_ATTACHMENT_FEEDBACK_LOOP_LAYOUT_EXTENSION_NAME
  LIBGCT_EXTENSION_SETTER( vk::AttachmentFeedbackLoopInfoEXT, feedback_loop )
#endif
  rendering_attachment_info_t &set_view( const std::shared_ptr< image_view_t > & );
  rendering_attachment_info_t &clear_view();
  const std::shared_ptr< image_view_t > &get_view() const {
    return view;
  }
  rendering_attachment_info_t &make_inout() {
    basic.setLoadOp( vk::AttachmentLoadOp::eLoad );
    return *this;
  }
private:
  std::shared_ptr< image_view_t > view;
};
  
void to_json( nlohmann::json &root, const rendering_attachment_info_t &v );
void from_json( const nlohmann::json &root, rendering_attachment_info_t &v );

}
#endif

#endif

