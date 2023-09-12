#include <fstream>
#include <gct/buffer.hpp>
#include <gct/image.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/format.hpp>

namespace gct {
  void command_buffer_recorder_t::copy(
    const std::shared_ptr< image_t > &src,
    const std::shared_ptr< buffer_t > &dest,
    const std::vector< vk::BufferImageCopy > &range
  ) {
    std::vector< vk::ImageMemoryBarrier > old;
    if( !( src->get_layout().is_uniform() && src->get_layout().is_copyable_source_layout() ) )
      old = convert_image( src, vk::ImageLayout::eTransferSrcOptimal );
    (*get_factory())->copyImageToBuffer(
      **src,
      src->get_layout().get_uniform_layout(),
      **dest,
      range
    );
    if( !( src->get_layout().is_uniform() && src->get_layout().is_copyable_source_layout() ) )
      revert_convert_image( src, old );
    get_factory()->unbound()->keep.push_back( src );
    get_factory()->unbound()->keep.push_back( dest );
  }
  void command_buffer_recorder_t::copy(
    const std::shared_ptr< image_t > &src,
    const std::shared_ptr< buffer_t > &dest,
    const std::vector< vk::BufferImageCopy > &range,
    vk::ImageLayout final_layout
  ) {
    if( !( src->get_layout().is_uniform() && src->get_layout().is_copyable_source_layout() ) )
      convert_image( src, vk::ImageLayout::eTransferSrcOptimal );
    (*get_factory())->copyImageToBuffer(
      **src,
      src->get_layout().get_uniform_layout(),
      **dest,
      range
    );
    convert_image( src, final_layout );
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
            .setLayerCount( src->get_props().get_basic().arrayLayers )
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
    unsigned int i
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
            .setBaseArrayLayer( i )
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
            .setLayerCount( src->get_props().get_basic().arrayLayers )
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
  void command_buffer_recorder_t::copy(
    const std::shared_ptr< image_t > &src,
    const std::shared_ptr< buffer_t > &dest,
    vk::ImageLayout final_layout,
    unsigned int i
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
            .setBaseArrayLayer( i )
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
