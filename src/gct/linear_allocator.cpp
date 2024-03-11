#include <gct/exception.hpp>
#include <gct/linear_allocator.hpp>

namespace gct {

linear_allocator::linear_allocator(
  const linear_allocator_create_info &ci
) : props( ci ) {}
std::uint32_t linear_allocator::allocate() {
  if( deallocated.empty() ) {
    if( tail == props.max ) {
      throw exception::out_of_range( "linear_allocator::allocate : no space left", __FILE__, __LINE__ );
    }
    return tail++;
  }
  else {
    auto back = std::prev( deallocated.end() );
    std::uint32_t temp = *back;
    deallocated.erase( back );
    return temp;
  }
}
void linear_allocator::release( std::uint32_t i ) {
  if( i == tail - 1u ) {
    --tail;
    if( !deallocated.empty() ) {
      auto back = std::prev( deallocated.end() );
      while( *back == tail - 1u ) {
        --tail;
        --back;
      }
      deallocated.erase( std::next( back ), deallocated.end() );
    }
  }
  else if( i < tail - 1u ) {
    deallocated.insert( i );
  }
}

}

