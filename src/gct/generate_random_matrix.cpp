#include <gct/generate_random_matrix.hpp>

namespace gct {
glm::mat4 generate_random_matrix(
  std::uniform_real_distribution<float> &dist,
  std::mt19937 &engine
) {
  glm::mat4 temp;
  temp[ 0 ][ 0 ] = dist( engine );
  temp[ 0 ][ 1 ] = dist( engine );
  temp[ 0 ][ 2 ] = dist( engine );
  temp[ 0 ][ 3 ] = dist( engine );
  temp[ 1 ][ 0 ] = dist( engine );
  temp[ 1 ][ 1 ] = dist( engine );
  temp[ 1 ][ 2 ] = dist( engine );
  temp[ 1 ][ 3 ] = dist( engine );
  temp[ 2 ][ 0 ] = dist( engine );
  temp[ 2 ][ 1 ] = dist( engine );
  temp[ 2 ][ 2 ] = dist( engine );
  temp[ 2 ][ 3 ] = dist( engine );
  temp[ 3 ][ 0 ] = dist( engine );
  temp[ 3 ][ 1 ] = dist( engine );
  temp[ 3 ][ 2 ] = dist( engine );
  temp[ 3 ][ 3 ] = dist( engine );
  return temp;
}

}

