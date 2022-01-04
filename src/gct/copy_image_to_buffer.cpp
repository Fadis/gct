#include <fstream>
#include <gct/image.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/format.hpp>

namespace gct {
  bool is_copyable_source_layout(
    vk::ImageLayout layout
  ) {
    if( layout == vk::ImageLayout::eSharedPresentKHR )
      return true;
    if( layout == vk::ImageLayout::eTransferSrcOptimal )
      return true;
    if( layout == vk::ImageLayout::eGeneral )
      return true;
    return false;
  }
  void command_buffer_recorder_t::copy(
    const std::shared_ptr< image_t > &src,
    const std::shared_ptr< buffer_t > &dest,
    const std::vector< vk::BufferImageCopy > &range
  ) {
    auto src_layout = src->get_props().get_basic().initialLayout;
    if( !is_copyable_source_layout( src_layout ) )
      convert_image( src, src_layout, vk::ImageLayout::eTransferSrcOptimal );
    (*get_factory())->copyImageToBuffer(
      **src,
      src->get_props().get_basic().initialLayout,
      **dest,
      range
    );
    if( !is_copyable_source_layout( src_layout ) )
      convert_image( src, vk::ImageLayout::eTransferSrcOptimal, src_layout );
    get_factory()->unbound()->keep.push_back( src );
    get_factory()->unbound()->keep.push_back( dest );
  }
  void command_buffer_recorder_t::copy(
    const std::shared_ptr< image_t > &src,
    const std::shared_ptr< buffer_t > &dest,
    const std::vector< vk::BufferImageCopy > &range,
    vk::ImageLayout final_layout
  ) {
    auto src_layout = src->get_props().get_basic().initialLayout;
    convert_image( src, src_layout, vk::ImageLayout::eTransferSrcOptimal );
    (*get_factory())->copyImageToBuffer(
      **src,
      src->get_props().get_basic().initialLayout,
      **dest,
      range
    );
    convert_image( src, vk::ImageLayout::eTransferSrcOptimal, final_layout );
    get_factory()->unbound()->keep.push_back( src );
    get_factory()->unbound()->keep.push_back( dest );
  }
  void command_buffer_recorder_t::copy(
    const std::shared_ptr< image_t > &src,
    const std::shared_ptr< buffer_t > &dest,
    const vk::BufferImageCopy &range
  ) {
    copy( src, dest, std::vector< vk::BufferImageCopy >{ range } );
  }
  void command_buffer_recorder_t::copy(
    const std::shared_ptr< image_t > &src,
    const std::shared_ptr< buffer_t > &dest,
    const vk::BufferImageCopy &range,
    vk::ImageLayout final_layout
  ) {
    copy( src, dest, std::vector< vk::BufferImageCopy >{ range }, final_layout );
  }
  void command_buffer_recorder_t::copy(
    const std::shared_ptr< image_t > &src,
    const std::shared_ptr< buffer_t > &dest
  ) {
    copy(
      src,
      dest,
      vk::BufferImageCopy()
        .setBufferOffset( 0 )
        .setBufferRowLength( src->get_props().get_basic().extent.width )
        .setBufferImageHeight( src->get_props().get_basic().extent.height )
        .setImageSubresource(
          vk::ImageSubresourceLayers()
            .setAspectMask( format_to_aspect( src->get_props().get_basic().format ) )
            .setMipLevel( 0 )
            .setBaseArrayLayer( 0 )
            .setLayerCount( 1 )
        )
        .setImageOffset(
          vk::Offset3D()
            .setX( 0 )
            .setY( 0 )
            .setZ( 0 )
        )
        .setImageExtent(
          src->get_props().get_basic().extent
        )
    );
  }
  void command_buffer_recorder_t::copy(
    const std::shared_ptr< image_t > &src,
    const std::shared_ptr< buffer_t > &dest,
    vk::ImageLayout final_layout
  ) {
    copy(
      src,
      dest,
      vk::BufferImageCopy()
        .setBufferOffset( 0 )
        .setBufferRowLength( src->get_props().get_basic().extent.width )
        .setBufferImageHeight( src->get_props().get_basic().extent.height )
        .setImageSubresource(
          vk::ImageSubresourceLayers()
            .setAspectMask( format_to_aspect( src->get_props().get_basic().format ) )
            .setMipLevel( 0 )
            .setBaseArrayLayer( 0 )
            .setLayerCount( 1 )
        )
        .setImageOffset(
          vk::Offset3D()
            .setX( 0 )
            .setY( 0 )
            .setZ( 0 )
        )
        .setImageExtent(
          src->get_props().get_basic().extent
        ),
      final_layout
    );
  }
}
