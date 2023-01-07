#ifndef GCT_INPUT_LIBINPUT_HPP
#define GCT_INPUT_LIBINPUT_HPP

#include <memory>
#include <exception>
#include <mutex>
#include <utility>

#include <libudev.h>
#include <libinput.h>

#include <gct/non_copyable_function.hpp>
#include <gct/sched/thread_pool.hpp>
#include <gct/sched/epoll_notifier.hpp>
#include <gct/input/key_state.hpp>

namespace gct::input {

struct libinput_event_deleter {
  void operator()( libinput_event *p ) const;
  void operator()( libinput_event_keyboard *p ) const;
  void operator()( libinput_event_touch *p ) const;
  void operator()( libinput_event_gesture *p ) const;
  void operator()( libinput_event_tablet_tool *p ) const;
  void operator()( libinput_event_tablet_pad *p ) const;
  void operator()( libinput_event_switch *p ) const;
};

enum class libinput_event_category {
  device_event,
  keyboard_event,
  pointer_event,
  touch_event,
  tablet_tool_event,
  tablet_pad_event,
  gesture_event,
  switch_event
};

template< libinput_event_category >
struct libinput_event_type {};
template<>
struct libinput_event_type< libinput_event_category::device_event > {
  using type = libinput_event;
};
template<>
struct libinput_event_type< libinput_event_category::keyboard_event > {
  using type = libinput_event_keyboard;
};
template<>
struct libinput_event_type< libinput_event_category::pointer_event > {
  using type = libinput_event_pointer;
};
template<>
struct libinput_event_type< libinput_event_category::touch_event > {
  using type = libinput_event_touch;
};
template<>
struct libinput_event_type< libinput_event_category::tablet_tool_event > {
  using type = libinput_event_tablet_tool;
};
template<>
struct libinput_event_type< libinput_event_category::tablet_pad_event > {
  using type = libinput_event_tablet_pad;
};
template<>
struct libinput_event_type< libinput_event_category::gesture_event > {
  using type = libinput_event_gesture;
};
template<>
struct libinput_event_type< libinput_event_category::switch_event > {
  using type = libinput_event_switch;
};
template< libinput_event_category e >
using libinput_event_type_t = typename libinput_event_type< e >::type;

class libinput_t final {
  static int open_restricted(const char *path, int flags, void *user_data);
  static void close_restricted(int fd, void *user_data);
class libinput_internal_t final : public std::enable_shared_from_this< libinput_internal_t > {
  struct task_t {
    non_copyable_function< void( libinput_event *event, std::exception_ptr ) > cb;
  };
  struct result_t {
    std::unique_ptr< libinput_event, libinput_event_deleter > event;
    std::exception_ptr p;
  };
  struct listeners_t {
    listeners_t() : current_id( 0u ) {}
    std::mutex guard;
    std::unique_ptr< libinput_event, libinput_event_deleter > current_event;
    std::unordered_map< std::uint64_t, task_t > listener;
    std::uint64_t current_id;
  };
public:
  libinput_internal_t(
    const std::shared_ptr< sched::epoll_notifier_t > &e,
    const std::shared_ptr< sched::thread_pool_t > &p
  );
  void run();
  ~libinput_internal_t();
  template< libinput_event_category type, typename F >
  std::shared_ptr< void > add( F &&cb ) {
    if constexpr ( type == libinput_event_category::device_event ) {
      const auto id = set(
        device,
        std::forward< F >( cb )
      );
      return std::shared_ptr< void >( this, [self=shared_from_this(),id=id]( void* ) { self->del( self->device, id ); } );
    }
#define GCT_INPUT_LIBINPUT_SET_CB( name ) \
  else if constexpr ( type == libinput_event_category:: name ## _event ) { \
    const auto id = set( \
      name , \
      [cb=std::forward< F >( cb )]( libinput_event *event, std::exception_ptr e ) { \
        cb( libinput_event_get_ ## name ## _event( event ), e ); \
      } \
    ); \
    return std::shared_ptr< void >( this, [self=shared_from_this(),id=id]( void* ) { self->del( self-> name , id ); } ); \
  }
    GCT_INPUT_LIBINPUT_SET_CB( keyboard )
    GCT_INPUT_LIBINPUT_SET_CB( pointer )
    GCT_INPUT_LIBINPUT_SET_CB( touch )
    GCT_INPUT_LIBINPUT_SET_CB( gesture )
    GCT_INPUT_LIBINPUT_SET_CB( tablet_tool )
    GCT_INPUT_LIBINPUT_SET_CB( tablet_pad )
    else if constexpr ( type == libinput_event_category::device_event ) {
      const auto id = set(
        switch_,
        [cb=std::forward< F >( cb )]( libinput_event *event, std::exception_ptr e ) {
          cb( libinput_event_get_switch_event( event ), e );
        }
      );
      return std::shared_ptr< void >( this, [self=shared_from_this(),id=id]( void* ) { self->del( self->switch_, id ); } );
    }
  }
  void set_end();
private:
  template< typename F >
  std::uint64_t set( listeners_t &fifo, F &&cb ) {
    std::uint64_t id = 0u;
    {
      std::scoped_lock< std::mutex > lock( fifo.guard );
      id = ++fifo.current_id;
      fifo.listener.insert(
        std::make_pair(
          id,
          task_t{
            std::forward< F >( cb )
          }
        )
      );
    }
    return id;
  }
  void del( listeners_t &fifo, std::uint64_t id );
  void push( listeners_t &fifo, std::unique_ptr< libinput_event, libinput_event_deleter > &&event );
  void del_all( listeners_t &fifo );
  void del_all();
private:
  void poll();
private:
  listeners_t device;
  listeners_t keyboard;
  listeners_t pointer;
  listeners_t touch;
  listeners_t tablet_tool;
  listeners_t tablet_pad;
  listeners_t gesture;
  listeners_t switch_;
  int set_fd;
  std::atomic< bool > end;
  std::shared_ptr< sched::epoll_notifier_t > epoll_notifier;
  std::shared_ptr< sched::thread_pool_t > thread_pool;
};
public:
  libinput_t(
    const std::shared_ptr< sched::epoll_notifier_t > &e,
    const std::shared_ptr< sched::thread_pool_t > &p
  );
  void run();
  ~libinput_t();
  template< libinput_event_category type, typename F >
  std::shared_ptr< void > add( F &&cb ) {
    return raw->add< type >( std::forward< F >( cb ) );
  }
private:
  std::shared_ptr< libinput_internal_t > internal;
  libinput_internal_t *raw = nullptr;
  std::atomic< unsigned int > executed;
};

key_state libinput_key_state_to_gct_key_state( libinput_key_state s );

struct pointer_moved {
  double dx = 0.0;
  double dy = 0.0;
};
struct pointer_button {
  std::uint32_t button_id = 0u;
  key_state state = key_state::released;
};
struct pointer_scroll_wheel {
  double dx = 0.0;
  double dy = 0.0;
};
struct pointer_scroll_finger {
  double dx = 0.0;
  double dy = 0.0;
};
struct pointer_scroll_continuous {
  double dx = 0.0;
  double dy = 0.0;
};

using pointer_event_event = std::variant<
  pointer_moved,
  pointer_button,
  pointer_scroll_wheel,
  pointer_scroll_finger,
  pointer_scroll_continuous
>;

struct pointer_event {
  std::chrono::microseconds relative_time;
  pointer_event_event event;
};

pointer_event libinput_pointer_event_to_gct_pointer_event( libinput_event_pointer *e );

}

#endif

