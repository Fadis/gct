#include <glm/geometric.hpp>
#include <gct/shader/glsl/gct/simeq.h>
#include <gct/shader/glsl/gct/n20t11b1.h>
#include <iostream>
#include <random>

int main() {
  std::default_random_engine rng( std::random_device{}() );
  std::uniform_real_distribution<> dist( -1.0, 1.0 );

  for( unsigned int i = 0u; i != 1000000; ++i ) {
    const glm::vec3 normal = glm::normalize( glm::vec3( dist( rng ), dist( rng ), dist( rng ) ) );
    const glm::vec3 binormal = glm::normalize( glm::vec3( dist( rng ), dist( rng ), dist( rng ) ) );
    const glm::vec4 tangent = glm::vec4( glm::normalize( glm::cross( normal, binormal ) ), dist( rng ) < 0.0 ? -1.0 : 1.0 );
    const auto encoded = n20t11b1_encode( normal, tangent );
    const auto decoded_normal = n20t11b1_decode_normal( encoded );
    const auto decoded_tangent = n20t11b1_decode_tangent( encoded, decoded_normal );
    if( !simeq( normal, decoded_normal, 0.024f ) || !simeq( tangent, decoded_tangent, 0.024f ) ) {
      std::cout <<
                   ( simeq( normal, decoded_normal, 0.024f ) ? "[  OK  ] " : "[FAILED] " ) <<
                   "normal ( " << normal.x << ", " << normal.y << ", " << normal.z << " ) " <<
                   "( " << decoded_normal.x << ", " << decoded_normal.y << ", " << decoded_normal.z << " )" <<
                   std::endl;
      std::cout <<
                   ( simeq( tangent, decoded_tangent, 0.024f ) ? "[  OK  ] " : "[FAILED] " ) <<
                   "tangent ( " << tangent.x << ", " << tangent.y << ", " << tangent.z << ", " << tangent.w << " ) " <<
                   "( " << decoded_tangent.x << ", " << decoded_tangent.y << ", " << decoded_tangent.z << ", " << decoded_tangent.w << " )" <<
                   std::endl;
    }
  }
}

