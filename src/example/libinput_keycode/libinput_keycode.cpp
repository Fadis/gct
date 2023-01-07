#include <algorithm>
#include <chrono>
#include <cmath>
#include <concepts>
#include <exception>
#include <memory>
#include <array>
#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include <thread>
#include <coroutine>
#include <functional>
#include <random>
#include <condition_variable>
#include <mutex>
#include <system_error>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <boost/asio.hpp>
#include <boost/coroutine2/all.hpp>
#include <boost/lockfree/queue.hpp>
#include <libudev.h>
#include <libinput.h>
#include <sdbus-c++/sdbus-c++.h>
#include <gct/future.hpp>
#include <gct/sched/thread_pool.hpp>
#include <gct/sched/epoll_notifier.hpp>
#include <gct/sched/sleep.hpp>
#include <gct/sched/wait.hpp>
#include <gct/dbus/dbus.hpp>
#include <gct/input/libinput.hpp>
#include <gct/input/input_buffer.hpp>
#include <gct/input/device_monitor.hpp>
#include <gct/input/xkbcommon.hpp>
#include <gct/input/fcitx5.hpp>

int main() {
  using namespace gct::sched;
  std::shared_ptr< epoll_notifier_t > epoll_notifier( new epoll_notifier_t() );
  std::shared_ptr< thread_pool_t > thread_pool( new thread_pool_t() );
  std::shared_ptr< gct::input::libinput_t > libinput( new gct::input::libinput_t( epoll_notifier, thread_pool ) );

  std::shared_ptr< gct::input::input_buffer< gct::input::libinput_event_category::keyboard_event, gct::input::xkb_key_event > > keyboard( new gct::input::input_buffer< gct::input::libinput_event_category::keyboard_event, gct::input::xkb_key_event >(
      libinput,
      []( auto &dest, auto *event ) {
        const auto key = libinput_event_keyboard_get_key( event );
        const auto time = std::chrono::microseconds(
          libinput_event_keyboard_get_time_usec( event )
        );
        const auto state = gct::input::libinput_key_state_to_gct_key_state(
          libinput_event_keyboard_get_key_state( event )
        );
        dest.push_back( gct::input::xkb_key_event{ 0u, key + 8u, state, time } );
      }
    )
  );
  thread_pool->add_co(
    [thread_pool=thread_pool,keyboard=keyboard]() {
      while( 1 ) {
        for( auto c: wait( keyboard->get_future() ) ) {
          std::cout << c.code;
          if( c.state == gct::input::key_state::released )
            std::cout << " released" << std::endl;
          else
            std::cout << " pressed" << std::endl;
        }
      }
    }
  );
  libinput->run();
  while( 1 ) {}
}

