#include <gct/sched/wait.hpp>

namespace gct::sched {

void wait( future< void > &&f ) {
  if( current_coroutine_context.push == nullptr ) {
    throw std::system_error( std::make_error_code( std::errc::operation_not_permitted ), "wait : outside of coroutine context." );
  }
  auto out = std::make_shared< std::exception_ptr >();
  (*current_coroutine_context.push)(
    [f=std::move(f),out]( non_copyable_function< void() > &&cont ) mutable {
      f.then(
        [cont=std::move(cont),out]( std::exception_ptr v ) {
          *out = v;
          cont();
        }
      );
    }
  );
  if( *out ) {
    std::rethrow_exception( *out );
  }
}

}

