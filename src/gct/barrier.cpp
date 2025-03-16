#include <gct/buffer.hpp>
#include <gct/image.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/format.hpp>
#include <gct/get_device.hpp>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace gct {
  std::vector< vk::ImageMemoryBarrier > command_buffer_recorder_t::barrier(
    vk::AccessFlags src_access_mask,
    vk::AccessFlags dest_access_mask,
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
#ifdef VK_API_VERSION_1_3
    const bool synchronization2_is_activated = api_version >= VK_API_VERSION_1_3 || exts.find( "VK_KHR_synchronization2" ) != exts.end();
#else
    const bool synchronization2_is_activated = exts.find( "VK_KHR_synchronization2" ) != exts.end();
#endif
    for( auto &v: image ) {
      // This causes old (older than 1.3.296?) validation layer fails to track image layout randomly.
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
  void command_buffer_recorder_t::barrier(
    vk::AccessFlags src_access_mask,
    vk::AccessFlags dest_access_mask,
    vk::PipelineStageFlagBits src_stage,
    vk::PipelineStageFlagBits dest_stage,
    vk::DependencyFlagBits dependency,
    const syncable &s
  ) {
    const auto available_queue_family_index = get_factory()->get_factory()->get_factory()->get_available_queue_family_index();
    std::vector< vk::BufferMemoryBarrier > raw_buffer;
    std::transform(
      s.buffer.begin(),
      s.buffer.end(),
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
#ifdef VK_API_VERSION_1_3
    const bool synchronization2_is_activated = api_version >= VK_API_VERSION_1_3 || exts.find( "VK_KHR_synchronization2" ) != exts.end();
#else
    const bool synchronization2_is_activated = exts.find( "VK_KHR_synchronization2" ) != exts.end();
#endif
    for( auto &v: s.image ) {
      // This causes old (older than 1.3.296?) validation layer fails to track image layout randomly.
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
    for( auto &v: s.image_view ) {
      // This causes old (older than 1.3.296?) validation layer fails to track image layout randomly.
      if( synchronization2_is_activated ) {
        raw_image.push_back(
          vk::ImageMemoryBarrier()
            .setSrcAccessMask( src_access_mask )
            .setDstAccessMask( dest_access_mask )
            .setOldLayout( vk::ImageLayout::eUndefined )
            .setNewLayout( vk::ImageLayout::eUndefined )
            .setSrcQueueFamilyIndex( available_queue_family_index )
            .setDstQueueFamilyIndex( available_queue_family_index )
            .setImage( **v->get_factory() )
            .setSubresourceRange(
              vk::ImageSubresourceRange()
                .setAspectMask( format_to_aspect( v->get_factory()->get_props().get_basic().format ) )
                .setBaseMipLevel( v->get_props().get_basic().subresourceRange.baseMipLevel )
                .setLevelCount( v->get_props().get_basic().subresourceRange.levelCount )
                .setBaseArrayLayer( v->get_props().get_basic().subresourceRange.baseArrayLayer )
                .setLayerCount( v->get_props().get_basic().subresourceRange.layerCount )
            )
        );
      }
      else {
        for( auto &f: v->get_factory()->get_layout().get_layout() ) {
          raw_image.push_back(
            vk::ImageMemoryBarrier( f )
              .setSrcAccessMask( src_access_mask )
              .setDstAccessMask( dest_access_mask )
              .setSrcQueueFamilyIndex( available_queue_family_index )
              .setDstQueueFamilyIndex( available_queue_family_index )
              .setImage( **v->get_factory() )
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
    get_factory()->unbound()->keep.push_back( s.buffer );
    get_factory()->unbound()->keep.push_back( s.image );
    get_factory()->unbound()->keep.push_back( s.image_view );
  }
  std::vector< vk::ImageMemoryBarrier > command_buffer_recorder_t::barrier(
    const std::vector< std::shared_ptr< buffer_t > > &buffer,
    const std::vector< std::shared_ptr< image_t > > &image
  ) {
    return barrier(
      vk::AccessFlagBits::eMemoryRead | vk::AccessFlagBits::eMemoryWrite,
      vk::AccessFlagBits::eMemoryRead | vk::AccessFlagBits::eMemoryWrite,
      vk::PipelineStageFlagBits::eAllCommands,
      vk::PipelineStageFlagBits::eAllCommands,
      vk::DependencyFlagBits( 0 ),
      buffer,
      image
    );
  }
  void command_buffer_recorder_t::barrier(
    const syncable &s
  ) {
    barrier(
      vk::AccessFlagBits::eMemoryRead | vk::AccessFlagBits::eMemoryWrite,
      vk::AccessFlagBits::eMemoryRead | vk::AccessFlagBits::eMemoryWrite,
      vk::PipelineStageFlagBits::eAllCommands,
      vk::PipelineStageFlagBits::eAllCommands,
      vk::DependencyFlagBits( 0 ),
      s
    );
  }
  std::vector< vk::ImageMemoryBarrier > command_buffer_recorder_t::compute_barrier(
    const std::vector< std::shared_ptr< buffer_t > > &buffer,
    const std::vector< std::shared_ptr< image_t > > &image
  ) {
    return barrier(
      vk::AccessFlagBits::eShaderWrite,
      vk::AccessFlagBits::eShaderRead,
      vk::PipelineStageFlagBits::eComputeShader,
      vk::PipelineStageFlagBits::eComputeShader,
      vk::DependencyFlagBits( 0 ),
      buffer,
      image
    );
  }
  void command_buffer_recorder_t::compute_barrier(
    const syncable &s
  ) {
    barrier(
      vk::AccessFlagBits::eShaderWrite,
      vk::AccessFlagBits::eShaderRead,
      vk::PipelineStageFlagBits::eComputeShader,
      vk::PipelineStageFlagBits::eComputeShader,
      vk::DependencyFlagBits( 0 ),
      s
    );
  }
  std::vector< vk::ImageMemoryBarrier > command_buffer_recorder_t::compute_write_barrier(
    const std::vector< std::shared_ptr< buffer_t > > &buffer,
    const std::vector< std::shared_ptr< image_t > > &image
  ) {
    return barrier(
      vk::AccessFlagBits::eShaderRead,
      vk::AccessFlagBits::eShaderWrite,
      vk::PipelineStageFlagBits::eComputeShader,
      vk::PipelineStageFlagBits::eComputeShader,
      vk::DependencyFlagBits( 0 ),
      buffer,
      image
    );
  }
  void command_buffer_recorder_t::compute_write_barrier(
    const syncable &s
  ) {
    barrier(
      vk::AccessFlagBits::eShaderRead,
      vk::AccessFlagBits::eShaderWrite,
      vk::PipelineStageFlagBits::eComputeShader,
      vk::PipelineStageFlagBits::eComputeShader,
      vk::DependencyFlagBits( 0 ),
      s
    );
  }
  std::vector< vk::ImageMemoryBarrier > command_buffer_recorder_t::transfer_barrier(
    const std::vector< std::shared_ptr< buffer_t > > &buffer,
    const std::vector< std::shared_ptr< image_t > > &image
  ) {
    return barrier(
      vk::AccessFlagBits::eTransferWrite,
      vk::AccessFlagBits::eTransferRead,
      vk::PipelineStageFlagBits::eTransfer,
      vk::PipelineStageFlagBits::eTransfer,
      vk::DependencyFlagBits( 0 ),
      buffer,
      image
    );
  }
  void command_buffer_recorder_t::transfer_barrier(
    const syncable &s
  ) {
    barrier(
      vk::AccessFlagBits::eTransferWrite,
      vk::AccessFlagBits::eTransferRead,
      vk::PipelineStageFlagBits::eTransfer,
      vk::PipelineStageFlagBits::eTransfer,
      vk::DependencyFlagBits( 0 ),
      s
    );
  }
  std::vector< vk::ImageMemoryBarrier > command_buffer_recorder_t::compute_to_transfer_barrier(
    const std::vector< std::shared_ptr< buffer_t > > &buffer,
    const std::vector< std::shared_ptr< image_t > > &image
  ) {
    return barrier(
      vk::AccessFlagBits::eShaderWrite,
      vk::AccessFlagBits::eTransferRead,
      vk::PipelineStageFlagBits::eComputeShader,
      vk::PipelineStageFlagBits::eTransfer,
      vk::DependencyFlagBits( 0 ),
      buffer,
      image
    );
  }
  void command_buffer_recorder_t::compute_to_transfer_barrier(
    const syncable &s
  ) {
    barrier(
      vk::AccessFlagBits::eShaderWrite,
      vk::AccessFlagBits::eTransferRead,
      vk::PipelineStageFlagBits::eComputeShader,
      vk::PipelineStageFlagBits::eTransfer,
      vk::DependencyFlagBits( 0 ),
      s
    );
  }
  std::vector< vk::ImageMemoryBarrier > command_buffer_recorder_t::transfer_to_compute_barrier(
    const std::vector< std::shared_ptr< buffer_t > > &buffer,
    const std::vector< std::shared_ptr< image_t > > &image
  ) {
    return barrier(
      vk::AccessFlagBits::eTransferWrite,
      vk::AccessFlagBits::eShaderRead,
      vk::PipelineStageFlagBits::eTransfer,
      vk::PipelineStageFlagBits::eComputeShader,
      vk::DependencyFlagBits( 0 ),
      buffer,
      image
    );
  }
  void command_buffer_recorder_t::transfer_to_compute_barrier(
    const syncable &s
  ) {
    barrier(
      vk::AccessFlagBits::eTransferWrite,
      vk::AccessFlagBits::eShaderRead,
      vk::PipelineStageFlagBits::eTransfer,
      vk::PipelineStageFlagBits::eComputeShader,
      vk::DependencyFlagBits( 0 ),
      s
    );
  }
  std::vector< vk::ImageMemoryBarrier > command_buffer_recorder_t::transfer_to_graphics_barrier(
    const std::vector< std::shared_ptr< buffer_t > > &buffer,
    const std::vector< std::shared_ptr< image_t > > &image
  ) {
    return barrier(
      vk::AccessFlagBits::eTransferWrite,
      vk::AccessFlagBits::eShaderRead,
      vk::PipelineStageFlagBits::eTransfer,
      vk::PipelineStageFlagBits::eVertexShader,
      vk::DependencyFlagBits( 0 ),
      buffer,
      image
    );
  }
  void command_buffer_recorder_t::transfer_to_graphics_barrier(
    const syncable &s
  ) {
    barrier(
      vk::AccessFlagBits::eTransferWrite,
      vk::AccessFlagBits::eShaderRead,
      vk::PipelineStageFlagBits::eTransfer,
      vk::PipelineStageFlagBits::eVertexShader,
      vk::DependencyFlagBits( 0 ),
      s
    );
  }
  std::vector< vk::ImageMemoryBarrier > command_buffer_recorder_t::graphics_to_transfer_barrier(
    const std::vector< std::shared_ptr< buffer_t > > &buffer,
    const std::vector< std::shared_ptr< image_t > > &image
  ) {
    return barrier(
      vk::AccessFlagBits::eColorAttachmentWrite,
      vk::AccessFlagBits::eTransferRead,
      vk::PipelineStageFlagBits::eColorAttachmentOutput,
      vk::PipelineStageFlagBits::eTransfer,
      vk::DependencyFlagBits( 0 ),
      buffer,
      image
    );
  }
  void command_buffer_recorder_t::graphics_to_transfer_barrier(
    const syncable &s
  ) {
    barrier(
      vk::AccessFlagBits::eColorAttachmentWrite,
      vk::AccessFlagBits::eTransferRead,
      vk::PipelineStageFlagBits::eColorAttachmentOutput,
      vk::PipelineStageFlagBits::eTransfer,
      vk::DependencyFlagBits( 0 ),
      s
    );
  }
  std::vector< vk::ImageMemoryBarrier > command_buffer_recorder_t::compute_to_graphics_barrier(
    const std::vector< std::shared_ptr< buffer_t > > &buffer,
    const std::vector< std::shared_ptr< image_t > > &image
  ) {
    return barrier(
      vk::AccessFlagBits::eShaderWrite,
      vk::AccessFlagBits::eShaderRead,
      vk::PipelineStageFlagBits::eComputeShader,
      vk::PipelineStageFlagBits::eVertexShader,
      vk::DependencyFlagBits( 0 ),
      buffer,
      image
    );
  }
  void command_buffer_recorder_t::compute_to_graphics_barrier(
    const syncable &s
  ) {
    barrier(
      vk::AccessFlagBits::eShaderWrite,
      vk::AccessFlagBits::eShaderRead,
      vk::PipelineStageFlagBits::eComputeShader,
      vk::PipelineStageFlagBits::eVertexShader,
      vk::DependencyFlagBits( 0 ),
      s
    );
  }
  std::vector< vk::ImageMemoryBarrier > command_buffer_recorder_t::graphics_to_compute_barrier(
    const std::vector< std::shared_ptr< buffer_t > > &buffer,
    const std::vector< std::shared_ptr< image_t > > &image
  ) {
    return barrier(
      vk::AccessFlagBits::eColorAttachmentWrite,
      vk::AccessFlagBits::eShaderRead,
      vk::PipelineStageFlagBits::eColorAttachmentOutput,
      vk::PipelineStageFlagBits::eComputeShader,
      vk::DependencyFlagBits( 0 ),
      buffer,
      image
    );
  }
  void command_buffer_recorder_t::graphics_to_compute_barrier(
    const syncable &s
  ) {
    barrier(
      vk::AccessFlagBits::eColorAttachmentWrite,
      vk::AccessFlagBits::eShaderRead,
      vk::PipelineStageFlagBits::eColorAttachmentOutput,
      vk::PipelineStageFlagBits::eComputeShader,
      vk::DependencyFlagBits( 0 ),
      s
    );
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
    }
    for(
      auto &f:
      image->get_layout().set_layout(
        mip_base,
        mip_count,
        array_base,
        array_count,
        to
      )
    ) {
      raw_image.push_back(
        vk::ImageMemoryBarrier( f )
          //.setNewLayout( to )
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
      vk::AccessFlagBits( 0x1FFFF ),
      vk::AccessFlagBits( 0x1FFFF ),
      vk::PipelineStageFlagBits::eAllCommands,
      vk::PipelineStageFlagBits::eAllCommands,
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
      vk::AccessFlagBits( 0x1FFFF ),
      vk::AccessFlagBits( 0x1FFFF ),
      vk::PipelineStageFlagBits::eAllCommands,
      vk::PipelineStageFlagBits::eAllCommands,
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
    for( const auto &v: old ) {
      if(
        v.oldLayout != vk::ImageLayout::eUndefined &&
        v.oldLayout != vk::ImageLayout::ePreinitialized
      ) {
        raw_image.push_back(
          vk::ImageMemoryBarrier( v )
            .setOldLayout( v.newLayout )
            .setNewLayout( v.oldLayout )
            .setSrcAccessMask( v.dstAccessMask )
            .setDstAccessMask( v.srcAccessMask )
            .setSrcQueueFamilyIndex( v.dstQueueFamilyIndex )
            .setDstQueueFamilyIndex( v.srcQueueFamilyIndex )
        );
        image->get_layout().set_layout(
          v.subresourceRange.baseMipLevel,
          v.subresourceRange.baseMipLevel + v.subresourceRange.levelCount,
          v.subresourceRange.baseArrayLayer,
          v.subresourceRange.baseArrayLayer + v.subresourceRange.layerCount,
          v.oldLayout
        );
      }
    }
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
      vk::PipelineStageFlagBits::eAllCommands,
      vk::PipelineStageFlagBits::eAllCommands,
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
