#ifndef GCT_RENDER_PASS_BEGIN_INFO_HPP
#define GCT_RENDER_PASS_BEGIN_INFO_HPP
#include <memory>
#include <unordered_map>
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>
#include <gct/subpass_description.hpp>

namespace gct {
  class render_pass_t;
  class framebuffer_t;
  class render_pass_begin_info_t : public chained_t {
  public:
    using self_type = render_pass_begin_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::RenderPassBeginInfo )
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::DeviceGroupRenderPassBeginInfo , group )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::RenderPassAttachmentBeginInfo , attachment )
#elif defined(VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::RenderPassAttachmentBeginInfoKHR , attachment )
#endif
#ifdef VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::RenderPassSampleLocationsBeginInfoEXT , sample_location )
#endif
#ifdef VK_QCOM_RENDER_PASS_TRANSFORM_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::RenderPassTransformBeginInfoQCOM , transform )
#endif
  private:
    std::vector< vk::ClearValue > clear_value;
  public:
    render_pass_begin_info_t &add_clear_value( const vk::ClearValue& );
    render_pass_begin_info_t &clear_clear_value();
  private:
    std::shared_ptr< render_pass_t > render_pass;
    std::shared_ptr< framebuffer_t > framebuffer;
  public:
    render_pass_begin_info_t &set_render_pass( const std::shared_ptr< render_pass_t >& );
    render_pass_begin_info_t &clear_render_pass();
    render_pass_begin_info_t &set_framebuffer( const std::shared_ptr< framebuffer_t >& );
    render_pass_begin_info_t &clear_framebuffer();
    const std::shared_ptr< render_pass_t > &get_render_pass() const {
      return render_pass;
    }
    const std::shared_ptr< framebuffer_t > &get_framebuffer() const {
      return framebuffer;
    }
  };
  void to_json( nlohmann::json &root, const render_pass_begin_info_t &v );
  void from_json( const nlohmann::json &root, render_pass_begin_info_t &v );
}

#endif

