#include <gct/similar_matrix.hpp>
namespace gct {

bool similar_matrix(
  const glm::mat4 &a,
  const glm::mat4 &b
) {
  for( unsigned int i = 0u; i != 4u; ++i ) {
    for( unsigned int j = 0u; j != 4u; ++j ) {
      if( std::abs( a[ i ][ j ] - b[ i ][ j ] ) > 0.0001f ) {
        return false;
      }
    }
  }
  return true; 
}

}

