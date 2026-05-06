#include <glm/geometric.hpp>
#include <gct/shader/glsl/gct/simeq.h>
#include <gct/shader/glsl/gct/n31.h>
#include <iostream>
#include <random>

int main() {
  std::default_random_engine rng( std::random_device{}() );
  std::uniform_real_distribution<> dist( -1.0, 1.0 );

  for( unsigned int i = 0u; i != 1000000; ++i ) {
    const glm::vec3 normal = glm::normalize( glm::vec3( dist( rng ), dist( rng ), dist( rng ) ) );
    const auto encoded = n31_encode( normal );
    const auto decoded_normal = n31_decode( encoded );
    if( !simeq( normal, decoded_normal, 0.009f ) ) {
      std::cout <<
                   ( simeq( normal, decoded_normal, 0.009f ) ? "[  OK  ] " : "[FAILED] " ) <<
                   "normal ( " << normal.x << ", " << normal.y << ", " << normal.z << " ) " <<
                   "( " << decoded_normal.x << ", " << decoded_normal.y << ", " << decoded_normal.z << " )" <<
                   std::endl;
    }
  }
}

