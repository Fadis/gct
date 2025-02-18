#ifndef GCT_SUBPASS_DESCRIPTION_HPP
#define GCT_SUBPASS_DESCRIPTION_HPP
#include <cstdint>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>
#include <gct/array_of.hpp>

namespace gct {
  class subpass_description_t : public chained_t {
  public:
    using self_type = subpass_description_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::SubpassDescription )
    LIBGCT_ARRAY_OF( vk::AttachmentReference, input_attachment )
    LIBGCT_ARRAY_OF( vk::AttachmentReference, color_attachment )
    LIBGCT_ARRAY_OF_SMALL( std::uint32_t, preserve_attachment )
  private:
    deep_copy_unique_ptr< vk::AttachmentReference > resolve_attachment;
    deep_copy_unique_ptr< vk::AttachmentReference > depth_stencil_attachment;
  public:
    subpass_description_t &add_input_attachment( uint32_t, vk::ImageLayout );
    subpass_description_t &add_color_attachment( uint32_t, vk::ImageLayout );
    subpass_description_t &set_resolve_attachment( uint32_t, vk::ImageLayout );
    subpass_description_t &set_resolve_attachment( const vk::AttachmentReference& );
    subpass_description_t &clear_resolve_attachment();
    subpass_description_t &set_depth_stencil_attachment( uint32_t, vk::ImageLayout );
    subpass_description_t &set_depth_stencil_attachment( const vk::AttachmentReference& );
    subpass_description_t &clear_depth_stencil_attachment();
  };
  void to_json( nlohmann::json &root, const subpass_description_t &v );
  void from_json( const nlohmann::json &root, subpass_description_t &v );
}

#endif


