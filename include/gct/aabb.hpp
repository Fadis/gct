#ifndef GCT_AABB_HPP
#define GCT_AABB_HPP

#include <glm/ext/vector_float4.hpp>
#include <random>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>

namespace gct {
struct aabb4;

struct aabb {
  aabb() {}
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
  aabb4() {}
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

std::string to_string( const aabb3& );
std::string to_string( const aabb4& );

void to_json( nlohmann::json&, const aabb& );
void to_json( nlohmann::json&, const aabb4& );

aabb4 generate_random_aabb(
  std::uniform_real_distribution<float> &dist,
  std::mt19937 &engine
);
bool similar_aabb( const aabb4 &l, const aabb4 &r );

}

#endif

