#include <numeric>
#include <sys/socket.h>
#include <gct/future.hpp>
#include <gct/sched/epoll_notifier.hpp>

namespace gct::sched {

  epoll_notifier_t::epoll_notifier_internal_t::epoll_notifier_internal_t() : end( false ), current_id( 1 ) {
    epoll_fd = epoll_create( 1 );
    end_fd = eventfd( 0, 0 );
    epoll_event end_event;
    end_event.events = EPOLLIN | EPOLLRDHUP;
    end_event.data.fd = end_fd;
    if( epoll_ctl(epoll_fd, EPOLL_CTL_ADD, end_fd, &end_event) < 0 ) {
      throw std::system_error( std::make_error_code( std::errc( errno ) ), "epoll_notifier::epoll_notifier_t : Initialization failed." );
    }
    thread = std::thread(
      [this]() {
        poll();
      }
    );
  }
  epoll_notifier_t::epoll_notifier_internal_t::~epoll_notifier_internal_t() {
    end = true;
    const std::uint64_t val = 1u;
    write( end_fd, &val, sizeof( val ) );
    thread.join();
    close( epoll_fd );
  }
  bool epoll_notifier_t::epoll_notifier_internal_t::del(
    int fd,
    std::uint64_t id
  ) {
    non_copyable_function< void( const epoll_event&, std::exception_ptr ) > func;
    epoll_event event;
    try {
      std::scoped_lock< std::mutex > lock( guard );
      const auto iter = tasks.find( fd );
      if( iter != tasks.end() ) {
        auto found = iter->second.find( id );
        if( found != iter->second.end() ) {
          if( iter->second.size() == 1u ) {
            if( epoll_ctl( epoll_fd, EPOLL_CTL_DEL, fd, &found->second->event ) != 0 ) {
              throw std::system_error( std::make_error_code( std::errc( errno ) ), "epoll_notifier::del : EPOLL_CTL_DEL failed." );
            }
            func = std::move( found->second->func );
            event = found->second->event;
            tasks.erase( iter );
          }
          else {
            const auto existing_events = std::accumulate( iter->second.begin(), iter->second.end(), std::uint32_t( 0 ), [id]( int sum, const auto &v ) { return ( v.first != id ) ? ( sum | v.second->event.events ) : sum; } );
            if( ( existing_events | found->second->event.events ) != existing_events ) {
              epoll_event sum;
              sum.data.fd = fd;
              sum.events = existing_events;
              if( epoll_ctl( epoll_fd, EPOLL_CTL_MOD, fd, &sum ) != 0 ) {
                throw std::system_error( std::make_error_code( std::errc( errno ) ), "epoll_notifier::del : EPOLL_CTL_MOD failed." );
              }
            }
            func = std::move( found->second->func );
            event = found->second->event;
            iter->second.erase( found );
          }
        }
      }
    }
    catch( ... ) {
      std::abort();
    }
    if( func ) {
      try {
        throw std::system_error( std::make_error_code( std::errc::operation_canceled ), "epoll_notifier::del : Canceled." );
      }
      catch( ... ) {
        func( event, std::current_exception() );
      }
      return true;
    }
    return false;
  }
  bool epoll_notifier_t::epoll_notifier_internal_t::del(
    int fd
  ) {
    std::vector<
      std::pair< 
        non_copyable_function< void( const epoll_event&, std::exception_ptr ) >,
        epoll_event
      >
    > funcs;
    try {
      std::scoped_lock< std::mutex > lock( guard );
      const auto iter = tasks.find( fd );
      if( iter != tasks.end() ) {
        const auto existing_events = std::accumulate( iter->second.begin(), iter->second.end(), std::uint32_t( 0 ), []( int sum, const auto &v ) { return sum | v.second->event.events; } );
        epoll_event sum;
        sum.data.fd = fd;
        sum.events = existing_events;
        if( epoll_ctl( epoll_fd, EPOLL_CTL_DEL, fd, &sum ) != 0 ) {
          throw std::system_error( std::make_error_code( std::errc( errno ) ), "epoll_notifier::del : EPOLL_CTL_DEL failed." );
        }
        for( auto &[id,t]: iter->second ) {
          funcs.push_back( std::make_pair( t->func, t->event ) );
        }
        tasks.erase( iter );
      }
    }
    catch( ... ) {
      std::abort();
    }
    for( auto &[func,event]: funcs ) {
      try {
        throw std::system_error( std::make_error_code( std::errc::operation_canceled ), "epoll_notifier::del : Canceled." );
      }
      catch( ... ) {
        func( event, std::current_exception() );
      }
    }
    return funcs.size();
  }
  void epoll_notifier_t::epoll_notifier_internal_t::poll() {
    epoll_event event;
    int wait_for = -1;
    while( 1 ) {
      const auto result = epoll_wait( epoll_fd, &event, 1, wait_for );
      if( result < 0 ) {
        if( errno == EINTR ) continue;
        else std::abort();
      }
      else if( result == 1 ) {
        if( event.data.fd == end_fd ) {
          epoll_ctl( epoll_fd, EPOLL_CTL_DEL, event.data.fd, &event );
          wait_for = 0;
        }
        else {
          non_copyable_function< void( const epoll_event&, std::exception_ptr ) > func;
          try {
            std::scoped_lock< std::mutex > lock( guard );
            const auto iter = tasks.find( event.data.fd );
            if( iter != tasks.end() ) {
              auto found = std::find_if( iter->second.begin(), iter->second.end(), [&]( const auto &v ) -> bool { return v.second->event.events & event.events; } );
              if( found != iter->second.end() ) {
                if( iter->second.size() == 1u ) {
                  if( epoll_ctl( epoll_fd, EPOLL_CTL_DEL, event.data.fd, &found->second->event ) != 0 ) {
                    throw std::system_error( std::make_error_code( std::errc( errno ) ), "epoll_notifier::poll : EPOLL_CTL_DEL failed." );
                  }
                  func = std::move( found->second->func );
                  tasks.erase( iter );
                }
                else {
                  const auto existing_events = std::accumulate( iter->second.begin(), iter->second.end(), std::uint32_t( 0 ), [id=found->first]( int sum, const auto &v ) { return ( v.first != id ) ? ( sum | v.second->event.events ) : sum; } );
                  if( ( existing_events | found->second->event.events ) != existing_events ) {
                    epoll_event sum;
                    sum.data.fd = event.data.fd;
                    sum.events = existing_events;
                    if( epoll_ctl( epoll_fd, EPOLL_CTL_MOD, event.data.fd, &sum ) != 0 ) {
                      throw std::system_error( std::make_error_code( std::errc( errno ) ), "epoll_notifier::poll : EPOLL_CTL_MOD failed." );
                    }
                  }
                  func = std::move( found->second->func );
                  iter->second.erase( found );
                }
              }
            }
          }
          catch( ... ) {
            std::abort();
          }
          if( func ) {
            func( event, std::exception_ptr() );
          }
        }
      }
      else if( result > 1 ) {
        std::abort();
      }
      else if( wait_for == 0 ) {
        break;
      }
    }
    std::vector< non_copyable_function< void() > > funcs;
    {
      std::scoped_lock< std::mutex > lock( guard );
      for( auto &[fd,t] : tasks ) {
        const auto existing_events = std::accumulate( t.begin(), t.end(), 0, []( int sum, const auto &v ) { return sum | v.second->event.events; } );
        epoll_event sum;
        sum.data.fd = fd;
        sum.events = existing_events;
        if( epoll_ctl( epoll_fd, EPOLL_CTL_DEL, fd, &sum ) < 0 ) {
          std::abort();
        }
        for( auto &[id,task] : t ) {
          funcs.emplace_back(
            [t=std::move( task )]() {
              try {
                throw std::system_error( std::make_error_code( std::errc::operation_canceled ), "epoll_notifier::poll : Closed." );
              }
              catch( ... ) {
                t->func( t->event, std::current_exception() );
              }
            }
          );
        }
      }
      tasks.clear();
    }
    for( auto &f: funcs ) {
      f();
    }
  }
  epoll_notifier_t::~epoll_notifier_t() {
    /*while( internal.use_count() >= 2u ) {
      sleep( 1 );
    }*/
  }
  future< epoll_event > epoll_notifier_t::epoll( int op, int fd ) {
    promise< epoll_event > p;
    auto f = p.get_future();
    const auto id = add(
      [p=std::move(p),op,fd]( const epoll_event &event, std::exception_ptr e ) mutable {
        if( e ) {
          p.set_exception( e );
        }
        else if( ( event.events & EPOLLERR ) && !( op & EPOLLERR ) ) {
          try {
            int error_value = 0;
            socklen_t value_size = sizeof( error_value );
            if( getsockopt( event.data.fd, SOL_SOCKET, SO_ERROR, &error_value, &value_size ) < 0 ) {
              throw std::system_error( std::make_error_code( std::errc( errno ) ), "epoll : Unable to retrive SO_ERROR." );
            }
            throw std::system_error( std::make_error_code( std::errc( error_value ) ), "epoll : Unexpected event on the fd." );
          }
          catch( ... ) {
            p.set_exception( std::current_exception() );
          }
        }
        else if( ( event.events & EPOLLHUP ) && !(  op & EPOLLHUP ) ) {
          try {
            throw std::system_error( std::make_error_code( std::errc::broken_pipe ), "epoll : The fd is closed." );
          }
          catch( ... ) {
            p.set_exception( std::current_exception() );
          }
        }
        else {
          p.set_value( event );
        }
      },
      op,
      fd
    );
    f.set_cancel( [fd,id,self=std::weak_ptr< epoll_notifier_internal_t >( internal )](){
      if( auto p = self.lock() ) {
        p->del( fd, id );
      }
    } );
    return f;
  }
  epoll_notifier_t::epoll_notifier_t() : internal( new epoll_notifier_internal_t() ) {
    raw = internal.get();
    //thread_pool_t::get().set_depend( internal );
  }

}

