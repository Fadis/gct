#include <fstream>
#include <gct/image.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/format.hpp>

namespace gct {
  void command_buffer_recorder_t::copy(
    const std::shared_ptr< buffer_t > &src,
    const std::shared_ptr< image_t > &dest,
    const std::vector< vk::BufferImageCopy > &range,
    vk::ImageLayout final_layout
  ) {
    auto dest_layout = dest->get_props().get_basic().initialLayout;
    convert_image(
      dest,
      vk::ImageLayout::eTransferDstOptimal
    );
    (*get_factory())->copyBufferToImage(
      **src,
      **dest,
      dest->get_props().get_basic().initialLayout,
      range
    );
    convert_image( dest, final_layout );
    get_factory()->unbound()->keep.push_back( src );
    get_factory()->unbound()->keep.push_back( dest );
  }
  void command_buffer_recorder_t::copy(
    const std::shared_ptr< buffer_t > &src,
    const std::shared_ptr< image_t > &dest,
    const vk::BufferImageCopy &range,
    vk::ImageLayout final_layout
  ) {
    copy( src, dest, std::vector< vk::BufferImageCopy >{ range }, final_layout );
  }
  void command_buffer_recorder_t::copy(
    const std::shared_ptr< buffer_t > &src,
    const std::shared_ptr< image_t > &dest,
    vk::ImageLayout final_layout
  ) {
    copy(
      src,
      dest,
      vk::BufferImageCopy()
        .setBufferOffset( 0 )
        .setBufferRowLength( dest->get_props().get_basic().extent.width )
        .setBufferImageHeight( dest->get_props().get_basic().extent.height )
        .setImageSubresource(
          vk::ImageSubresourceLayers()
            .setAspectMask( format_to_aspect( dest->get_props().get_basic().format ) )
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
          dest->get_props().get_basic().extent
        ),
      final_layout
    );
  }
}
