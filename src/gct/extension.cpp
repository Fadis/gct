#include <gct/extension.hpp>

namespace gct {
  void **get_chain_tail( const void *v ) {
    auto iter = reinterpret_cast< chainable_t* >( const_cast< void* >( v ) );
    if( iter == nullptr ) return nullptr;
    auto last = iter->pNext;
    while( last ) {
      iter = reinterpret_cast< chainable_t* >( last );
      last = iter->pNext;
    }
    return &iter->pNext;
  }
}

