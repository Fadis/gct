#include <gct/swapchain.hpp>
#include <gct/image_create_info.hpp>
#include <gct/image_view_create_info.hpp>
#include <gct/format.hpp>
#include <gct/swapchain_image.hpp>

namespace gct {
  image_create_info_t to_image_create_info( const vk::SwapchainCreateInfoKHR &v ) {
    return image_create_info_t()
      .set_basic(
        vk::ImageCreateInfo()
          .setImageType( vk::ImageType::e2D )
          .setFormat( v.imageFormat )
          .setExtent(
            vk::Extent3D()
              .setWidth( v.imageExtent.width )
              .setHeight( v.imageExtent.height )
              .setDepth( 1 )
          )
          .setMipLevels( 1 )
          .setArrayLayers( v.imageArrayLayers )
          .setSamples( vk::SampleCountFlagBits::e1 )
          .setSharingMode( v.imageSharingMode )
          .setQueueFamilyIndexCount( v.queueFamilyIndexCount )
          .setPQueueFamilyIndices( v.pQueueFamilyIndices )
          .setInitialLayout( /*vk::ImageLayout::ePresentSrcKHR*/vk::ImageLayout::eUndefined )
      )
      .rebuild_chain();
  }
  swapchain_image_t::swapchain_image_t(
    const std::shared_ptr< swapchain_t > &swapchain,
    const vk::Image &image_
  ) :
    image_t( to_image_create_info( swapchain->get_props().get_basic() ) ),
    created_from< swapchain_t >( swapchain ),
    handle( image_ ) {}
  std::shared_ptr< image_view_t > swapchain_image_t::get_view(
    const image_view_create_info_t &create_info
  ) {
    return std::shared_ptr< image_view_t >(
      new image_view_t(
        shared_from_this(),
        create_info
      )
    );
  }
  std::shared_ptr< image_view_t > swapchain_image_t::get_view(
    vk::ImageAspectFlags aspect
  ) {
    return get_view(
      image_view_create_info_t()
        .set_basic(
          vk::ImageViewCreateInfo()
            .setSubresourceRange( vk::ImageSubresourceRange( aspect, 0, 1, 0, 1 ) )
            .setViewType( to_image_view_type( get_props().get_basic().imageType, get_props().get_basic().arrayLayers ) )
        )
        .rebuild_chain()
    );
  }
  std::shared_ptr< image_view_t > swapchain_image_t::get_view() {
    return get_view( format_to_aspect( get_props().get_basic().format ) );
  }
  std::shared_ptr< device_t > swapchain_image_t::get_device() const {
    return get_factory()->get_factory();
  }
}

