#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/string_cast.hpp>
#include <gct/to_matrix.hpp>
namespace gct {
  glm::tmat4x4< float > to_matrix(
    const std::array< float, 3 > &t,
    const std::array< float, 4 > &r,
    const std::array< float, 3 > &s
  ) {
    glm::tmat4x4< float > dest( 1.0f );
    dest = glm::translate( dest, glm::tvec3< float >( t[ 0 ], t[ 1 ], t[ 2 ] ) );
    auto rot = glm::tquat< float >( r[ 3 ], r[ 0 ], r[ 1 ], r[ 2 ] );
    glm::tmat4x4< float > quatMat = glm::mat4_cast( rot );
    dest *= quatMat;
    return glm::scale( dest, glm::tvec3< float >( s[ 0 ], s[ 1 ], s[ 2 ] ) );
  }
  glm::tmat4x4< float > to_matrix(
    const std::array< float, 16 > &t
  ) {
    return glm::tmat4x4< float >(
      t[ 0 ], t[ 1 ], t[ 2 ], t[ 3 ],
      t[ 4 ], t[ 5 ], t[ 6 ], t[ 7 ],
      t[ 8 ], t[ 9 ], t[ 10 ], t[ 11 ],
      t[ 12 ], t[ 13 ], t[ 14 ], t[ 15 ]
    );
  }
}

