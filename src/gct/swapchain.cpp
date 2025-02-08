#include <gct/swapchain_image.hpp>
#include <gct/device.hpp>
#include <gct/semaphore.hpp>
#include <gct/swapchain.hpp>

namespace gct {
#ifdef VK_KHR_SWAPCHAIN_EXTENSION_NAME
  swapchain_t::swapchain_t(
    const std::shared_ptr< device_t > &device,
    const swapchain_create_info_t &create_info
  ) :
    property_type( create_info ),
    created_from< device_t >( device ) {
    props.rebuild_chain();
    handle = (*device)->createSwapchainKHRUnique(
      props.get_basic()
    );
  }
  std::vector< std::shared_ptr< image_t > > swapchain_t::get_images() {
    std::vector< std::shared_ptr< image_t > > images;
    for( auto &swapchain_image: (*get_factory())->getSwapchainImagesKHR( *handle ) ) {
      std::shared_ptr< image_t > image(
        new swapchain_image_t( shared_from_this(), swapchain_image )
      );
      images.push_back( std::move( image ) );
    }
    return images;
  }
  std::uint32_t swapchain_t::acquire_next_image(
    const std::shared_ptr< semaphore_t > &semaphore
  ) {
    return (*get_factory())->acquireNextImageKHR(
      *handle,
      UINT64_MAX,
      **semaphore,
      vk::Fence()
    ).value;
  }
#endif
}

