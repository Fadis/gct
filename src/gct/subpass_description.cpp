#include <nlohmann/json.hpp>
#include <vulkan2json/SubpassDescription.hpp>
#include <vulkan2json/AttachmentReference.hpp>
#include <gct/subpass_description.hpp>

namespace gct {
  void to_json( nlohmann::json &root, const subpass_description_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
    LIBGCT_ARRAY_OF_TO_JSON( basic, pInputAttachments, input_attachment )
    LIBGCT_ARRAY_OF_TO_JSON( basic, pColorAttachments, color_attachment )
    LIBGCT_ARRAY_OF_TO_JSON( basic, pPreserveAttachments, preserve_attachment )
  }
  void from_json( const nlohmann::json &root, subpass_description_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to subpass_description_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
    LIBGCT_ARRAY_OF_FROM_JSON( basic, pInputAttachments, input_attachment )
    LIBGCT_ARRAY_OF_FROM_JSON( basic, pColorAttachments, color_attachment )
    LIBGCT_ARRAY_OF_FROM_JSON( basic, pPreserveAttachments, preserve_attachment )
  }
  subpass_description_t &subpass_description_t::rebuild_chain() {
    if( chained ) return *this;
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( basic, InputAttachmentCount, PInputAttachments, input_attachment )
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( basic, ColorAttachmentCount, PColorAttachments, color_attachment )
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( basic, PreserveAttachmentCount, PPreserveAttachments, preserve_attachment )
    basic
      .setPResolveAttachments( resolve_attachment ? static_cast< vk::AttachmentReference* >( nullptr ) : resolve_attachment.get() )
      .setPDepthStencilAttachment( !depth_stencil_attachment ? static_cast< vk::AttachmentReference* >( nullptr ) : depth_stencil_attachment.get() );
    return *this;
  }
  subpass_description_t &subpass_description_t::add_input_attachment( std::uint32_t attachment, vk::ImageLayout layout ) {
    return add_input_attachment(
      vk::AttachmentReference()
        .setAttachment( attachment )
        .setLayout( layout )
    );
  }
  subpass_description_t &subpass_description_t::add_color_attachment( std::uint32_t attachment, vk::ImageLayout layout ) {
    return add_color_attachment(
      vk::AttachmentReference()
        .setAttachment( attachment )
        .setLayout( layout )
    );
  }
  subpass_description_t &subpass_description_t::set_resolve_attachment( std::uint32_t attachment, vk::ImageLayout layout ) {
    return set_resolve_attachment(
      vk::AttachmentReference()
        .setAttachment( attachment )
        .setLayout( layout )
    );
  }
  subpass_description_t &subpass_description_t::set_resolve_attachment( const vk::AttachmentReference &v ) {
    resolve_attachment.reset(
      new vk::AttachmentReference( v ) 
    );
    chained = false;
    return *this;
  }
  subpass_description_t &subpass_description_t::clear_resolve_attachment() {
    resolve_attachment.reset();
    chained = false;
    return *this;
  }
  subpass_description_t &subpass_description_t::set_depth_stencil_attachment( std::uint32_t attachment, vk::ImageLayout layout ) {
    return set_depth_stencil_attachment(
      vk::AttachmentReference()
        .setAttachment( attachment )
        .setLayout( layout )
    );
  }
  subpass_description_t &subpass_description_t::set_depth_stencil_attachment( const vk::AttachmentReference &v ) {
    depth_stencil_attachment.reset(
      new vk::AttachmentReference( v ) 
    );
    chained = false;
    return *this;
  }
  subpass_description_t &subpass_description_t::clear_depth_stencil_attachment() {
    depth_stencil_attachment.reset();
    chained = false;
    return *this;
  }
}

