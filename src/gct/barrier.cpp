
#include <gct/image.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/format_to_aspect.hpp>

namespace gct {
  void command_buffer_recorder_t::barrier(
    vk::AccessFlagBits src_access_mask,
    vk::AccessFlagBits dest_access_mask,
    vk::PipelineStageFlagBits src_stage,
    vk::PipelineStageFlagBits dest_stage,
    vk::DependencyFlagBits dependency,
    const std::vector< std::shared_ptr< buffer_t > > &buffer,
    const std::vector< std::shared_ptr< image_t > > &image
  ) {
    const auto available_queue_family_index = get_factory()->get_factory()->get_factory()->get_available_queue_family_index();
    std::vector< vk::BufferMemoryBarrier > raw_buffer;
    std::transform(
      buffer.begin(),
      buffer.end(),
      std::back_inserter( raw_buffer ),
      [&]( const auto &v ) {
        return vk::BufferMemoryBarrier()
          .setSrcAccessMask( src_access_mask )
          .setDstAccessMask( dest_access_mask )
          .setSrcQueueFamilyIndex( available_queue_family_index )
          .setDstQueueFamilyIndex( available_queue_family_index )
          .setBuffer( **v )
          .setOffset( 0 )
          .setSize( v->get_props().get_basic().size );
      }
    );
    std::vector< vk::ImageMemoryBarrier > raw_image;
    std::transform(
      image.begin(),
      image.end(),
      std::back_inserter( raw_image ),
      [&]( const auto &v ) {
        return vk::ImageMemoryBarrier()
          .setSrcAccessMask( src_access_mask )
          .setDstAccessMask( dest_access_mask )
          .setOldLayout( v->get_props().get_basic().initialLayout )
          .setNewLayout( v->get_props().get_basic().initialLayout )
          .setSrcQueueFamilyIndex( available_queue_family_index )
          .setDstQueueFamilyIndex( available_queue_family_index )
          .setImage( **v )
          .setSubresourceRange(
            vk::ImageSubresourceRange()
              .setAspectMask( format_to_aspect( v->get_props().get_basic().format ) )
              .setBaseMipLevel( 0 )
              .setLevelCount( v->get_props().get_basic().mipLevels )
              .setBaseArrayLayer( 0 )
              .setLayerCount( v->get_props().get_basic().arrayLayers )
          );
      }
    );
    (*get_factory())->pipelineBarrier(
      src_stage,
      dest_stage,
      dependency,
      std::vector< vk::MemoryBarrier >{},
      raw_buffer,
      raw_image
    );
    get_factory()->unbound()->keep.push_back( buffer );
    get_factory()->unbound()->keep.push_back( image );
  }
  void command_buffer_recorder_t::convert_image(
    vk::AccessFlagBits src_access_mask,
    vk::AccessFlagBits dest_access_mask,
    vk::PipelineStageFlagBits src_stage,
    vk::PipelineStageFlagBits dest_stage,
    const std::shared_ptr< image_t > &image,
    uint32_t mip_base,
    uint32_t mip_count,
    vk::ImageLayout from,
    vk::ImageLayout to
  ) {
    (*get_factory())->pipelineBarrier(
      src_stage,
      dest_stage,
      vk::DependencyFlagBits( 0 ),
      {},
      {},
      {
        vk::ImageMemoryBarrier()
          .setOldLayout( from )
          .setNewLayout( to )
          .setSrcAccessMask( src_access_mask )
          .setDstAccessMask( dest_access_mask )
          .setImage( **image )
          .setSubresourceRange(
            vk::ImageSubresourceRange()
              .setAspectMask( format_to_aspect( image->get_props().get_basic().format ) )
              .setBaseMipLevel( mip_base )
              .setLevelCount( mip_count )
              .setBaseArrayLayer( 0 )
              .setLayerCount( image->get_props().get_basic().arrayLayers )
          )
      }
    );
    get_factory()->unbound()->keep.push_back( image );
  }
  void command_buffer_recorder_t::convert_image(
    vk::AccessFlagBits src_access_mask,
    vk::AccessFlagBits dest_access_mask,
    vk::PipelineStageFlagBits src_stage,
    vk::PipelineStageFlagBits dest_stage,
    const std::shared_ptr< image_t > &image,
    vk::ImageLayout from,
    vk::ImageLayout to
  ) {
    (*get_factory())->pipelineBarrier(
      src_stage,
      dest_stage,
      vk::DependencyFlagBits( 0 ),
      {},
      {},
      {
        vk::ImageMemoryBarrier()
          .setOldLayout( from )
          .setNewLayout( to )
          .setSrcAccessMask( src_access_mask )
          .setDstAccessMask( dest_access_mask )
          .setImage( **image )
          .setSubresourceRange(
            vk::ImageSubresourceRange()
              .setAspectMask( format_to_aspect( image->get_props().get_basic().format ) )
              .setBaseMipLevel( 0 )
              .setLevelCount( image->get_props().get_basic().mipLevels )
              .setBaseArrayLayer( 0 )
              .setLayerCount( image->get_props().get_basic().arrayLayers )
          )
      }
    );
    get_factory()->unbound()->keep.push_back( image );
  }
  void command_buffer_recorder_t::convert_image(
    const std::shared_ptr< image_t > &image,
    std::uint32_t mip_base,
    std::uint32_t mip_count,
    vk::ImageLayout from,
    vk::ImageLayout to
  ) {
    (*get_factory())->pipelineBarrier(
      vk::PipelineStageFlagBits::eTransfer,
      vk::PipelineStageFlagBits::eFragmentShader,
      vk::DependencyFlagBits( 0 ),
      {},
      {},
      {
        vk::ImageMemoryBarrier()
          .setOldLayout( from )
          .setNewLayout( to )
          .setSrcAccessMask( vk::AccessFlagBits::eTransferRead )
          .setDstAccessMask( vk::AccessFlagBits::eShaderRead )
          .setImage( **image )
          .setSubresourceRange(
            vk::ImageSubresourceRange()
              .setAspectMask( format_to_aspect( image->get_props().get_basic().format ) )
              .setBaseMipLevel( mip_base )
              .setLevelCount( mip_count )
              .setBaseArrayLayer( 0 )
              .setLayerCount( 1 )
          )
      }
    );
    image->set_layout( to );
    get_factory()->unbound()->keep.push_back( image );
  }
  void command_buffer_recorder_t::convert_image(
    const std::shared_ptr< image_t > &image,
    vk::ImageLayout from,
    vk::ImageLayout to
  ) {
    (*get_factory())->pipelineBarrier(
      vk::PipelineStageFlagBits::eTransfer,
      vk::PipelineStageFlagBits::eFragmentShader,
      vk::DependencyFlagBits( 0 ),
      {},
      {},
      {
        vk::ImageMemoryBarrier()
          .setOldLayout( from )
          .setNewLayout( to )
          .setSrcAccessMask( vk::AccessFlagBits::eTransferRead )
          .setDstAccessMask( vk::AccessFlagBits::eShaderRead )
          .setImage( **image )
          .setSubresourceRange(
            vk::ImageSubresourceRange()
              .setAspectMask( format_to_aspect( image->get_props().get_basic().format ) )
              .setBaseMipLevel( 0 )
              .setLevelCount( image->get_props().get_basic().mipLevels )
              .setBaseArrayLayer( 0 )
              .setLayerCount( image->get_props().get_basic().arrayLayers )
          )
      }
    );
    image->set_layout( to );
    get_factory()->unbound()->keep.push_back( image );
  }
}
