#include <fstream>
#include <gct/image.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>

namespace gct {
  void command_buffer_recorder_t::blit(
    const std::shared_ptr< image_t > &src,
    const std::shared_ptr< image_t > &dest,
    const std::vector< vk::ImageBlit > &range,
    vk::Filter filter
  ) {
    std::vector< vk::ImageMemoryBarrier > old_src;
    std::vector< vk::ImageMemoryBarrier > old_dest;
    bool src_converted = false;
    bool dest_converted = false;
    if( !( src->get_layout().is_uniform() && src->get_layout().is_copyable_source_layout() ) ) {
      old_src = convert_image( src, vk::ImageLayout::eTransferSrcOptimal );
      src_converted = true;
    }
    if( !( dest->get_layout().is_uniform() && dest->get_layout().is_copyable_source_layout() ) ) {
      old_dest = convert_image( dest, vk::ImageLayout::eTransferDstOptimal );
      dest_converted = true;
    }
    (*get_factory())->blitImage(
      **src,
      src->get_layout().get_uniform_layout(),
      **dest,
      dest->get_layout().get_uniform_layout(),
      range,
      filter
    );
    if( dest_converted ) {
      revert_convert_image( dest, old_dest );
    }
    if( src_converted ) {
      revert_convert_image( src, old_src );
    }
    get_factory()->unbound()->keep.push_back( src );
    get_factory()->unbound()->keep.push_back( dest );
  }
  void command_buffer_recorder_t::blit(
    const std::shared_ptr< image_t > &src,
    const std::shared_ptr< image_t > &dest,
    const vk::ImageBlit &range,
    vk::Filter filter
  ) {
    blit( src, dest, std::vector< vk::ImageBlit >{ range }, filter );
  }
  void command_buffer_recorder_t::blit(
    const std::shared_ptr< image_t > &src,
    const std::shared_ptr< image_t > &dest
  ) {
    std::vector< vk::ImageBlit > ranges;
    const auto mip_levels = std::min(
      src->get_props().get_basic().mipLevels,
      dest->get_props().get_basic().mipLevels
    );
    for( std::uint32_t mip = 0u; mip != mip_levels; ++mip ) {
      ranges.push_back(
        vk::ImageBlit()
          .setSrcSubresource(
            vk::ImageSubresourceLayers()
              .setAspectMask( vk::ImageAspectFlagBits::eColor )
              .setMipLevel( mip )
              .setBaseArrayLayer( 0 )
              .setLayerCount( std::min(
                src->get_props().get_basic().arrayLayers, 
                dest->get_props().get_basic().arrayLayers
              ) )
          )
          .setSrcOffsets({
            vk::Offset3D(),
            vk::Offset3D()
              .setX(
                std::max( src->get_props().get_basic().extent.width >> mip, 1u )
              )
              .setY(
                std::max( src->get_props().get_basic().extent.height >> mip, 1u )
              )
              .setZ(
                std::max( src->get_props().get_basic().extent.depth >> mip, 1u )
              )
          })
          .setDstSubresource(
            vk::ImageSubresourceLayers()
              .setAspectMask( vk::ImageAspectFlagBits::eColor )
              .setMipLevel( mip )
              .setBaseArrayLayer( 0 )
              .setLayerCount( std::min(
                src->get_props().get_basic().arrayLayers, 
                dest->get_props().get_basic().arrayLayers
              ) )
          )
          .setDstOffsets({
            vk::Offset3D(),
            vk::Offset3D()
              .setX(
                std::max( dest->get_props().get_basic().extent.width >> mip, 1u )
              )
              .setY(
                std::max( dest->get_props().get_basic().extent.height >> mip, 1u )
              )
              .setZ(
                std::max( dest->get_props().get_basic().extent.depth >> mip, 1u )
              )
          })
      );
    }
    blit( src, dest,ranges, vk::Filter::eNearest );
  }
  void command_buffer_recorder_t::blit(
    const std::shared_ptr< image_view_t > &src,
    const std::shared_ptr< image_view_t > &dest,
    const std::vector< vk::ImageBlit > &range,
    vk::Filter filter
  ) {
    auto range_ = range;
    for( auto &r : range_ ) {
      r.srcSubresource.baseArrayLayer += src->get_props().get_basic().subresourceRange.baseArrayLayer;
      r.srcSubresource.mipLevel += src->get_props().get_basic().subresourceRange.baseMipLevel;
      r.dstSubresource.baseArrayLayer += dest->get_props().get_basic().subresourceRange.baseArrayLayer;
      r.dstSubresource.mipLevel += dest->get_props().get_basic().subresourceRange.baseMipLevel;
    }
    blit(
      src->get_factory(),
      dest->get_factory(),
      range_,
      filter
    );
  }
}
