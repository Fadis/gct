#ifndef GCT_TIMER_HPP
#define GCT_TIMER_HPP
#include <thread>
#include <chrono>

namespace gct {

class blocking_timer final {
public:
  blocking_timer(
    const std::chrono::high_resolution_clock::duration &d =
      std::chrono::microseconds( 16667 )
  ) :
    duration( d ),
    end_time( std::chrono::high_resolution_clock::now() + d ) {}
  ~blocking_timer() {
    const auto now = std::chrono::high_resolution_clock::now();
    while( end_time < now ) {
      end_time += duration;
    }
    std::this_thread::sleep_until( end_time );
  }
  blocking_timer( const blocking_timer& ) = delete;
  blocking_timer( blocking_timer&& ) = delete;
  blocking_timer &operator=( const blocking_timer& ) = delete;
  blocking_timer &operator=( blocking_timer&& ) = delete;
private:
  std::chrono::high_resolution_clock::duration duration;
  std::chrono::high_resolution_clock::time_point end_time;
};

}

#endif
