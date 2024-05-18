#ifndef GCT_AABB_HPP
#define GCT_AABB_HPP

#include <vector>
#include <random>
#include <functional>
#include <glm/ext/vector_float4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>

namespace gct {
struct aabb4;

struct aabb {
  aabb() :
    min( 0.f, 0.f, 0.f ),
    max( 0.f, 0.f, 0.f ) {}
  aabb( const glm::vec3 &min_, const glm::vec3 &max_ );
  aabb( const aabb4& );
  aabb( const aabb& ) = default;
  aabb &operator=( const aabb& ) = default;
  LIBGCT_SETTER( min )
  LIBGCT_SETTER( max )
  glm::vec3 min;
  glm::vec3 max;
};

using aabb3 = aabb;

struct aabb4 {
  aabb4() :
    min( 0.f, 0.f, 0.f, 1.f ),
    max( 0.f, 0.f, 0.f, 1.f ) {}
  aabb4( const glm::vec4 &min_, const glm::vec4 &max_ );
  aabb4( const aabb& );
  aabb4( const aabb4& ) = default;
  aabb4 &operator=( const aabb4& ) = default;
  LIBGCT_SETTER( min )
  LIBGCT_SETTER( max )
  glm::vec4 min;
  glm::vec4 max;
};

bool operator&&( const aabb3&, const aabb3& );
bool operator&&( const aabb4&, const aabb4& );
aabb3 operator&( const aabb3&, const aabb3& );
aabb4 operator&( const aabb4&, const aabb4& );
aabb3 operator|( const aabb3&, const aabb3& );
aabb4 operator|( const aabb4&, const aabb4& );

aabb3 operator*( const glm::mat4 &, const aabb3& );
aabb4 operator*( const glm::mat4 &, const aabb4& );

bool operator==( const aabb3&, const aabb3& );
bool operator==( const aabb4&, const aabb4& );
bool operator!=( const aabb3&, const aabb3& );
bool operator!=( const aabb4&, const aabb4& );

bool contain( const aabb3&, const glm::vec3& );
bool contain( const aabb3&, const glm::vec4& );
bool contain( const aabb4&, const glm::vec3& );
bool contain( const aabb4&, const glm::vec4& );
bool contain( const aabb3&, const std::vector< glm::vec3 >& );
bool contain( const aabb3&, const std::vector< glm::vec4 >& );
bool contain( const aabb4&, const std::vector< glm::vec3 >& );
bool contain( const aabb4&, const std::vector< glm::vec4 >& );
bool outside( const aabb3&, const std::vector< glm::vec3 >& );
bool outside( const aabb3&, const std::vector< glm::vec4 >& );
bool outside( const aabb4&, const std::vector< glm::vec3 >& );
bool outside( const aabb4&, const std::vector< glm::vec4 >& );

std::string to_string( const aabb3& );
std::string to_string( const aabb4& );

void to_json( nlohmann::json&, const aabb& );
void to_json( nlohmann::json&, const aabb4& );

aabb4 generate_random_aabb(
  std::uniform_real_distribution<float> &dist,
  std::mt19937 &engine
);
bool similar_aabb( const aabb4 &l, const aabb4 &r );

std::vector< glm::vec4 > to_vertices( const aabb& );
std::vector< glm::vec4 > to_vertices( const aabb4& );
aabb from_vertices( const std::vector< glm::vec4 >& );

aabb get_unit_aabb();
aabb4 get_unit_aabb4();

float area( const aabb3 &a );
float area( const aabb4 &a );
float surface_area( const aabb3 &a );
float surface_area( const aabb4 &a );
float get_inner_sphere_radius( const aabb3 &a );
float get_inner_sphere_radius( const aabb4 &a );
bool operator&&( const aabb3 &l, const aabb3 &r );
bool operator&&( const aabb4 &l, const aabb4 &r );

aabb4 create_cube_area(
  glm::vec3 pos,
  float size
);

glm::vec3 get_center( const aabb3& );
std::pair< float, float > intersect(
  const aabb &box,
  const glm::vec3 &p0,
  const glm::vec3 &p1
);
std::pair< float, float > intersect(
  const aabb &box,
  const glm::vec4 &p0,
  const glm::vec4 &p1
);

}

namespace std {
  template<>
  struct hash< gct::aabb3 > {
    std::size_t operator()( const gct::aabb3 &v ) const {
      return
        std::hash< float >()( v.min.x ) ^
        std::hash< float >()( v.min.y ) ^
        std::hash< float >()( v.min.z ) ^
        std::hash< float >()( v.max.x ) ^
        std::hash< float >()( v.max.y ) ^
        std::hash< float >()( v.max.z );
    }
  };
  template<>
  struct hash< gct::aabb4 > {
    std::size_t operator()( const gct::aabb4 &v ) const {
      return
        std::hash< float >()( v.min.x ) ^
        std::hash< float >()( v.min.y ) ^
        std::hash< float >()( v.min.z ) ^
        std::hash< float >()( v.min.w ) ^
        std::hash< float >()( v.max.x ) ^
        std::hash< float >()( v.max.y ) ^
        std::hash< float >()( v.max.z ) ^
        std::hash< float >()( v.max.w );
    }
  };
}

#endif

