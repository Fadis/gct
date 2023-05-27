
#include <iostream>
#include <limits>
#include <memory>
#include <exception>
#include <mutex>
#include <utility>
#include <cassert>
#include <boost/circular_buffer.hpp>

#include <libudev.h>
#include <libinput.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <gct/future.hpp>
#include <gct/non_copyable_function.hpp>
#include <gct/sched/thread_pool.hpp>
#include <gct/sched/epoll_notifier.hpp>
#include <gct/sched/wait.hpp>
#include <gct/input/key_state.hpp>
#include <gct/input/libinput.hpp>

namespace gct::input {

void libinput_event_deleter::operator()( libinput_event *p ) const {
  libinput_event_destroy( p );
}
void libinput_event_deleter::operator()( libinput_event_keyboard *p ) const {
  libinput_event_destroy( libinput_event_keyboard_get_base_event( p ) );
}
void libinput_event_deleter::operator()( libinput_event_touch *p ) const {
  libinput_event_destroy( libinput_event_touch_get_base_event( p ) );
}
void libinput_event_deleter::operator()( libinput_event_gesture *p ) const {
  libinput_event_destroy( libinput_event_gesture_get_base_event( p ) );
}
void libinput_event_deleter::operator()( libinput_event_tablet_tool *p ) const {
  libinput_event_destroy( libinput_event_tablet_tool_get_base_event( p ) );
}
void libinput_event_deleter::operator()( libinput_event_tablet_pad *p ) const {
  libinput_event_destroy( libinput_event_tablet_pad_get_base_event( p ) );
}
void libinput_event_deleter::operator()( libinput_event_switch *p ) const {
  libinput_event_destroy( libinput_event_switch_get_base_event( p ) );
}
int libinput_t::open_restricted(const char *path, int flags, void *user_data) {
  int fd = open(path, flags);
  return fd < 0 ? -errno : fd;
}
void libinput_t::close_restricted(int fd, void *user_data) {
  close(fd);
}
libinput_t::libinput_internal_t::libinput_internal_t(
  const std::shared_ptr< sched::epoll_notifier_t > &e,
  const std::shared_ptr< sched::thread_pool_t > &p
) : end( false ), epoll_notifier( e ), thread_pool( p ) {
  set_fd = eventfd( 0, 0 );
}
void libinput_t::libinput_internal_t::run() {
  thread_pool->add_co(
    [self=shared_from_this()]() {
      self->poll();
    }, true, 2, 0
  );
}
libinput_t::libinput_internal_t::~libinput_internal_t() {
  ::close( set_fd );
}
void libinput_t::libinput_internal_t::set_end() {
  end = true;
  std::uint64_t temp = 1u;
  if( write( set_fd, &temp, sizeof( temp ) ) < 0 ) {
    throw std::system_error( std::make_error_code( std::errc( errno ) ), "libinput_t::set : event notification failed." );
  }
}
void libinput_t::libinput_internal_t::del( listeners_t &fifo, std::uint64_t id ) {
  std::scoped_lock< std::mutex > lock( fifo.guard );
  fifo.listener.erase( id );
}
void libinput_t::libinput_internal_t::push( listeners_t &fifo, std::unique_ptr< libinput_event, libinput_event_deleter > &&event ) {
  std::scoped_lock< std::mutex > lock( fifo.guard );
  for( auto &[id,t]: fifo.listener ) {
    if( event ) {
      t.cb( event.get(), std::exception_ptr() );
    }
  }
}
void libinput_t::libinput_internal_t::del_all( listeners_t &fifo ) {
  {
    std::scoped_lock< std::mutex > lock( fifo.guard );
    fifo.listener.clear();
  }
}
void libinput_t::libinput_internal_t::del_all() {
  del_all( device );
  del_all( keyboard );
  del_all( pointer );
  del_all( touch );
  del_all( tablet_tool );
  del_all( tablet_pad );
  del_all( gesture );
  del_all( switch_ );
}
void libinput_t::libinput_internal_t::poll() {
  auto udev = udev_new();
  libinput_interface interface;
  interface.open_restricted = open_restricted;
  interface.close_restricted = close_restricted;
  auto li = libinput_udev_create_context(&interface, nullptr, udev);
  if( li == nullptr ) {
    std::abort();
  }
  if( libinput_udev_assign_seat(li, "seat0") != 0 ) {
    std::abort();
  }
  auto libinput_fd = libinput_get_fd( li );
  while( 1 ) {
    int e = 0u;
    if( ( e = libinput_dispatch( li ) ) != 0 ) {
      throw std::system_error( std::make_error_code( std::errc( -e ) ), "libinput_t::poll : initial libinput_dispatch failed." );
    }
    libinput_event *raw_event = nullptr;
    while ((raw_event = libinput_get_event( li )) != nullptr) {
      std::unique_ptr< libinput_event, libinput_event_deleter > event( raw_event );
      const auto type = libinput_event_get_type( raw_event );
      if( type == LIBINPUT_EVENT_DEVICE_ADDED ) {
        push( device, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_DEVICE_REMOVED ) {
        push( device, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_KEYBOARD_KEY ) {
        push( keyboard, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_POINTER_MOTION ) {
        push( pointer, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_POINTER_MOTION_ABSOLUTE ) {
        push( pointer, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_POINTER_BUTTON ) {
        push( pointer, std::move( event ) );
      }
#ifndef GCT_LIBINPUT_NO_POINTER_WHEEL
      else if( type == LIBINPUT_EVENT_POINTER_AXIS ) {
        push( pointer, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_POINTER_SCROLL_WHEEL ) {
        push( pointer, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_POINTER_SCROLL_FINGER ) {
        push( pointer, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_POINTER_SCROLL_CONTINUOUS ) {
        push( pointer, std::move( event ) );
      }
#endif
      else if( type == LIBINPUT_EVENT_TOUCH_DOWN ) {
        push( touch, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_TOUCH_UP ) {
        push( touch, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_TOUCH_MOTION ) {
        push( touch, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_TOUCH_CANCEL ) {
        push( touch, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_TOUCH_FRAME ) {
        push( touch, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_TABLET_TOOL_AXIS ) {
        push( tablet_tool, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_TABLET_TOOL_PROXIMITY ) {
        push( tablet_tool, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_TABLET_TOOL_TIP ) {
        push( tablet_tool, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_TABLET_TOOL_BUTTON ) {
        push( tablet_tool, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_TABLET_PAD_BUTTON ) {
        push( tablet_pad, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_TABLET_PAD_RING ) {
        push( tablet_pad, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_TABLET_PAD_STRIP ) {
        push( tablet_pad, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_TABLET_PAD_KEY ) {
        push( tablet_pad, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_GESTURE_SWIPE_BEGIN ) {
        push( gesture, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_GESTURE_SWIPE_UPDATE ) {
        push( gesture, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_GESTURE_SWIPE_END ) {
        push( gesture, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_GESTURE_PINCH_BEGIN ) {
        push( gesture, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_GESTURE_PINCH_UPDATE ) {
        push( gesture, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_GESTURE_PINCH_END ) {
        push( gesture, std::move( event ) );
      }
#ifndef GCT_LIBINPUT_NO_GESTURE_HOLD
      else if( type == LIBINPUT_EVENT_GESTURE_HOLD_BEGIN ) {
        push( gesture, std::move( event ) );
      }
      else if( type == LIBINPUT_EVENT_GESTURE_HOLD_END ) {
        push( gesture, std::move( event ) );
      }
#endif
      else if( type == LIBINPUT_EVENT_SWITCH_TOGGLE ) {
        push( switch_, std::move( event ) );
      }
      if( ( e = libinput_dispatch( li ) ) != 0 ) {
        throw std::system_error( std::make_error_code( std::errc( -e ) ), "libinput_t::poll : initial libinput_dispatch failed." );
      }
    }
    try {
      auto event = sched::wait( epoll_notifier->epoll( EPOLLIN, libinput_fd ) | epoll_notifier->epoll( EPOLLIN, set_fd ) );
      if( event.data.fd == libinput_fd ) {
      }
      else if( event.data.fd == set_fd ) {
        std::uint64_t temp;
        if( read( set_fd, &temp, sizeof( temp ) ) < 0 ) {
          throw std::system_error( std::make_error_code( std::errc( errno ) ), "libinput_t::call : broken event notifier." );
        }
        if( !end ) {
        }
        else {
          break;
        }
      }
    }
    catch( ... ) {
    }
  }
  del_all();
  libinput_unref(li);
  udev_unref( udev );
}
libinput_t::libinput_t(
  const std::shared_ptr< sched::epoll_notifier_t > &e,
  const std::shared_ptr< sched::thread_pool_t > &p
) : internal( new libinput_internal_t( e, p ) ) {
  raw = internal.get();
}
void libinput_t::run() {
  if( ++executed == 1u ) {
    raw->run();
  }
}
libinput_t::~libinput_t() {
  raw->set_end();
  /*while( internal.use_count() >= 2u ) {
    sleep( 1 );
  }*/
}

key_state libinput_key_state_to_gct_key_state( libinput_key_state s ) {
  if( s == libinput_key_state::LIBINPUT_KEY_STATE_RELEASED ) return key_state::released;
  else if( s == libinput_key_state::LIBINPUT_KEY_STATE_PRESSED ) return key_state::pressed;
  assert( s == LIBINPUT_KEY_STATE_RELEASED || s == LIBINPUT_KEY_STATE_PRESSED );
  return key_state::released;
}
pointer_event libinput_pointer_event_to_gct_pointer_event( libinput_event_pointer *e ) {
  const auto base = libinput_event_pointer_get_base_event( e );
  const auto type = libinput_event_get_type( base );
  const auto time = std::chrono::microseconds(
    libinput_event_pointer_get_time_usec( e )
  );
  if( type == ::libinput_event_type::LIBINPUT_EVENT_POINTER_MOTION ) {
    return pointer_event {
      time,
      pointer_moved{
        libinput_event_pointer_get_dx( e ),
        libinput_event_pointer_get_dy( e )
      }
    };
  }
  else if( type == ::libinput_event_type::LIBINPUT_EVENT_POINTER_BUTTON ) {
    return pointer_event {
      time,
      pointer_button{
        libinput_event_pointer_get_button( e ),
        ( libinput_event_pointer_get_button_state( e ) == libinput_button_state::LIBINPUT_BUTTON_STATE_RELEASED ) ?
        key_state::released:
        key_state::pressed
      }
    };
  }
#ifndef GCT_LIBINPUT_NO_POINTER_WHEEL
  else if( type == ::libinput_event_type::LIBINPUT_EVENT_POINTER_SCROLL_WHEEL ) {
    return pointer_event {
      time,
      pointer_scroll_wheel{
        libinput_event_pointer_get_axis_value(
          e, libinput_pointer_axis::LIBINPUT_POINTER_AXIS_SCROLL_HORIZONTAL
        ),
        libinput_event_pointer_get_axis_value(
          e, libinput_pointer_axis::LIBINPUT_POINTER_AXIS_SCROLL_VERTICAL
        )
      }
    };
  }
  else if( type == ::libinput_event_type::LIBINPUT_EVENT_POINTER_SCROLL_FINGER ) {
    return pointer_event {
      time,
      pointer_scroll_finger{
        libinput_event_pointer_get_axis_value(
          e, libinput_pointer_axis::LIBINPUT_POINTER_AXIS_SCROLL_HORIZONTAL
        ),
        libinput_event_pointer_get_axis_value(
          e, libinput_pointer_axis::LIBINPUT_POINTER_AXIS_SCROLL_VERTICAL
        )
      }
    };
  }
  else if( type == ::libinput_event_type::LIBINPUT_EVENT_POINTER_SCROLL_CONTINUOUS ) {
    return pointer_event {
      time,
      pointer_scroll_continuous{
        libinput_event_pointer_get_axis_value(
          e, libinput_pointer_axis::LIBINPUT_POINTER_AXIS_SCROLL_HORIZONTAL
        ),
        libinput_event_pointer_get_axis_value(
          e, libinput_pointer_axis::LIBINPUT_POINTER_AXIS_SCROLL_VERTICAL
        )
      }
    };
  }
#endif
  else throw -1;

}

}

