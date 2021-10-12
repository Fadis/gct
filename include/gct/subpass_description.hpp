#ifndef GCT_SUBPASS_DESCRIPTION_HPP
#define GCT_SUBPASS_DESCRIPTION_HPP
#include <memory>
#include <vector>
#include <cstdint>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class subpass_description_t {
  public:
    using self_type = subpass_description_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::SubpassDescription )
  private:
    std::vector< vk::AttachmentReference > input_attachment;
    std::vector< vk::AttachmentReference > color_attachment;
    deep_copy_unique_ptr< vk::AttachmentReference > resolve_attachment;
    deep_copy_unique_ptr< vk::AttachmentReference > depth_stencil_attachment;
    std::vector< std::uint32_t > preserve_attachment;
  public:
    subpass_description_t &add_input_attachment( uint32_t, vk::ImageLayout );
    subpass_description_t &add_input_attachment( const vk::AttachmentReference& );
    subpass_description_t &clear_input_attachment();
    subpass_description_t &add_color_attachment( uint32_t, vk::ImageLayout );
    subpass_description_t &add_color_attachment( const vk::AttachmentReference& );
    subpass_description_t &clear_color_attachment();
    subpass_description_t &set_resolve_attachment( uint32_t, vk::ImageLayout );
    subpass_description_t &set_resolve_attachment( const vk::AttachmentReference& );
    subpass_description_t &clear_resolve_attachment();
    subpass_description_t &set_depth_stencil_attachment( uint32_t, vk::ImageLayout );
    subpass_description_t &set_depth_stencil_attachment( const vk::AttachmentReference& );
    subpass_description_t &clear_depth_stencil_attachment();
    subpass_description_t &add_preserve_attachment( std::uint32_t );
    subpass_description_t &clear_preserve_attachment();
  };
}

#endif


