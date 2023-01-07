#ifndef GCT_SCHED_COROUTINE_HPP
#define GCT_SCHED_COROUTINE_HPP
#include <boost/coroutine2/coroutine.hpp>
#include <gct/non_copyable_function.hpp>

namespace gct::sched {

struct coroutine_context_t {
  using pull_type = boost::coroutines2::coroutine< non_copyable_function< void( non_copyable_function< void() >&& ) > >::pull_type;
  using push_type = boost::coroutines2::coroutine< non_copyable_function< void( non_copyable_function< void() >&& ) > >::push_type;
  push_type *push = nullptr;
  bool bound = false;
  unsigned int priority = 3u;
  unsigned int thread = 0u;
};

extern thread_local coroutine_context_t current_coroutine_context;

void yield();

}

#endif

