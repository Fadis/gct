#include <nlohmann/json.hpp>
#include <gct/exception.hpp>
#include <gct/linear_allocator.hpp>

namespace gct {

linear_allocator::linear_allocator(
  const linear_allocator_create_info &ci
) : property_type( ci ) {}
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
        if( back == deallocated.begin() ) break;
        --back;
      }
      deallocated.erase( std::next( back ), deallocated.end() );
    }
  }
  else if( i < tail - 1u ) {
    deallocated.insert( i );
  }
}
void linear_allocator::reset() {
  deallocated.clear();
  tail = 0u;
}
void linear_allocator::to_json( nlohmann::json &dest ) const {
  dest = nlohmann::json::object();
  dest[ "props" ] = get_props();
  dest[ "tail" ] = tail;
  dest[ "deallocated" ] = nlohmann::json::array();
  for( auto e: deallocated ) {
    dest[ "deallocated" ].push_back( e );
  }
}
void to_json( nlohmann::json &dest, const linear_allocator &src ) {
  src.to_json( dest );
}

reduced_linear_allocator::reduced_linear_allocator(
  const linear_allocator_create_info &ci
) : property_type( ci ) {}
std::uint32_t reduced_linear_allocator::allocate() {
  if( tail == props.max ) {
    throw exception::out_of_range( "reduced_linear_allocator::allocate : no space left", __FILE__, __LINE__ );
  }
  return tail++;
}
void reduced_linear_allocator::reset() {
  tail = 0u;
}
void reduced_linear_allocator::to_json( nlohmann::json &dest ) const {
  dest = nlohmann::json::object();
  dest[ "props" ] = get_props();
  dest[ "tail" ] = tail;
}
void to_json( nlohmann::json &dest, const reduced_linear_allocator &src ) {
  src.to_json( dest );
}

}

