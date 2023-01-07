#include <thread>
#include <gct/sched/coroutine.hpp>

namespace gct::sched {

void yield() {
  if( current_coroutine_context.push == nullptr ) {
    std::this_thread::yield();
    return;
  }
  (*current_coroutine_context.push)(
    []( non_copyable_function< void() > &&cont ) {
      cont();
    }
  );
}

thread_local coroutine_context_t current_coroutine_context;

}

