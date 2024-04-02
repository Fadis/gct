#include <algorithm>
#include <nlohmann/json.hpp>
#include <gct/aabb.hpp>

namespace gct {

aabb3 operator&( const aabb3 &l, const aabb3 &r ) {
  aabb3 result;
  result.min[ 0 ] = std::max( l.min[ 0 ], r.min[ 0 ] );
  result.min[ 1 ] = std::max( l.min[ 1 ], r.min[ 1 ] );
  result.min[ 2 ] = std::max( l.min[ 2 ], r.min[ 2 ] );
  result.max[ 0 ] = std::min( l.max[ 0 ], r.max[ 0 ] );
  result.max[ 1 ] = std::min( l.max[ 1 ], r.max[ 1 ] );
  result.max[ 2 ] = std::min( l.max[ 2 ], r.max[ 2 ] );
  result.min[ 0 ] = std::min( result.min[ 0 ], result.max[ 0 ] );
  result.min[ 1 ] = std::min( result.min[ 1 ], result.max[ 1 ] );
  result.min[ 2 ] = std::min( result.min[ 2 ], result.max[ 2 ] );
  return result;
}
aabb4 operator&( const aabb4 &l, const aabb4 &r ) {
  aabb4 result;
  result.min[ 0 ] = std::max( l.min[ 0 ], r.min[ 0 ] );
  result.min[ 1 ] = std::max( l.min[ 1 ], r.min[ 1 ] );
  result.min[ 2 ] = std::max( l.min[ 2 ], r.min[ 2 ] );
  result.min[ 3 ] = std::max( l.min[ 3 ], r.min[ 3 ] );
  result.max[ 0 ] = std::min( l.max[ 0 ], r.max[ 0 ] );
  result.max[ 1 ] = std::min( l.max[ 1 ], r.max[ 1 ] );
  result.max[ 2 ] = std::min( l.max[ 2 ], r.max[ 2 ] );
  result.max[ 3 ] = std::min( l.max[ 3 ], r.max[ 3 ] );
  result.min[ 0 ] = std::min( result.min[ 0 ], result.max[ 0 ] );
  result.min[ 1 ] = std::min( result.min[ 1 ], result.max[ 1 ] );
  result.min[ 2 ] = std::min( result.min[ 2 ], result.max[ 2 ] );
  result.min[ 3 ] = std::min( result.min[ 3 ], result.max[ 3 ] );
  return result;
}
aabb3 operator|( const aabb3 &l, const aabb3 &r ) {
  aabb3 result;
  result.min[ 0 ] = std::min( l.min[ 0 ], r.min[ 0 ] );
  result.min[ 1 ] = std::min( l.min[ 1 ], r.min[ 1 ] );
  result.min[ 2 ] = std::min( l.min[ 2 ], r.min[ 2 ] );
  result.max[ 0 ] = std::max( l.max[ 0 ], r.max[ 0 ] );
  result.max[ 1 ] = std::max( l.max[ 1 ], r.max[ 1 ] );
  result.max[ 2 ] = std::max( l.max[ 2 ], r.max[ 2 ] );
  return result;
}
aabb4 operator|( const aabb4 &l, const aabb4 &r ) {
  aabb4 result;
  result.min[ 0 ] = std::min( l.min[ 0 ], r.min[ 0 ] );
  result.min[ 1 ] = std::min( l.min[ 1 ], r.min[ 1 ] );
  result.min[ 2 ] = std::min( l.min[ 2 ], r.min[ 2 ] );
  result.min[ 3 ] = std::min( l.min[ 3 ], r.min[ 3 ] );
  result.max[ 0 ] = std::max( l.max[ 0 ], r.max[ 0 ] );
  result.max[ 1 ] = std::max( l.max[ 1 ], r.max[ 1 ] );
  result.max[ 2 ] = std::max( l.max[ 2 ], r.max[ 2 ] );
  result.max[ 3 ] = std::max( l.max[ 3 ], r.max[ 3 ] );
  return result;
}

aabb3 operator*( const glm::mat4 &l, const aabb3 &r ) {
  std::array< glm::vec4, 8u > vertex{
    glm::vec4( r.min[ 0 ], r.min[ 1 ], r.min[ 2 ], 1.0f ),
    glm::vec4( r.min[ 0 ], r.min[ 1 ], r.max[ 2 ], 1.0f ),
    glm::vec4( r.min[ 0 ], r.max[ 1 ], r.min[ 2 ], 1.0f ),
    glm::vec4( r.min[ 0 ], r.max[ 1 ], r.max[ 2 ], 1.0f ),
    glm::vec4( r.max[ 0 ], r.min[ 1 ], r.min[ 2 ], 1.0f ),
    glm::vec4( r.max[ 0 ], r.min[ 1 ], r.max[ 2 ], 1.0f ),
    glm::vec4( r.max[ 0 ], r.max[ 1 ], r.min[ 2 ], 1.0f ),
    glm::vec4( r.max[ 0 ], r.max[ 1 ], r.max[ 2 ], 1.0f ),
  };
  aabb3 result;
  {
    auto v = l * vertex[ 0 ];
    v /= v[ 3 ];
    result.min[ 0 ] = v[ 0 ];
    result.min[ 1 ] = v[ 1 ];
    result.min[ 2 ] = v[ 2 ];
    result.max[ 0 ] = v[ 0 ];
    result.max[ 1 ] = v[ 1 ];
    result.max[ 2 ] = v[ 2 ];
  }
  for( unsigned int i = 1u; i != 8u; ++i ) {
    auto v = l * vertex[ i ];
    v /= v[ 3 ];
    result.min[ 0 ] = std::min( result.min[ 0 ], v[ 0 ] );
    result.min[ 1 ] = std::min( result.min[ 1 ], v[ 1 ] );
    result.min[ 2 ] = std::min( result.min[ 2 ], v[ 2 ] );
    result.max[ 0 ] = std::max( result.max[ 0 ], v[ 0 ] );
    result.max[ 1 ] = std::max( result.max[ 1 ], v[ 1 ] );
    result.max[ 2 ] = std::max( result.max[ 2 ], v[ 2 ] );
  }
  return result;
}

aabb4 operator*( const glm::mat4 &l, const aabb4 &r ) {
  aabb3 temp;
  temp.min[ 0 ] = r.min[ 0 ]/r.min[ 3 ];
  temp.min[ 1 ] = r.min[ 1 ]/r.min[ 3 ];
  temp.min[ 2 ] = r.min[ 2 ]/r.min[ 3 ];
  temp.max[ 0 ] = r.max[ 0 ]/r.max[ 3 ];
  temp.max[ 1 ] = r.max[ 1 ]/r.max[ 3 ];
  temp.max[ 2 ] = r.max[ 2 ]/r.max[ 3 ];
  temp = l * temp;
  aabb4 result;
  result.min[ 0 ] = temp.min[ 0 ];
  result.min[ 1 ] = temp.min[ 1 ];
  result.min[ 2 ] = temp.min[ 2 ];
  result.min[ 3 ] = 1.0f;
  result.max[ 0 ] = temp.max[ 0 ];
  result.max[ 1 ] = temp.max[ 1 ];
  result.max[ 2 ] = temp.max[ 2 ];
  result.max[ 3 ] = 1.0f;
  return result;
}

void to_json( nlohmann::json &dest, const aabb &src ) {
  dest = nlohmann::json::object();
  dest[ "min" ] = nlohmann::json::array();
  dest[ "min" ].push_back( src.min[ 0 ] );
  dest[ "min" ].push_back( src.min[ 1 ] );
  dest[ "min" ].push_back( src.min[ 2 ] );
  dest[ "max" ] = nlohmann::json::array();
  dest[ "max" ].push_back( src.max[ 0 ] );
  dest[ "max" ].push_back( src.max[ 1 ] );
  dest[ "max" ].push_back( src.max[ 2 ] );
}

void to_json( nlohmann::json &dest, const aabb4 &src ) {
  dest = nlohmann::json::object();
  dest[ "min" ] = nlohmann::json::array();
  dest[ "min" ].push_back( src.min[ 0 ] );
  dest[ "min" ].push_back( src.min[ 1 ] );
  dest[ "min" ].push_back( src.min[ 2 ] );
  dest[ "min" ].push_back( src.min[ 3 ] );
  dest[ "max" ] = nlohmann::json::array();
  dest[ "max" ].push_back( src.max[ 0 ] );
  dest[ "max" ].push_back( src.max[ 1 ] );
  dest[ "max" ].push_back( src.max[ 2 ] );
  dest[ "max" ].push_back( src.max[ 3 ] );
}

aabb4 generate_random_aabb(
  std::uniform_real_distribution<float> &dist,
  std::mt19937 &engine
) {
  aabb4 temp;
  temp.min[ 0 ] = dist( engine );
  temp.min[ 1 ] = dist( engine );
  temp.min[ 2 ] = dist( engine );
  temp.min[ 3 ] = dist( engine );
  temp.max[ 0 ] = dist( engine );
  temp.max[ 1 ] = dist( engine );
  temp.max[ 2 ] = dist( engine );
  temp.max[ 3 ] = dist( engine );
  if( temp.min[ 0 ] > temp.max[ 0 ] ) std::swap( temp.min[ 0 ], temp.max[ 0 ] );
  if( temp.min[ 1 ] > temp.max[ 1 ] ) std::swap( temp.min[ 1 ], temp.max[ 1 ] );
  if( temp.min[ 2 ] > temp.max[ 2 ] ) std::swap( temp.min[ 2 ], temp.max[ 2 ] );
  if( temp.min[ 3 ] > temp.max[ 3 ] ) std::swap( temp.min[ 3 ], temp.max[ 3 ] );
  return temp;
}

bool similar_aabb( const aabb4 &l, const aabb4 &r ) {
  for( unsigned int i = 0u; i != 4u; ++i )  {
    if( std::abs( l.min[ i ] - r.min[ i ] ) > 0.0001f ) return false;
  }
  for( unsigned int i = 0u; i != 4u; ++i )  {
    if( std::abs( l.max[ i ] - r.max[ i ] ) > 0.0001f ) return false;
  }
  return true;
}

}

