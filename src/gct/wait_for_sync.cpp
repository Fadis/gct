#include <thread>
#include <gct/wait_for_sync.hpp>
namespace gct {
  void wait_for_sync( const std::chrono::high_resolution_clock::time_point &begin_time ) {
    const auto end_time = std::chrono::high_resolution_clock::now();
    const auto elapsed_time = end_time - begin_time;
    if( elapsed_time < std::chrono::microseconds( 16667 ) ) {
      const auto sleep_for = std::chrono::microseconds( 16667 ) - elapsed_time;
      std::this_thread::sleep_for( sleep_for );
    }
  }
}

