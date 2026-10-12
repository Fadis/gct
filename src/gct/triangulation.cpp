#include <array>
#include <vector>
#include <iterator>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <gct/intersect.hpp>

namespace gct {

bool point_is_inside_of_triangle(
  const glm::vec2 &p,
  const std::array< glm::vec2, 3u > &t
) {
  const auto ab = t[ 1 ] - t[ 0 ];
  const auto bc = t[ 2 ] - t[ 1 ];
  const auto ca = t[ 0 ] - t[ 2 ];
  const auto ap = p - t[ 0 ];
  const auto bp = p - t[ 1 ];
  const auto cp = p - t[ 2 ];
  const auto ab3 = glm::vec3( ab.x, ab.y, 0.0f );
  const auto bc3 = glm::vec3( bc.x, bc.y, 0.0f );
  const auto ca3 = glm::vec3( ca.x, ca.y, 0.0f );
  const auto ap3 = glm::vec3( ap.x, ap.y, 0.0f );
  const auto bp3 = glm::vec3( bp.x, bp.y, 0.0f );
  const auto cp3 = glm::vec3( cp.x, cp.y, 0.0f );
  auto ax = glm::cross( ab3, ap3 );
  auto bx = glm::cross( bc3, bp3 );
  auto cx = glm::cross( ca3, cp3 );
  if( ax.z < 0.0f && bx.z < 0.0f && cx.z < 0.0f ) return true;
  if( ax.z >= 0.0f && bx.z >= 0.0f && cx.z >= 0.0f ) return true;
  return false;
}


std::vector< std::array< glm::vec2, 3u > > ear_clipping_triangulation(
  const std::vector< glm::vec2 > &input
) {
  if( input.size() < 3u ) return std::vector< std::array< glm::vec2, 3u > >{};
  std::vector< glm::vec2 > left = input;
  std::vector< std::array< glm::vec2, 3u > > output;
  while( left.size() > 3u ) {
    for( unsigned int i = 0u; i != left.size(); ++i ) {
      const std::array< glm::vec2, 3u > cand{
        left[ i ],
        left[ ( i + 1 ) % left.size() ],
        left[ ( i + 2 ) % left.size() ]
      };
      bool valid = true;
      for( unsigned int j = 0u; j != input.size(); ++j ) {
        if(
          intersect( { cand[ 2 ], cand[ 0 ]  }, { input[ j ], input[ ( j + 1 ) % input.size() ] }, false )
        ) {
          valid = false;
          break;
        }
      }
      if( valid ) {
        output.push_back( cand );
        left.erase( std::next( left.begin(), ( i + 1 ) % left.size() ) );
        break;
      }
    }
  }
  output.push_back(
    std::array< glm::vec2, 3u >{
      left[ 0 ],
      left[ 1 ],
      left[ 2 ]
    }
  );
  return output;
}

}
