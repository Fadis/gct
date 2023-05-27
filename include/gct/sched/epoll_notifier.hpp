#ifndef GCT_SCHED_EPOLL_NOTIFIER_HPP
#define GCT_SCHED_EPOLL_NOTIFIER_HPP

#include <concepts>
#include <exception>
#include <memory>
#include <unordered_map>
#include <utility>
#include <thread>
#include <mutex>
#include <system_error>
#include <numeric>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <gct/future.hpp>
#include <gct/non_copyable_function.hpp>

namespace gct::sched {

class epoll_notifier_t final {
class epoll_notifier_internal_t final {
struct epoll_task_t {
  non_copyable_function< void( const epoll_event&, std::exception_ptr ) > func;
  epoll_event event;
};
public:
  epoll_notifier_internal_t();
  ~epoll_notifier_internal_t();
  template< typename F >
  std::uint64_t add(
    F &&f,
    std::uint32_t events,
    int fd
  ) requires std::invocable< F, epoll_event, std::exception_ptr > {
    if( end.load() ) {
      try {
        throw std::system_error( std::make_error_code( std::errc::operation_canceled ), "epoll_notifier::add : closed." );
      }
      catch( ... ) {
        epoll_event temp;
        temp.events = events;
        temp.data.fd = fd;
        f( temp, std::current_exception() );
      }
      return 0;
    }
    std::unique_ptr< epoll_task_t > task( new epoll_task_t() );
    task->event.data.fd = fd;
    task->event.events = events;
    const std::uint64_t id = current_id++;
    try {
      std::scoped_lock< std::mutex > lock( guard );
      const auto existing = tasks.find( fd );
      if( existing != tasks.end() ) {
        const auto existing_events = std::accumulate( existing->second.begin(), existing->second.end(), std::uint32_t ( 0 ), []( int sum, const auto &v ) { return sum | v.second->event.events; } );
        if( ( existing_events | events ) != existing_events ) {
          epoll_event sum;
          sum.data.fd = fd;
          sum.events = existing_events | events;
          if( epoll_ctl( epoll_fd, EPOLL_CTL_MOD, fd, &sum ) != 0 ) {
            throw std::system_error( std::make_error_code( std::errc( errno ) ), "epoll_notifier::add : EPOLL_CTL_MOD failed." );
          }
        }
        task->func = std::move( f );
        existing->second.insert( std::make_pair( id, std::move( task ) ) );
      }
      else {
        if( epoll_ctl( epoll_fd, EPOLL_CTL_ADD, fd, &task->event ) != 0 ) {
          throw std::system_error( std::make_error_code( std::errc( errno ) ), "epoll_notifier::add : EPOLL_CTL_ADD failed." );
        }
        task->func = std::move( f );
        std::unordered_map< std::uint64_t, std::unique_ptr< epoll_task_t > > temp;
        temp.insert( std::make_pair( id, std::move( task ) ) );
        tasks.insert( std::make_pair( fd, std::move( temp ) ) );
      }
    }
    catch( ... ) {
      f( task->event, std::current_exception() );
    }
    return id;
  }
  bool del(
    int fd,
    std::uint64_t id
  );
  bool del(
    int fd
  );
private:
  void poll();
  int epoll_fd = 0;
  std::unordered_map< int, std::unordered_map< std::uint64_t, std::unique_ptr< epoll_task_t > > > tasks;
  std::thread thread;
  std::mutex guard;
  int end_fd = 0;
  std::atomic< bool > end;
  std::atomic< std::uint64_t > current_id;
};
public:
  ~epoll_notifier_t();
  epoll_notifier_t( const epoll_notifier_t& ) = delete;
  epoll_notifier_t( epoll_notifier_t&& ) = delete;
  epoll_notifier_t &operator=( const epoll_notifier_t& ) = delete;
  epoll_notifier_t &operator=( epoll_notifier_t&& ) = delete;
  template< typename ...Args >
  auto add(
    Args&& ... args
  ) {
    return raw->add( std::forward< Args >( args )... );
  }
  template< typename ...Args >
  auto del(
    Args&& ... args
  ) {
    return raw->del( std::forward< Args >( args )... );
  }
  future< epoll_event > epoll( int op, int fd );
  epoll_notifier_t();
private:
  std::shared_ptr< epoll_notifier_internal_t > internal;
  epoll_notifier_internal_t *raw = nullptr;
};

}

#endif

