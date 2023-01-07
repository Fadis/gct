#ifndef GCT_SCHED_WAIT_HPP
#define GCT_SCHED_WAIT_HPP

#include <memory>
#include <type_traits>
#include <exception>
#include <variant>
#include <utility>
#include <gct/future.hpp>
#include <gct/sched/coroutine.hpp>

namespace gct::sched {

template< typename T >
auto wait( future< T > &&f ) -> std::enable_if_t< !std::is_void_v< T >, T > {
  if( current_coroutine_context.push == nullptr ) {
    throw std::system_error( std::make_error_code( std::errc::operation_not_permitted ), "wait : outside of coroutine context." );
  }
  auto out = std::make_shared< std::variant< std::exception_ptr, T > >();
  (*current_coroutine_context.push)(
    [f=std::move(f),out]( non_copyable_function< void() > &&cont ) mutable {
      f.then(
        [cont=std::move(cont),out]( std::variant< std::exception_ptr, T > &&v ) {
          *out = std::move( v );
          cont();
        }
      );
    }
  );
  if( out->index() == 0u ) {
    std::rethrow_exception( std::get< std::exception_ptr >( *out ) );
  }
  else {
    return std::move( std::get< T >( *out ) );
  }
}

void wait( future< void > &&f );

}

#endif

