#include <random>
#include <iostream>
#include <gct/kdtree.hpp>

namespace gct {

bool test_kdtree() {
  std::uniform_real_distribution<float> pos_dist( -500.0f, 500.0f );
  std::uniform_real_distribution<float> size_dist( 0.0f, 5.0f );
  std::uniform_real_distribution<float> size2_dist( 0.0f, 100.0f );
  std::mt19937 engine( std::random_device{}() );
  kdtree< unsigned int > tree;
  bool success = true;
  for( unsigned int i = 0u; i != 100000u; ++i ) {
    aabb r;
    r.min = glm::vec3( pos_dist( engine ), pos_dist( engine ), pos_dist( engine ) );
    r.max = r.min + glm::vec3( size_dist( engine ), size_dist( engine ), size_dist( engine ) );
    tree.insert( r, i );
  }
  std::uint32_t found_count = 0u;
  while( found_count < 1000u ) {
    aabb r;
    r.min = glm::vec3( pos_dist( engine ), pos_dist( engine ), pos_dist( engine ) );
    r.max = r.min + glm::vec3( size2_dist( engine ), size2_dist( engine ), size2_dist( engine ) );
    for( auto &v: tree.find( r ) ) {
      if( !( v->range && r ) ) {
        std::cout << "oops : " << gct::to_string( v->range ) << " and " << gct::to_string( r ) << " don't intersect" << std::endl;
        success = false;
      }
      ++found_count;
    }
  }
  return success;
}

}

