#include <algorithm>
#include <glm/fwd.hpp>
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
    v /= std::abs( v[ 3 ] );
    result.min[ 0 ] = v[ 0 ];
    result.min[ 1 ] = v[ 1 ];
    result.min[ 2 ] = v[ 2 ];
    result.max[ 0 ] = v[ 0 ];
    result.max[ 1 ] = v[ 1 ];
    result.max[ 2 ] = v[ 2 ];
  }
  for( unsigned int i = 1u; i != 8u; ++i ) {
    auto v = l * vertex[ i ];
    v /= std::abs( v[ 3 ] );
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

bool contain( const aabb3 &range, const glm::vec3 &p ) {
  if( range.min.x > p.x ) return false;
  if( range.min.y > p.y ) return false;
  if( range.min.z > p.z ) return false;
  if( range.max.x < p.x ) return false;
  if( range.max.x < p.y ) return false;
  if( range.max.x < p.z ) return false;
  return true;
}
bool contain( const aabb3 &range, const glm::vec4 &p ) {
  return contain( range, glm::vec3( p.x/p.w, p.y/p.w, p.z/p.w ) );
}
bool contain( const aabb4 &range, const glm::vec3 &p ) {
  return contain( aabb3( range ), p );
}
bool contain( const aabb4 &range, const glm::vec4 &p ) {
  return contain( aabb3( range ), glm::vec3( p.x/p.w, p.y/p.w, p.z/p.w ) );
}
bool contain( const aabb3 &range, const std::vector< glm::vec3 >&p ) {
  return std::find_if(
    p.begin(), p.end(), [&range]( const auto &p ) {
      return !contain( range, p );
    }
  ) == p.end();
}
bool contain( const aabb3 &range, const std::vector< glm::vec4 >&p ) {
  return std::find_if(
    p.begin(), p.end(), [&range]( const auto &p ) {
      return !contain( range, p );
    }
  ) == p.end();
}
bool contain( const aabb4 &range, const std::vector< glm::vec3 >&p ) {
  return std::find_if(
    p.begin(), p.end(), [&range]( const auto &p ) {
      return !contain( range, p );
    }
  ) == p.end();
}
bool contain( const aabb4 &range, const std::vector< glm::vec4 >&p ) {
  return std::find_if(
    p.begin(), p.end(), [&range]( const auto &p ) {
      return !contain( range, p );
    }
  ) == p.end();
}
bool outside( const aabb3 &range, const std::vector< glm::vec3 >&p ) {
  return std::find_if(
    p.begin(), p.end(), [&range]( const auto &p ) {
      return contain( range, p );
    }
  ) == p.end();
}
bool outside( const aabb3 &range, const std::vector< glm::vec4 >&p ) {
  return std::find_if(
    p.begin(), p.end(), [&range]( const auto &p ) {
      return contain( range, p );
    }
  ) == p.end();
}
bool outside( const aabb4 &range, const std::vector< glm::vec3 >&p ) {
  return std::find_if(
    p.begin(), p.end(), [&range]( const auto &p ) {
      return contain( range, p );
    }
  ) == p.end();
}
bool outside( const aabb4 &range, const std::vector< glm::vec4 >&p ) {
  return std::find_if(
    p.begin(), p.end(), [&range]( const auto &p ) {
      return contain( range, p );
    }
  ) == p.end();
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

std::vector< glm::vec4 > to_vertices( const aabb &box ) {
  return std::vector< glm::vec4 >{
    glm::vec4( box.min.x, box.min.y, box.min.z, 1.f ),
    glm::vec4( box.min.x, box.min.y, box.max.z, 1.f ),
    glm::vec4( box.min.x, box.max.y, box.min.z, 1.f ),
    glm::vec4( box.min.x, box.max.y, box.max.z, 1.f ),
    glm::vec4( box.max.x, box.min.y, box.min.z, 1.f ),
    glm::vec4( box.max.x, box.min.y, box.max.z, 1.f ),
    glm::vec4( box.max.x, box.max.y, box.min.z, 1.f ),
    glm::vec4( box.max.x, box.max.y, box.max.z, 1.f )
  };
}

std::vector< glm::vec4 > to_vertices( const aabb4 &box ) {
  return to_vertices( aabb( box ) );
}

aabb from_vertices( const std::vector< glm::vec4 > &points ) {
  if( points.empty() ) {
    return aabb()
      .set_min( glm::vec3( 0.f, 0.f, 0.f ) )
      .set_max( glm::vec3( 0.f, 0.f, 0.f ) );
  }
  aabb temp;
  temp.min.x = points[ 0 ].x/points[ 0 ].w;
  temp.min.y = points[ 0 ].y/points[ 0 ].w;
  temp.min.z = points[ 0 ].z/points[ 0 ].w;
  temp.max.x = points[ 0 ].x/points[ 0 ].w;
  temp.max.y = points[ 0 ].y/points[ 0 ].w;
  temp.max.z = points[ 0 ].z/points[ 0 ].w;
  for( unsigned int i = 1u; i != points.size(); ++i ) {
    temp.min.x = std::min( temp.min.x, points[ i ].x/points[ i ].w );
    temp.min.y = std::min( temp.min.y, points[ i ].y/points[ i ].w );
    temp.min.z = std::min( temp.min.z, points[ i ].z/points[ i ].w );
    temp.max.x = std::max( temp.max.x, points[ i ].x/points[ i ].w );
    temp.max.y = std::max( temp.max.y, points[ i ].y/points[ i ].w );
    temp.max.z = std::max( temp.max.z, points[ i ].z/points[ i ].w );
  }
  return temp;
}

aabb get_unit_aabb() {
  aabb temp;
  temp.min = glm::vec3( -1.f, -1.f, -1.f );
  temp.max = glm::vec3(  1.f,  1.f,  1.f );
  return temp;
}

aabb4 get_unit_aabb4() {
  aabb4 temp;
  temp.min = glm::vec4( -1.f, -1.f, -1.f,  1.f );
  temp.max = glm::vec4(  1.f,  1.f,  1.f,  1.f );
  return temp;
}

aabb4 create_cube_area(
  glm::vec3 pos,
  float size
) {
  gct::aabb4 near_box;
  near_box.min = glm::vec4( pos - glm::vec3( size, size, size ), 1.f );
  near_box.max = glm::vec4( pos + glm::vec3( size, size, size ), 1.f );
  return near_box;
}

glm::vec3 get_center( const aabb3 &a ) {
  return glm::vec3(
    ( a.min.x + a.max.x ) / 2.f,
    ( a.min.y + a.max.y ) / 2.f,
    ( a.min.z + a.max.z ) / 2.f
  );
}

float surface_area( const aabb3 &a ) {
  return 
    ( a.max.x - a.min.x ) * ( a.max.y - a.min.y ) * 2.f +
    ( a.max.y - a.min.y ) * ( a.max.z - a.min.z ) * 2.f +
    ( a.max.z - a.min.z ) * ( a.max.x - a.min.x ) * 2.f;
}
float surface_area( const aabb4 &a ) {
  return surface_area( aabb3( a ) );
}

float get_inner_sphere_radius( const aabb3 &a ) {
  return std::min( std::min( a.max.x - a.min.x, a.max.y - a.min.y ), a.max.z - a.min.z );
}
float get_inner_sphere_radius( const aabb4 &a ) {
  return get_inner_sphere_radius( aabb3( a ) );
}

std::pair< float, float > intersect(
  const aabb &box,
  const glm::vec3 &p0,
  const glm::vec3 &p1
) {
  glm::vec3 tmin = ( box.min - p0 ) / ( p1 - p0 );
  glm::vec3 tmax = ( box.max - p0 ) / ( p1 - p0 );
  glm::vec3 t1 = glm::min( tmin, tmax );
  glm::vec3 t2 = glm::max( tmin, tmax );
  float tnear = std::max( std::max( t1.x, t1.y ), t1.z );
  float tfar = std::min( std::min( t2.x, t2.y ), t2.z );
  return std::make_pair( tnear, tfar );
}
std::pair< float, float > intersect(
  const aabb &box,
  const glm::vec4 &p0,
  const glm::vec4 &p1
) {
  glm::vec4 p0t = p0;
  glm::vec4 p1t = p1;
  p0t /= p0t.w;
  p1t /= p1t.w;
  glm::vec3 p03( p0t.x, p0t.y, p0t.z );
  glm::vec3 p13( p1t.x, p1t.y, p1t.z );
  return intersect( box, p03, p13 );
}

}

