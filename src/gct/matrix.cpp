#include <nlohmann/json.hpp>
#include <gct/matrix.hpp>

namespace glm {

void to_json( nlohmann::json &dest, const glm::mat4 &src ) {
  dest = nlohmann::json::array();
  for( unsigned int i = 0u; i != 4u; ++i ) {
    for( unsigned int j = 0u; j != 4u; ++j ) {
      dest.push_back( src[ i ][ j ] );
    }
  }
}

}
