#ifndef GCT_WAIT_FOR_SYNC_HPP
#define GCT_WAIT_FOR_SYNC_HPP
#include <chrono>
namespace gct {
  void wait_for_sync( const std::chrono::high_resolution_clock::time_point &begin_time );
}
#endif

