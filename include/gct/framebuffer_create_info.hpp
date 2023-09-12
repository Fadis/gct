#ifndef GCT_FRAMEBUFFER_CREATE_INFO_HPP
#define GCT_FRAMEBUFFER_CREATE_INFO_HPP
#include <memory>
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class image_view_t;
  class render_pass_t;
  class framebuffer_create_info_t : public chained_t {
  public:
    using self_type = framebuffer_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::FramebufferCreateInfo )
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::FramebufferAttachmentsCreateInfo, attachments )
#elif defined(VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::FramebufferAttachmentsCreateInfoKHR, attachments )
#endif
  private:
    std::vector< std::shared_ptr< image_view_t > > attachment;
    std::vector< vk::ImageView > raw_attachment;
  public:
    framebuffer_create_info_t &add_attachment( const std::shared_ptr< image_view_t >& );
    framebuffer_create_info_t &clear_attachment();
    const std::vector< std::shared_ptr< image_view_t > > &get_attachment() const {
      return attachment;
    }
    std::uint32_t get_width() const;
    std::uint32_t get_height() const;
    std::uint32_t get_layer() const;
  };
  void to_json( nlohmann::json &root, const framebuffer_create_info_t &v );
  void from_json( const nlohmann::json &root, framebuffer_create_info_t &v );
}

#endif

