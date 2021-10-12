#include <gct/semaphore.hpp>
#include <gct/submit_info.hpp>

namespace gct {
  submit_info_t &submit_info_t::rebuild_chain() {
    if( chained ) return *this;
    wait_raw.clear();
    wait_raw.reserve( wait.size() );
    std::transform(
      wait.begin(),
      wait.end(),
      std::back_inserter( wait_raw ),
      []( const auto &v ) { return **v; }
    );
    signal_raw.clear();
    signal_raw.reserve( signal.size() );
    std::transform(
      signal.begin(),
      signal.end(),
      std::back_inserter( signal_raw ),
      []( const auto &v ) { return **v; }
    );
    auto basic = get_basic();
    basic
      .setWaitSemaphoreCount( wait_raw.size() )
      .setPWaitSemaphores( wait_raw.data() )
      .setPWaitDstStageMask( stage.data() )
      .setSignalSemaphoreCount( signal_raw.size() )
      .setPSignalSemaphores( signal_raw.data() );
    set_basic( std::move( basic ) );
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( d3d12_fence )
#endif
#if defined(VK_VERSION1_1) || defined(VK_KHR_DEVICE_GROUP_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( device_group )
#endif
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( performance_query )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_REBUILD_CHAIN( protected_ )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( timeline_semaphore )
#endif
#ifdef VK_KHR_WIN32_KEYED_MUTEX_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( win32_keyed_mutex_acquire_release )
#endif
#ifdef VK_NV_WIN32_KEYED_MUTEX_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( win32_keyed_mutex_acquire_release_nv )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  submit_info_t &submit_info_t::add_wait_for(
    const std::shared_ptr< semaphore_t > &s,
    vk::PipelineStageFlags t
  ) {
    wait.push_back( s );
    stage.push_back( t );
    chained = false;
    return *this;
  }
  submit_info_t &submit_info_t::clear_wait_for() {
    wait.clear();
    stage.clear();
    chained = false;
    return *this;
  }
  submit_info_t &submit_info_t::add_signal_to( const std::shared_ptr< semaphore_t > &s ) {
    signal.push_back( s );
    chained = false;
    return *this;
  }
  submit_info_t &submit_info_t::clear_signal_to() {
    signal.clear();
    chained = false;
    return *this;
  }
}

