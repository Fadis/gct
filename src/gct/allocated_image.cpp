#include <gct/allocator.hpp>
#include <gct/image_create_info.hpp>
#include <gct/image_view_create_info.hpp>
#include <gct/format.hpp>
#include <gct/subview_range.hpp>
#include <gct/allocated_image.hpp>

namespace gct {
  allocated_image_t::allocated_image_t(
    const std::shared_ptr< allocator_t > &allocator,
    const image_create_info_t &create_info,
    VmaMemoryUsage usage
  ) :
    image_t( create_info ),
    created_from< allocator_t >( allocator ),
    allocation( new VmaAllocation() ) {
    VmaAllocationCreateInfo image_alloc_info = {};
    VkImageCreateInfo raw_image_create_info = static_cast< VkImageCreateInfo >( props.get_basic() );
    image_alloc_info.usage = usage;
    VkImage image_;
    const auto result = vmaCreateImage( **allocator, &raw_image_create_info, &image_alloc_info, &image_, allocation.get(), nullptr );
    if( result != VK_SUCCESS ) {
#if VK_HEADER_VERSION >= 256
      vk::detail::throwResultException( vk::Result( result ), "イメージを作成できない" );
#else
      vk::throwResultException( vk::Result( result ), "イメージを作成できない" );
#endif
    }
    handle.reset(
      new vk::Image( image_ ),
      [allocator=allocator,allocation=allocation]( vk::Image *p ) {
        if( p ) {
          vmaDestroyImage( **allocator, VkImage( *p ), *allocation );
          delete p;
        }
      }
    );
  }
  std::shared_ptr< image_view_t > allocated_image_t::get_view(
    const image_view_create_info_t &create_info
  ) {
    return std::shared_ptr< image_view_t >(
      new image_view_t(
        shared_from_this(),
        create_info
      )
    );
  }
  std::shared_ptr< image_view_t > allocated_image_t::get_view(
    vk::ImageAspectFlags aspect,
    bool force_array
  ) {
    return get_view(
      image_view_create_info_t()
        .set_basic(
          vk::ImageViewCreateInfo()
            .setSubresourceRange(
              vk::ImageSubresourceRange()
                .setAspectMask( aspect )
                .setBaseMipLevel( 0 )
                .setLevelCount( get_props().get_basic().mipLevels )
                .setBaseArrayLayer( 0 )
                .setLayerCount( get_props().get_basic().arrayLayers )
            )
            .setViewType( to_image_view_type( get_props().get_basic().imageType, get_props().get_basic().arrayLayers, force_array ) )
        )
        .rebuild_chain()
    );
  }
  std::shared_ptr< image_view_t > allocated_image_t::get_view( bool force_array ) {
    return get_view( format_to_aspect( get_props().get_basic().format ), force_array );
  }
  std::vector< std::shared_ptr< image_view_t > > allocated_image_t::get_thin_views(
    vk::ImageAspectFlags aspect,
    std::uint32_t layer,
    bool force_array
  ) {
    std::vector< std::shared_ptr< image_view_t > > temp;
    for( unsigned int l = 0u; l != get_props().get_basic().arrayLayers / layer; ++l ) {
      temp.push_back( get_view(
        image_view_create_info_t()
          .set_basic(
            vk::ImageViewCreateInfo()
              .setSubresourceRange(
                vk::ImageSubresourceRange()
                  .setAspectMask( aspect )
                  .setBaseMipLevel( 0 )
                  .setLevelCount( get_props().get_basic().mipLevels )
                  .setBaseArrayLayer( l * layer )
                  .setLayerCount( layer )
              )
              .setViewType( to_image_view_type( get_props().get_basic().imageType, layer, force_array ) )
          )
          .rebuild_chain()
      ) );
    }
    return temp;
  }
  std::shared_ptr< image_view_t > allocated_image_t::get_view(
    const subview_range &r
  ) {
    if( get_props().get_basic().mipLevels < r.mip_offset + r.mip_count ) {
      throw gct::exception::out_of_range( "allocated_image_t::get_view : Specified mipmap range is outside of available mipmap levels." );
    }
    if( get_props().get_basic().arrayLayers < r.layer_offset + r.layer_count ) {
      throw gct::exception::out_of_range( "allocated_image_t::get_view : Specified layer range is outside of available layers." );
    }
    return get_view(
      image_view_create_info_t()
        .set_basic(
          vk::ImageViewCreateInfo()
            .setSubresourceRange(
              vk::ImageSubresourceRange()
                .setAspectMask( format_to_aspect( get_props().get_basic().format ) )
                .setBaseMipLevel( r.mip_offset )
                .setLevelCount( r.mip_count )
                .setBaseArrayLayer( r.layer_offset )
                .setLayerCount( r.layer_count )
            )
            .setViewType( to_image_view_type( get_props().get_basic().imageType, r.layer_count, r.force_array ) )
        )
        .rebuild_chain()
    );
  }
  std::vector< std::shared_ptr< image_view_t > > allocated_image_t::get_thin_views(
    std::uint32_t layer,
    bool force_array
  ) {
    return get_thin_views( format_to_aspect( get_props().get_basic().format ), layer, force_array );
  }
  std::shared_ptr< device_t > allocated_image_t::get_device() const {
    return get_factory()->get_factory();
  }
}

