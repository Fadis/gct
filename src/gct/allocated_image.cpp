#include <gct/allocator.hpp>
#include <gct/image_create_info.hpp>
#include <gct/image_view_create_info.hpp>
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
    VkImageCreateInfo raw_image_create_info = props.get_basic();
    image_alloc_info.usage = usage;
    VkImage image_;
    const auto result = vmaCreateImage( **allocator, &raw_image_create_info, &image_alloc_info, &image_, allocation.get(), nullptr );
    if( result != VK_SUCCESS ) vk::throwResultException( vk::Result( result ), "イメージを作成できない" );
    handle.reset(
      new vk::Image( image_ ),
      [allocator=allocator,allocation=allocation]( vk::Image *p ) {
        if( p ) {
          vmaDestroyImage( **allocator, *p, *allocation );
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
    vk::ImageAspectFlags aspect
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
            .setViewType( to_image_view_type( get_props().get_basic().imageType ) )
        )
        .rebuild_chain()
    );
  }
  std::shared_ptr< device_t > allocated_image_t::get_device() const {
    return get_factory()->get_factory();
  }
}

