#include <algorithm>
#include <nlohmann/json.hpp>
#include <gct/aabb.hpp>

namespace gct {

aabb::aabb( const aabb4 &a ) {
  min[ 0 ] = a.min[ 0 ]/a.min[ 3 ];
  min[ 1 ] = a.min[ 1 ]/a.min[ 3 ];
  min[ 2 ] = a.min[ 2 ]/a.min[ 3 ];
  max[ 0 ] = a.max[ 0 ]/a.max[ 3 ];
  max[ 1 ] = a.max[ 1 ]/a.max[ 3 ];
  max[ 2 ] = a.max[ 2 ]/a.max[ 3 ];
}

aabb4::aabb4( const aabb &a ) {
  min[ 0 ] = a.min[ 0 ];
  min[ 1 ] = a.min[ 1 ];
  min[ 2 ] = a.min[ 2 ];
  min[ 3 ] = 1.0f;
  max[ 0 ] = a.max[ 0 ];
  max[ 1 ] = a.max[ 1 ];
  max[ 2 ] = a.max[ 2 ];
  max[ 3 ] = 1.0f;
}

aabb::aabb( const glm::vec3 &min_, const glm::vec3 &max_ ) :
  min( min_ ),
  max( max_ ) {}

aabb4::aabb4( const glm::vec4 &min_, const glm::vec4 &max_ ) :
  min( min_ ),
  max( max_ ) {}

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
  return aabb( l ) & aabb( r );
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
  return aabb( l ) | aabb( r );
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
  return l * aabb( r );
  aabb3 temp;
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
float area( const aabb3 &a ) {
  return
    ( a.max[ 0 ] - a.min[ 0 ] ) *
    ( a.max[ 1 ] - a.min[ 1 ] ) *
    ( a.max[ 2 ] - a.min[ 2 ] );
}
float area( const aabb4 &a ) {
  return area( aabb( a ) );
}
bool operator&&( const aabb3 &l, const aabb3 &r ) {
  return area( l & r ) != 0.0f;
}
bool operator&&( const aabb4 &l, const aabb4 &r ) {
  return area( l & r ) != 0.0f;
}

bool operator==( const aabb3 &l, const aabb3 &r ) {
  return l.min == r.min && l.max == r.max;
}
bool operator==( const aabb4 &l, const aabb4 &r ) {
  return l.min == r.min && l.max == r.max;
}
bool operator!=( const aabb3 &l, const aabb3 &r ) {
  return l.min != r.min || l.max != r.max;
}
bool operator!=( const aabb4 &l, const aabb4 &r ) {
  return l.min != r.min || l.max != r.max;
}

std::string to_string( const aabb3 &v ) {
  std::string dest = "(";
  dest += std::to_string( v.min.x );
  dest += ",";
  dest += std::to_string( v.min.y );
  dest += ",";
  dest += std::to_string( v.min.z );
  dest += ")-(";
  dest += std::to_string( v.max.x );
  dest += ",";
  dest += std::to_string( v.max.y );
  dest += ",";
  dest += std::to_string( v.max.z );
  dest += ")";
  return dest;
}
std::string to_string( const aabb4 &v ) {
  std::string dest = "(";
  dest += std::to_string( v.min.x );
  dest += ",";
  dest += std::to_string( v.min.y );
  dest += ",";
  dest += std::to_string( v.min.z );
  dest += ",";
  dest += std::to_string( v.min.w );
  dest += ")-(";
  dest += std::to_string( v.max.x );
  dest += ",";
  dest += std::to_string( v.max.y );
  dest += ",";
  dest += std::to_string( v.max.z );
  dest += ",";
  dest += std::to_string( v.max.w );
  dest += ")";
  return dest;
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

