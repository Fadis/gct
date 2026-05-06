#include <iostream>
#include <random>
#include <glm/geometric.hpp>
#include <gct/simeq.hpp>
#include <gct/n21t11.hpp>

int main() {
  std::default_random_engine rng( std::random_device{}() );
  std::uniform_real_distribution<> dist( -1.0, 1.0 );

  for( unsigned int i = 0u; i != 1000000; ++i ) {
    const glm::vec3 normal = glm::normalize( glm::vec3( dist( rng ), dist( rng ), dist( rng ) ) );
    const glm::vec3 binormal = glm::normalize( glm::vec3( dist( rng ), dist( rng ), dist( rng ) ) );
    const glm::vec3 tangent = glm::normalize( glm::cross( normal, binormal ) );
    const auto encoded = gct::n21t11_encode( normal, tangent );
    const auto decoded_normal = gct::n21t11_decode_normal( encoded );
    const auto decoded_tangent = gct::n21t11_decode_tangent( encoded, decoded_normal );
    if( !gct::simeq( normal, decoded_normal, 0.017f ) || !gct::simeq( tangent, decoded_tangent, 0.017f ) ) {
      std::cout <<
                   ( gct::simeq( normal, decoded_normal, 0.017f ) ? "[  OK  ] " : "[FAILED] " ) <<
                   "normal ( " << normal.x << ", " << normal.y << ", " << normal.z << " ) " <<
                   "( " << decoded_normal.x << ", " << decoded_normal.y << ", " << decoded_normal.z << " )" <<
                   std::endl;
      std::cout <<
                   ( gct::simeq( tangent, decoded_tangent, 0.017f ) ? "[  OK  ] " : "[FAILED] " ) <<
                   "tangent ( " << tangent.x << ", " << tangent.y << ", " << tangent.z << " ) " <<
                   "( " << decoded_tangent.x << ", " << decoded_tangent.y << ", " << decoded_tangent.z << " )" <<
                   std::endl;
    }
  }
}

