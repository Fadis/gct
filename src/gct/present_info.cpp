#include <fstream>
#include <iterator>
#include <gct/semaphore.hpp>
#include <gct/swapchain.hpp>
#include <gct/present_info.hpp>

namespace gct {
  present_info_t &present_info_t::rebuild_chain() {
    if( chained ) return *this;
    raw_semaphore.clear();
    raw_semaphore.reserve( semaphore.size() );
    std::transform(
      semaphore.begin(),
      semaphore.end(),
      std::back_inserter( raw_semaphore ),
      []( const auto &v ) { return **v; }
    );
    raw_swapchain.clear();
    raw_swapchain.reserve( swapchain.size() );
    std::transform(
      swapchain.begin(),
      swapchain.end(),
      std::back_inserter( raw_swapchain ),
      []( const auto &v ) { return **v; }
    );
    basic
      .setWaitSemaphoreCount( raw_semaphore.size() )
      .setPWaitSemaphores( raw_semaphore.data() )
      .setSwapchainCount( raw_swapchain.size() )
      .setPSwapchains( raw_swapchain.data() )
      .setPImageIndices( image_index.data() );
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#if ( defined(VK_VERSION_1_1) && defined(VK_KHR_SWAPCHAIN_EXTENSION_NAME) ) || ( defined(VK_STRUCTURE_TYPE_DEVICE_GROUP_PRESENT_CAPABILITIES_KHR) && defined(VK_KHR_SWAPCHAIN_EXTENSION_NAME) ) 
    LIBGCT_EXTENSION_REBUILD_CHAIN( device_group )
#endif
#ifdef VK_KHR_DISPLAY_SWAPCHAIN_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( display )
#endif
#ifdef VK_GGP_FRAME_TOKEN_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( frame_token )
#endif
#ifdef VK_KHR_PRESENT_ID_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( id )
#endif
#ifdef VK_KHR_INCREMENTAL_PRESENT_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( regions )
#endif
#ifdef VK_GOOGLE_DISPLAY_TIMING_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( times )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  present_info_t &present_info_t::add_wait_for( const std::shared_ptr< semaphore_t > &v ) {
    semaphore.push_back( v );
    chained = false;
    return *this;
  }
  present_info_t &present_info_t::clear_wait_for() {
    semaphore.clear();
    chained = false;
    return *this;
  }
  present_info_t &present_info_t::add_swapchain( const std::shared_ptr< swapchain_t > &v, std::uint32_t i ) {
    swapchain.push_back( v );
    image_index.push_back( i );
    chained = false;
    return *this;
  }
  present_info_t &present_info_t::clear_swapchain() {
    swapchain.clear();
    image_index.clear();
    chained = false;
    return *this;
  }
}

