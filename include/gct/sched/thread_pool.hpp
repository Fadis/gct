#ifndef GCT_SCHED_THREAD_POOL_HPP
#define GCT_SCHED_THREAD_POOL_HPP

#include <cstddef>
#include <cstdlib>
#include <concepts>
#include <exception>
#include <mutex>
#include <thread>
#include <array>
#include <memory>
#include <atomic>
#include <vector>
#include <random>
#include <utility>
#include <condition_variable>
#include <any>
#include <boost/lockfree/queue.hpp>
#include <gct/non_copyable_function.hpp>
#include <gct/sched/coroutine.hpp>
namespace gct::sched {

struct add_task_failed {};

class thread_pool_t final {
class task_queue_t final {
public:
struct task_t {
  non_copyable_function< void() > func;
  bool bound = false;
  unsigned int priority = 3u;
  unsigned int thread = 0u;
};
  task_queue_t();
  task_queue_t( const task_queue_t& ) = delete;
  task_queue_t( task_queue_t&& ) = delete;
  task_queue_t &operator=( const task_queue_t& ) = delete;
  task_queue_t &operator=(  task_queue_t&& ) = delete;
  static constexpr std::size_t queue_size = 1024u;
  bool set(
    std::unique_ptr< task_t > &&p,
    bool bound,
    unsigned int priority
  );
  std::unique_ptr< task_t > get();
  std::unique_ptr< task_t > get_from_other_thread();
  void set_end();
  bool get_end() const;
  void suspend();
  void wakeup();
  bool empty() const;
private:
  bool pop( unsigned int queue_index, std::unique_ptr< task_t > &p );
  bool increment_task_count();
  bool decrement_task_count();
  std::array< boost::lockfree::queue< task_t* >, 8u > queue{{
    boost::lockfree::queue< task_t* >{ queue_size },
    boost::lockfree::queue< task_t* >{ queue_size },
    boost::lockfree::queue< task_t* >{ queue_size },
    boost::lockfree::queue< task_t* >{ queue_size },
    boost::lockfree::queue< task_t* >{ queue_size },
    boost::lockfree::queue< task_t* >{ queue_size },
    boost::lockfree::queue< task_t* >{ queue_size },
    boost::lockfree::queue< task_t* >{ queue_size }
  }};
  std::discrete_distribution< unsigned int > self_queue_pickup_dist{{
    8., 8., 4., 4., 2., 2., 1., 1.
  }};
  std::discrete_distribution< unsigned int > other_queue_pickup_dist{{
    0., 8., 0., 4., 0., 2., 0., 1.
  }};
  std::atomic< std::size_t > task_count;
  std::minstd_rand random;
  std::atomic< bool > end;
  std::condition_variable_any suspend_state;
  std::mutex suspend_mutex;
};
class thread_pool_internal_t : public std::enable_shared_from_this< thread_pool_internal_t > {
public:
  thread_pool_internal_t();
  ~thread_pool_internal_t();
  void set_end();
  template< typename F >
  void add(
    F &&f,
    bool bound,
    unsigned int priority,
    unsigned int thread
  ) requires std::invocable< F > {
    std::unique_ptr< task_queue_t::task_t > p( new task_queue_t::task_t{ std::move( f ), bound, priority, thread } );
    add( std::move( p ) );
  }
  template< typename F >
  void add(
    F &&f,
    bool bound,
    unsigned int priority
  ) requires std::invocable< F > {
    if( task_queue_thread_index < threads.size() && threads[ task_queue_thread_index ].get_id() == std::this_thread::get_id() ) {
      add( std::move( f ), bound, priority, task_queue_thread_index );
    }
    else {
      add( std::move( f ), bound, priority, round_robin.fetch_add( 1u ) % threads.size() );
    }
  }
  template< typename F >
  void add(
    F &&f,
    bool bound
  ) requires std::invocable< F > {
    add( std::move( f ), bound, 2u );
  }
  template< typename F >
  void add(
    F &&f
  ) requires std::invocable< F > {
    add( std::move( f ), false );
  }
  static std::size_t get_local_thread_id();
  template< typename F >
  void add_co(
    F &&f,
    bool bound,
    unsigned int priority,
    unsigned int thread
  ) {
    std::shared_ptr< coroutine_context_t::pull_type > pull(
      new coroutine_context_t::pull_type(
        [f=std::move( f )]( coroutine_context_t::push_type &push ) mutable {
          push( []( non_copyable_function< void() >&& ){} );
          current_coroutine_context.push = &push;
          f();
        }
      )
    );
    schedule_coroutine( std::move( pull ), bound, priority, thread );
  }
  template< typename F >
  void add_co(
    F &&f,
    bool bound,
    unsigned int priority
  ) requires std::invocable< F > {
    if( task_queue_thread_index < threads.size() && threads[ task_queue_thread_index ].get_id() == std::this_thread::get_id() ) {
      add_co( std::move( f ), bound, priority, task_queue_thread_index );
    }
    else {
      add_co( std::move( f ), bound, priority, round_robin.fetch_add( 1u ) % threads.size() );
    }
  }
  template< typename F >
  void add_co(
    F &&f,
    bool bound
  ) requires std::invocable< F > {
    add_co( std::move( f ), bound, 2u );
  }
  template< typename F >
  void add_co(
    F &&f
  ) requires std::invocable< F > {
    add_co( std::move( f ), false );
  }
  template< typename T >
  void set_depend( T &&v ) {
    depends.push_back( std::forward< T >( v ) );
  }
private:
  void add(
    std::unique_ptr< task_queue_t::task_t > &&p
  );
  static bool run_one(
    task_queue_t *queues,
    std::size_t cur,
    std::size_t count
  );
  void schedule_coroutine(
    std::shared_ptr< coroutine_context_t::pull_type > &&pull,
    bool bound,
    unsigned int priority,
    unsigned int thread,
    coroutine_context_t::push_type *push = nullptr
  );
  static thread_local std::size_t task_queue_thread_index;
  std::shared_ptr< task_queue_t[] > queues;
  std::vector< std::thread > threads;
  std::atomic< unsigned int > round_robin;
  std::vector< std::any > depends;
};


public:
  ~thread_pool_t();
  template< typename ...Args >
  void add(
    Args&& ... args
  ) {
    raw->add( std::forward< Args >( args )... );
  }
  template< typename ...Args >
  void add_co(
    Args&& ... args
  ) {
    raw->add_co( std::forward< Args >( args )... );
  }
  template< typename T >
  void set_depend( T &&v ) {
    raw->set_depend( std::forward< T >( v ) );
  }
  thread_pool_t();
private:
  std::shared_ptr< thread_pool_internal_t > internal;
  thread_pool_internal_t *raw = nullptr;
};

}

#endif

