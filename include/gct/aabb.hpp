#ifndef GCT_AABB_HPP
#define GCT_AABB_HPP

#include <random>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>

namespace gct {
struct aabb {
  LIBGCT_SETTER( min )
  LIBGCT_SETTER( max )
  glm::vec3 min;
  glm::vec3 max;
};

using aabb3 = aabb;

struct aabb4 {
  LIBGCT_SETTER( min )
  LIBGCT_SETTER( max )
  glm::vec4 min;
  glm::vec4 max;
};

aabb3 operator&( const aabb3&, const aabb3& );
aabb4 operator&( const aabb4&, const aabb4& );
aabb3 operator|( const aabb3&, const aabb3& );
aabb4 operator|( const aabb4&, const aabb4& );

aabb3 operator*( const glm::mat4 &, const aabb3& );
aabb4 operator*( const glm::mat4 &, const aabb4& );

void to_json( nlohmann::json&, const aabb& );
void to_json( nlohmann::json&, const aabb4& );

aabb4 generate_random_aabb(
  std::uniform_real_distribution<float> &dist,
  std::mt19937 &engine
);
bool similar_aabb( const aabb4 &l, const aabb4 &r );

}

#endif

