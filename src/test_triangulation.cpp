#include <iostream>
#include <random>
#include <glm/geometric.hpp>
#include <gct/simeq.hpp>
#include <gct/triangulation.hpp>
#include <gct/area.hpp>

int main() {
  std::default_random_engine rng( std::random_device{}() );
  std::uniform_real_distribution<> dist( -1.0, 1.0 );

  std::vector< glm::vec2 > vertex{
    glm::vec2( -0.6, 0.6 ),
    glm::vec2( -0.6, -0.6 ),
    glm::vec2(  0.6, -0.6 ),
    glm::vec2(  0.6,  0.6 ),
    glm::vec2(  0.4,  0.6 ),
    glm::vec2(  0.4,  -0.3 ),
    glm::vec2(  -0.4,  -0.3 ),
    glm::vec2(  -0.4,  0.6 ),
    glm::vec2(  -0.4,  0.6 )
  };

  const auto t = gct::ear_clipping_triangulation( vertex );

  for( const auto &v: t ) {
    std::cout << v[ 0 ].x << "\t" << v[ 0 ].y << std::endl;
    std::cout << v[ 1 ].x << "\t" << v[ 1 ].y << std::endl;
    std::cout << v[ 2 ].x << "\t" << v[ 2 ].y << std::endl;
    std::cout << v[ 0 ].x << "\t" << v[ 0 ].y << std::endl;
    std::cout << std::endl;
  }

  std::cout << gct::area( vertex ) << std::endl;
}

