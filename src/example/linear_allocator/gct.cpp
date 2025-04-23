#include <random>
#include <nlohmann/json.hpp>
#include "gct/linear_allocator_create_info.hpp"
#include <iostream>
#include <gct/linear_allocator.hpp>

int main( int argc, const char *argv[] ) {
  gct::sized_linear_allocator allocator(
    gct::linear_allocator_create_info()
      .set_max( 65536 )
  );
  std::uniform_int_distribution< unsigned int > dist( 1u, 30u );
  std::mt19937 engine( std::random_device{}() );
  std::vector< std::uint32_t > allocated;
  for( unsigned int i = 0u; i != 1000u; ++i ) {
    const auto op = dist( engine );
    if( op % 2 ) {
      const auto size = dist( engine );
      allocated.push_back( allocator.allocate( size ) );
      std::cout << "allocate " << allocated.back() << " with size " << size << std::endl;
    }
    else {
      if( !allocated.empty() ) {
        const auto pos = dist( engine ) % allocated.size();
        allocator.release( allocated[ pos ] );
        std::cout << "release " << allocated[ pos ] << std::endl;
        allocated.erase( std::next( allocated.begin(), pos ) );
      }
    }
    allocator.check();
  }

  std::cout << nlohmann::json( allocator ).dump( 2 ) << std::endl;

}

