#ifndef GCT_SCHED_SLEEP_HPP
#define GCT_SCHED_SLEEP_HPP

#include <chrono>
#include <exception>
#include <sys/timerfd.h>
#include <gct/future.hpp>
#include <gct/concepts/clock.hpp>
#include <gct/sched/epoll_notifier.hpp>

namespace gct::sched {

template< typename C, typename D >
future< void > sleep_until( const std::shared_ptr< sched::epoll_notifier_t > &epoll, const std::chrono::time_point< C, D > &until ) requires Clock< C > {
  const auto now = C::now();
  if( until <= now ) {
    promise< void > p;
    p.set_value();
    return p.get_future();
  }
  const int timerfd = timerfd_create( CLOCK_MONOTONIC, 0 );
  if( timerfd < 0 ) {
    try {
      throw std::system_error( std::make_error_code( std::errc( errno ) ), "sleep_for : timerfd_create failed." );
    }
    catch( ... ) {
      promise< void > p;
      p.set_exception( std::current_exception() );
      return p.get_future();
    }
  }
  const std::uint64_t count = std::chrono::duration_cast< std::chrono::nanoseconds >( until - now ).count();
  itimerspec temp;
  temp.it_value.tv_sec = count / ( 1000u * 1000u * 1000u );
  temp.it_value.tv_nsec = count % ( 1000u * 1000u * 1000u );
  temp.it_interval.tv_sec = 0;
  temp.it_interval.tv_nsec = 0;
  if( timerfd_settime( timerfd, 0, &temp, nullptr ) < 0 ) {
    try {
      throw std::system_error( std::make_error_code( std::errc( errno ) ), "sleep_for : timerfd_settime failed." );
    }
    catch( ... ) {
      close( timerfd );
      promise< void > p;
      p.set_exception( std::current_exception() );
      return p.get_future();
    }
  }
  auto f = epoll->epoll( EPOLLIN, timerfd );
  f.set_cancel( [timerfd]() { close( timerfd ); } );
  return f.then( [timerfd]( auto ) { close( timerfd ); } );
}

template< typename R, typename P >
future< void > sleep_for( const std::shared_ptr< sched::epoll_notifier_t > &epoll, const std::chrono::duration< R, P > &duration ) {
  return sleep_until( epoll, std::chrono::high_resolution_clock::now() + duration );
}

}

#endif

