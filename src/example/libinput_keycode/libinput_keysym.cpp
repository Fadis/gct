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

namespace {
#ifdef GCT_SDBUSCPP_STRONG_TYPE
  using error_type = std::optional< sdbus::Error >;
#else
  using error_type = sdbus::Error*;
#endif
}

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
  std::string current_text;
  bool text_changed = false;
  std::mutex text_guard;
  thread_pool->add_co(
    [thread_pool=thread_pool,keyboard=keyboard,&current_text,&text_changed,&text_guard]() {
#ifdef GCT_SDBUSCPP_STRONG_TYPE
      auto dbus_locale = sdbus::createProxy( sdbus::ServiceName( "org.freedesktop.locale1" ), sdbus::ObjectPath( "/org/freedesktop/locale1" ) );
#else
      auto dbus_locale = sdbus::createProxy( "org.freedesktop.locale1", "/org/freedesktop/locale1" );
#endif
      auto [layout,model,variant,options] = wait(
        gct::dbus::call< error_type, sdbus::Variant >(
          dbus_locale->callMethodAsync( "Get" )
            .onInterface( "org.freedesktop.DBus.Properties" )
            .withArguments( "org.freedesktop.locale1", "X11Layout" )
        ) &
        gct::dbus::call< error_type, sdbus::Variant >(
          dbus_locale->callMethodAsync( "Get" )
            .onInterface( "org.freedesktop.DBus.Properties" )
            .withArguments( "org.freedesktop.locale1", "X11Model" )
        ) &
        gct::dbus::call< error_type, sdbus::Variant >(
          dbus_locale->callMethodAsync( "Get" )
            .onInterface( "org.freedesktop.DBus.Properties" )
            .withArguments( "org.freedesktop.locale1", "X11Variant" )
        ) &
        gct::dbus::call< error_type, sdbus::Variant >(
          dbus_locale->callMethodAsync( "Get" )
            .onInterface( "org.freedesktop.DBus.Properties" )
            .withArguments( "org.freedesktop.locale1", "X11Options" )
        )
      );
      const auto layout_str = layout.get< std::string >();
      const auto model_str = model.get< std::string >();
      const auto variant_str = variant.get< std::string >();
      const auto options_str = options.get< std::string >();
      xkb_rule_names names;
      names.rules = nullptr;
      names.model = model_str.c_str();
      names.layout = layout_str.c_str();
      names.variant = variant_str.c_str();
      names.options = options_str.c_str();
      std::shared_ptr< gct::input::xkb_state > xkb_state( new gct::input::xkb_state( names ) );
      thread_pool->add_co(
        [keyboard=keyboard,xkb_state=xkb_state]() {
          try {
            while( 1 ) {
              for( auto c: wait( keyboard->get_future() ) ) {
                xkb_state->key_event( c );
              }
            }
          }
          catch( ... ) {
            std::cout << "keyboard watcher exited." << std::endl;
          }
        }
      );
      thread_pool->add_co(
        [xkb_state=xkb_state,&current_text,&text_changed,&text_guard]() {
          try {
            while( 1 ) {
              for( auto c: wait( xkb_state->get_future() ) ) {
                if( c.state == gct::input::key_state::pressed ) {
                  if( 0x20 <= c.sym && c.sym <= 0x7F ) {
                    std::scoped_lock< std::mutex > lock( text_guard );
                    current_text += char( c.sym );
                    std::cout << current_text << std::endl;
                    text_changed = true;
                  }
                  else if( c.sym == 0xFF08 ) {
                    if( !current_text.empty() ) {
                      std::scoped_lock< std::mutex > lock( text_guard );
                      current_text.pop_back();
                      std::cout << current_text << std::endl;
                      text_changed = true;
                    }
                  }
                  else if( c.sym == 0xFF0d ) {
                    std::scoped_lock< std::mutex > lock( text_guard );
                    current_text += '\n';
                    std::cout << current_text << std::endl;
                    text_changed = true;
                  }
                }
              }
            }
          }
          catch( ... ) {
            std::cout << "keyboard watcher exited." << std::endl;
          }
        }
      );
    }
  );
  libinput->run();
  while( 1 ) {}
}

