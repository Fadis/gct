#ifndef GCT_PRESENT_INFO_HPP
#define GCT_PRESENT_INFO_HPP
#include <memory>
#include <optional>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
#ifdef VK_KHR_SWAPCHAIN_EXTENSION_NAME
  class semaphore_t;
  class swapchain_t;
  class present_info_t : public chained_t {
  public:
    using self_type = present_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::PresentInfoKHR )
#if ( defined(VK_VERSION_1_1) && defined(VK_KHR_SWAPCHAIN_EXTENSION_NAME) ) || ( defined(VK_STRUCTURE_TYPE_DEVICE_GROUP_PRESENT_CAPABILITIES_KHR) && defined(VK_KHR_SWAPCHAIN_EXTENSION_NAME) ) 
    LIBGCT_EXTENSION_SETTER( vk::DeviceGroupPresentInfoKHR , device_group )
#endif
#ifdef VK_KHR_DISPLAY_SWAPCHAIN_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::DisplayPresentInfoKHR, display )
#endif
#ifdef VK_GGP_FRAME_TOKEN_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PresentFrameTokenGGP , frame_token )
#endif
#ifdef VK_KHR_PRESENT_ID_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PresentIdKHR , id )
#endif
#ifdef VK_KHR_INCREMENTAL_PRESENT_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PresentRegionsKHR , regions )
#endif
#ifdef VK_GOOGLE_DISPLAY_TIMING_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PresentTimesInfoGOOGLE , times )
#endif
  private:
    std::vector< std::shared_ptr< semaphore_t > > semaphore;
    std::vector< vk::Semaphore > raw_semaphore;
    std::vector< std::shared_ptr< swapchain_t > > swapchain;
    std::vector< vk::SwapchainKHR > raw_swapchain;
    std::vector< std::uint32_t > image_index;
  public:
    present_info_t &add_wait_for( const std::shared_ptr< semaphore_t >& );
    present_info_t &clear_wait_for();
    present_info_t &add_swapchain( const std::shared_ptr< swapchain_t >&, std::uint32_t );
    present_info_t &clear_swapchain();
  };
#endif
}

#endif

