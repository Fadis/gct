#include <fstream>
#include <gct/image.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/format.hpp>
#include <hdmap/cross.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace gct {
  void command_buffer_recorder_t::copy(
    const std::shared_ptr< image_t > &src,
    const std::shared_ptr< image_t > &dest,
    const std::vector< vk::ImageCopy > &range
  ) {
    for( const auto &r: range ) {
      const auto &ll = src->get_layout().get_layout_map();
      const auto &rl = dest->get_layout().get_layout_map();
      const auto llrect = ll.rect(
        ll.enc( r.srcSubresource.mipLevel, r.srcSubresource.baseArrayLayer ),
        ll.enc( r.srcSubresource.mipLevel + 1u, r.srcSubresource.baseArrayLayer + r.srcSubresource.layerCount )
      );
      const auto rlrect = rl.rect(
        rl.enc( r.dstSubresource.mipLevel, r.dstSubresource.baseArrayLayer ),
        rl.enc( r.dstSubresource.mipLevel + 1u, r.dstSubresource.baseArrayLayer + r.dstSubresource.layerCount )
      );
      auto intersection = hdmap::cross(
        ll, rl, llrect, rlrect
      );
      for( const auto &[key,value]: intersection.leaves() ) {
        const auto [mip_begin,array_begin] = intersection.dec( key.left_top );
        const auto [mip_end,array_end] = intersection.dec( key.right_bottom );
        (*get_factory())->copyImage(
          **src,
          value.first,
          **dest,
          value.second,
          {
            vk::ImageCopy()
              .setSrcSubresource(
                vk::ImageSubresourceLayers()
                  .setAspectMask( r.srcSubresource.aspectMask )
                  .setMipLevel( mip_begin + r.srcSubresource.mipLevel )
                  .setBaseArrayLayer( array_begin + r.srcSubresource.baseArrayLayer )
                  .setLayerCount( array_end - array_begin )
              )
              .setSrcOffset( r.srcOffset )
              .setDstSubresource(
                vk::ImageSubresourceLayers()
                  .setAspectMask( r.dstSubresource.aspectMask )
                  .setMipLevel( mip_begin + r.dstSubresource.mipLevel )
                  .setBaseArrayLayer( array_begin + r.dstSubresource.baseArrayLayer )
                  .setLayerCount( array_end - array_begin )
              )
              .setSrcOffset( r.dstOffset )
              .setExtent( r.extent )
          }
        );
      }
    }
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
