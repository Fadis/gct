
#include <gct/image.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/format.hpp>
#include <gct/get_device.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace gct {
  std::vector< vk::ImageMemoryBarrier > command_buffer_recorder_t::barrier(
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
    const auto api_version = get_device( *this ).get_api_version();
    const auto &exts = get_device( *this ).get_activated_extensions();
    const bool synchronization2_is_activated = api_version >= VK_API_VERSION_1_3 || exts.find( "VK_KHR_synchronization2" ) != exts.end();
    for( auto &v: image ) {
      if( synchronization2_is_activated ) {
        raw_image.push_back(
          vk::ImageMemoryBarrier()
            .setSrcAccessMask( src_access_mask )
            .setDstAccessMask( dest_access_mask )
            .setOldLayout( vk::ImageLayout::eUndefined )
            .setNewLayout( vk::ImageLayout::eUndefined )
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
            )
        );
      }
      else {
        for( auto &f: v->get_layout().get_layout() ) {
          raw_image.push_back(
            vk::ImageMemoryBarrier( f )
              .setSrcAccessMask( src_access_mask )
              .setDstAccessMask( dest_access_mask )
              .setSrcQueueFamilyIndex( available_queue_family_index )
              .setDstQueueFamilyIndex( available_queue_family_index )
              .setImage( **v )
              .setSubresourceRange(
                vk::ImageSubresourceRange( f.subresourceRange )
                  .setAspectMask( format_to_aspect( v->get_props().get_basic().format ) )
              )
          );
        }
      }
    }
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
    return raw_image;
  }
  std::vector< vk::ImageMemoryBarrier > command_buffer_recorder_t::convert_image(
    vk::AccessFlagBits src_access_mask,
    vk::AccessFlagBits dest_access_mask,
    vk::PipelineStageFlagBits src_stage,
    vk::PipelineStageFlagBits dest_stage,
    const std::shared_ptr< image_t > &image,
    uint32_t mip_base,
    uint32_t mip_count,
    uint32_t array_base,
    uint32_t array_count,
    vk::ImageLayout to
  ) {
    const auto available_queue_family_index = get_factory()->get_factory()->get_factory()->get_available_queue_family_index();
    std::vector< vk::ImageMemoryBarrier > raw_image;
    for(
      auto &f:
      image->get_layout().get_layout(
        mip_base,
        mip_count,
        array_base,
        array_count
      )
    ) {
      raw_image.push_back(
        vk::ImageMemoryBarrier( f )
          .setNewLayout( to )
          .setSrcAccessMask( src_access_mask )
          .setDstAccessMask( dest_access_mask )
          .setSrcQueueFamilyIndex( available_queue_family_index )
          .setDstQueueFamilyIndex( available_queue_family_index )
          .setImage( **image )
          .setSubresourceRange(
            vk::ImageSubresourceRange( f.subresourceRange )
              .setAspectMask( format_to_aspect( image->get_props().get_basic().format ) )
          )
      );
    }
    image->get_layout().set_layout(
      mip_base,
      mip_count,
      0u,
      image->get_props().get_basic().arrayLayers,
      to
    );
    (*get_factory())->pipelineBarrier(
      src_stage,
      dest_stage,
      vk::DependencyFlagBits( 0 ),
      {},
      {},
      raw_image
    );
    get_factory()->unbound()->keep.push_back( image );
    return raw_image;
  }
  std::vector< vk::ImageMemoryBarrier > command_buffer_recorder_t::convert_image(
    vk::AccessFlagBits src_access_mask,
    vk::AccessFlagBits dest_access_mask,
    vk::PipelineStageFlagBits src_stage,
    vk::PipelineStageFlagBits dest_stage,
    const std::shared_ptr< image_t > &image,
    vk::ImageLayout to
  ) {
    return convert_image(
      src_access_mask,
      dest_access_mask,
      src_stage,
      dest_stage,
      image,
      0,
      image->get_props().get_basic().mipLevels,
      0u,
      image->get_props().get_basic().arrayLayers,
      to
    );
  }
  std::vector< vk::ImageMemoryBarrier > command_buffer_recorder_t::convert_image(
    const std::shared_ptr< image_t > &image,
    std::uint32_t mip_base,
    std::uint32_t mip_count,
    std::uint32_t array_base,
    std::uint32_t array_count,
    vk::ImageLayout to
  ) {
    return convert_image(
      vk::AccessFlagBits::eTransferRead,
      vk::AccessFlagBits::eShaderRead,
      vk::PipelineStageFlagBits::eTransfer,
      vk::PipelineStageFlagBits::eFragmentShader,
      image,
      mip_base,
      mip_count,
      array_base,
      array_count,
      to
    );
  }
  std::vector< vk::ImageMemoryBarrier > command_buffer_recorder_t::convert_image(
    const std::shared_ptr< image_t > &image,
    vk::ImageLayout to
  ) {
    return convert_image(
      vk::AccessFlagBits::eTransferRead,
      vk::AccessFlagBits::eShaderRead,
      vk::PipelineStageFlagBits::eTransfer,
      vk::PipelineStageFlagBits::eFragmentShader,
      image,
      to
    );
  }
  std::vector< vk::ImageMemoryBarrier > command_buffer_recorder_t::revert_convert_image(
    vk::PipelineStageFlagBits src_stage,
    vk::PipelineStageFlagBits dest_stage,
    const std::shared_ptr< image_t > &image,
    const std::vector< vk::ImageMemoryBarrier > &old
  ) {
    std::vector< vk::ImageMemoryBarrier > raw_image;
    raw_image.reserve( old.size() );
    std::transform(
      old.begin(), old.end(),
      std::back_inserter( raw_image ),
      []( const auto &v ) {
        return vk::ImageMemoryBarrier( v )
          .setOldLayout( v.newLayout )
          .setNewLayout( v.oldLayout )
          .setSrcAccessMask( v.dstAccessMask )
          .setDstAccessMask( v.srcAccessMask )
          .setSrcQueueFamilyIndex( v.dstQueueFamilyIndex )
          .setDstQueueFamilyIndex( v.srcQueueFamilyIndex );
      }
    );
    (*get_factory())->pipelineBarrier(
      src_stage,
      dest_stage,
      vk::DependencyFlagBits( 0 ),
      {},
      {},
      raw_image
    );
    get_factory()->unbound()->keep.push_back( image );
    return raw_image;
  }
  std::vector< vk::ImageMemoryBarrier > command_buffer_recorder_t::revert_convert_image(
    const std::shared_ptr< image_t > &image,
    const std::vector< vk::ImageMemoryBarrier > &old
  ) {
    return revert_convert_image(
      vk::PipelineStageFlagBits::eTransfer,
      vk::PipelineStageFlagBits::eFragmentShader,
      image,
      old
    );
  }
  /*
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
  */
}
