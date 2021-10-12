#include <gct/subpass_description.hpp>

namespace gct {
  subpass_description_t &subpass_description_t::rebuild_chain() {
    basic
      .setInputAttachmentCount( input_attachment.size() )
      .setPInputAttachments( input_attachment.empty() ? static_cast< vk::AttachmentReference* >( nullptr ) : input_attachment.data() )
      .setColorAttachmentCount( color_attachment.size() )
      .setPColorAttachments( color_attachment.empty() ? static_cast< vk::AttachmentReference* >( nullptr ) : color_attachment.data() )
      .setPResolveAttachments( resolve_attachment ? static_cast< vk::AttachmentReference* >( nullptr ) : resolve_attachment.get() )
      .setPDepthStencilAttachment( !depth_stencil_attachment ? static_cast< vk::AttachmentReference* >( nullptr ) : depth_stencil_attachment.get() )
      .setPreserveAttachmentCount( preserve_attachment.size() )
      .setPPreserveAttachments( preserve_attachment.empty() ? static_cast< std::uint32_t* >( nullptr ) : preserve_attachment.data() );
    return *this;
  }
  subpass_description_t &subpass_description_t::add_input_attachment( std::uint32_t attachment, vk::ImageLayout layout ) {
    return add_input_attachment(
      vk::AttachmentReference()
        .setAttachment( attachment )
        .setLayout( layout )
    );
  }
  subpass_description_t &subpass_description_t::add_input_attachment( const vk::AttachmentReference &v ) {
    input_attachment.push_back( v );
    chained = false;
    return *this;
  }
  subpass_description_t &subpass_description_t::clear_input_attachment() {
    input_attachment.clear();
    chained = false;
    return *this;
  }
  subpass_description_t &subpass_description_t::add_color_attachment( std::uint32_t attachment, vk::ImageLayout layout ) {
    return add_color_attachment(
      vk::AttachmentReference()
        .setAttachment( attachment )
        .setLayout( layout )
    );
  }
  subpass_description_t &subpass_description_t::add_color_attachment( const vk::AttachmentReference &v ) {
    color_attachment.push_back( v );
    chained = false;
    return *this;
  }
  subpass_description_t &subpass_description_t::clear_color_attachment() {
    color_attachment.clear();
    chained = false;
    return *this;
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
  subpass_description_t &subpass_description_t::add_preserve_attachment( std::uint32_t v ) {
    preserve_attachment.push_back( v );
    chained = false;
    return *this;
  }
  subpass_description_t &subpass_description_t::clear_preserve_attachment() {
    preserve_attachment.clear();
    chained = false;
    return *this;
  }
}

