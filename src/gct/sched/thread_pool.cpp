#include <iostream>
#include <gct/sched/thread_pool.hpp>
namespace gct::sched {

thread_pool_t::task_queue_t::task_queue_t() : random( std::random_device()() ), end( false ) {}
bool thread_pool_t::task_queue_t::set(
  std::unique_ptr< thread_pool_t::task_queue_t::task_t > &&p,
  bool bound,
  unsigned int priority
) {
  if( priority >= 4u ) priority = 3u;
  if( end.load() == true ) return false;
  if( !increment_task_count() ) {
    return false;
  }
  auto queue_index = ( priority * 2u ) + ( bound ? 0u : 1u );
  while( !queue[ queue_index ].push( p.get() ) ) {
    std::this_thread::yield();
  }
  p.release();
  return true;
}
std::unique_ptr< thread_pool_t::task_queue_t::task_t > thread_pool_t::task_queue_t::get() {
  auto queue_index = self_queue_pickup_dist( random );
  std::unique_ptr< thread_pool_t::task_queue_t::task_t > p;
  if( pop( queue_index, p ) ) {
    return p;
  }
  for( auto i = queue_index; i != 0u; --i ) {
    if( pop( i - 1u, p ) ) {
      return p;
    }
  }
  for( auto i = queue_index + 1u; i != queue.size(); ++i ) {
    if( pop( i, p ) ) {
      return p;
    }
  }
  return std::unique_ptr< thread_pool_t::task_queue_t::task_t >();
}
std::unique_ptr< thread_pool_t::task_queue_t::task_t > thread_pool_t::task_queue_t::get_from_other_thread() {
  auto queue_index = other_queue_pickup_dist( random );
  std::unique_ptr< thread_pool_t::task_queue_t::task_t > p;
  if( pop( queue_index, p ) ) {
    return p;
  }
  for( auto i = queue_index; i != 1u; i -= 2u ) {
    if( pop( i - 2u, p ) ) {
      return p;
    }
  }
  for( auto i = queue_index + 2u; i < queue.size(); i += 2u ) {
    if( pop( i, p ) ) {
      return p;
    }
  }
  return std::unique_ptr< thread_pool_t::task_queue_t::task_t >();
}
void thread_pool_t::task_queue_t::set_end() {
  end = true;
}
bool thread_pool_t::task_queue_t::get_end() const {
  return end.load();
}
void thread_pool_t::task_queue_t::suspend() {
  std::unique_lock< std::mutex > lock( suspend_mutex );
  suspend_state.wait( lock );
}
void thread_pool_t::task_queue_t::wakeup() {
  suspend_state.notify_one();
}
bool thread_pool_t::task_queue_t::empty() const {
  return task_count.load();
}
bool thread_pool_t::task_queue_t::pop( unsigned int queue_index, std::unique_ptr< thread_pool_t::task_queue_t::task_t > &p ) {
  thread_pool_t::task_queue_t::task_t *temp = nullptr;
  if( queue[ queue_index ].pop( temp ) ) {
    p.reset( temp );
    decrement_task_count();
    return true;
  }
  else return false;
}
bool thread_pool_t::task_queue_t::increment_task_count() {
  auto v = task_count.load();
  if( v >= queue_size ) return false;
  while( !task_count.compare_exchange_strong( v, v + 1u ) ) {
    if( v >= queue_size ) return false;
  }
  return true;
}
bool thread_pool_t::task_queue_t::decrement_task_count() {
  auto v = task_count.load();
  if( v == 0u ) return false;
  while( !task_count.compare_exchange_strong( v, v - 1u ) ) {
    if( v == 0u ) return false;
  }
  return true;
}
thread_pool_t::thread_pool_internal_t::thread_pool_internal_t() : queues( new task_queue_t[ std::thread::hardware_concurrency() ] ), round_robin( 0u ) {
  const auto thread_count = std::thread::hardware_concurrency();
  for( std::size_t i = 0u; i != thread_count; ++i ) {
    threads.emplace_back(
      [queues=queues,cur=i,count=thread_count]() {
        auto raw_queues = queues.get();
        task_queue_thread_index = cur;
        while( 1 ) {
          if( !run_one( raw_queues, cur, count ) ) {
            if( raw_queues[ cur ].get_end() ) break; 
            else if( raw_queues[ cur ].empty() == 0u ) {
              raw_queues[ cur ].suspend();
            }
          }
        }
      }
    );
  }
}
thread_pool_t::thread_pool_internal_t::~thread_pool_internal_t() {
  for( auto &t: threads )
    t.join();
}
void thread_pool_t::thread_pool_internal_t::set_end() {
  for( std::size_t i = 0u; i != threads.size(); ++i ) {
    queues[ i ].set_end();
    queues[ i ].wakeup();
  }
}
std::size_t thread_pool_t::thread_pool_internal_t::get_local_thread_id() {
  return task_queue_thread_index;
}
void thread_pool_t::thread_pool_internal_t::add(
  std::unique_ptr< task_queue_t::task_t > &&p
) {
  const auto bound = p->bound;
  const auto priority = p->priority;
  const auto thread = p->thread;
  auto result = queues[ thread % threads.size() ].set( std::move( p ), bound, priority );
  if( result ) {
    queues[ thread % threads.size() ].wakeup();
  }
  if( !result && !bound ) {
    for( std::size_t offset = 1u; offset != threads.size(); ++offset ) {
      result = queues[ ( thread + offset ) % threads.size() ].set( std::move( p ), bound, priority );
      if( result ) {
        queues[ ( thread + offset ) % threads.size() ].wakeup();
        break;
      }
    }
  }
  if( !result ) throw add_task_failed();
}
bool thread_pool_t::thread_pool_internal_t::run_one(
  task_queue_t *queues,
  std::size_t cur,
  std::size_t count
) {
  auto p = queues[ cur ].get();
  if( !p ) {
    for( std::size_t offset = 1u; offset != count; ++offset ) {
      p = queues[ ( cur + offset ) % count ].get_from_other_thread();
      if( p ) break;
    }
  }
  if( !p ) return false;
  try {
    p->func();
  }
  catch( const std::exception &e ) {
    std::cerr << "Uncought exception : " << e.what() << std::endl;
  }
  catch( ... ) {
    std::cerr << "Uncought exception" << std::endl;
  }
  return true;
}
void thread_pool_t::thread_pool_internal_t::schedule_coroutine(
  std::shared_ptr< coroutine_context_t::pull_type > &&pull,
  bool bound,
  unsigned int priority,
  unsigned int thread,
  coroutine_context_t::push_type *push
) {
  add(
    [pull=std::move( pull ),push=push,self=shared_from_this(),bound,priority,thread]() mutable {
      current_coroutine_context.push = push;
      current_coroutine_context.bound = bound;
      current_coroutine_context.priority = priority;
      current_coroutine_context.thread = thread;
      (*pull)();
      auto push_ = current_coroutine_context.push;
      current_coroutine_context.push = nullptr;
      if( *pull ) {
        pull->get()(
          [pull=std::move( pull ),push=push_,self=self,bound,priority,thread]() mutable {
            try {
              self->schedule_coroutine( std::move( pull ), bound, priority, thread, push );
            }
            catch( ... ) {
              std::abort();
            }
          }
        );
      }
    },
    bound,
    priority,
    thread
  );
}
thread_pool_t::~thread_pool_t() {
  internal->set_end();
  /*while( internal.use_count() >= 2u ) {
    sleep( 1 );
  }*/
}
thread_pool_t::thread_pool_t() : internal( new thread_pool_internal_t() ) {
  raw = internal.get();
}

thread_local std::size_t thread_pool_t::thread_pool_internal_t::task_queue_thread_index;

}

