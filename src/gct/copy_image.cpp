#include <fstream>
#include <gct/image.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/format_to_aspect.hpp>

namespace gct {
  void command_buffer_recorder_t::copy(
    const std::shared_ptr< image_t > &src,
    const std::shared_ptr< image_t > &dest,
    const std::vector< vk::ImageCopy > &range
  ) {
    (*get_factory())->copyImage(
      **src,
      src->get_props().get_basic().initialLayout,
      **dest,
      dest->get_props().get_basic().initialLayout,
      range
    );
    get_factory()->unbound()->keep.push_back( src );
    get_factory()->unbound()->keep.push_back( dest );
  }
  void command_buffer_recorder_t::copy(
    const std::shared_ptr< image_t > &src,
    const std::shared_ptr< image_t > &dest,
    const vk::ImageCopy &range
  ) {
    copy( src, dest, std::vector< vk::ImageCopy >{ range } );
  }
  void command_buffer_recorder_t::copy(
    const std::shared_ptr< image_t > &src,
    const std::shared_ptr< image_t > &dest
  ) {
    const auto max_mip = std::min( src->get_props().get_basic().mipLevels, dest->get_props().get_basic().mipLevels );
    std::vector< vk::ImageCopy > regions;
    regions.reserve( max_mip );
    for( std::uint32_t mip_level = 0u; mip_level != max_mip; ++mip_level ) {
      regions.push_back(
        vk::ImageCopy()
          .setSrcSubresource(
            vk::ImageSubresourceLayers()
              .setAspectMask( format_to_aspect( src->get_props().get_basic().format ) )
              .setMipLevel( mip_level )
              .setBaseArrayLayer( 0 )
              .setLayerCount( std::min( src->get_props().get_basic().arrayLayers, dest->get_props().get_basic().arrayLayers ) )
          )
          .setSrcOffset(
            vk::Offset3D()
              .setX( 0 )
              .setY( 0 )
              .setZ( 0 )
          )
          .setDstSubresource(
            vk::ImageSubresourceLayers()
              .setAspectMask( format_to_aspect( src->get_props().get_basic().format ) )
              .setMipLevel( mip_level )
              .setBaseArrayLayer( 0 )
              .setLayerCount( std::min( src->get_props().get_basic().arrayLayers, dest->get_props().get_basic().arrayLayers ) )
          )
          .setDstOffset(
            vk::Offset3D()
              .setX( 0 )
              .setY( 0 )
              .setZ( 0 )
          )
          .setExtent(
            vk::Extent3D()
              .setWidth( std::min( src->get_props().get_basic().extent.width, dest->get_props().get_basic().extent.width ) >> mip_level )
              .setHeight( std::min( src->get_props().get_basic().extent.height, dest->get_props().get_basic().extent.height ) >> mip_level )
              .setDepth( std::min( src->get_props().get_basic().extent.depth, dest->get_props().get_basic().extent.depth ) >> mip_level )
          )
      );
    }
    copy(
      src,
      dest,
      regions
    );
  }
}
