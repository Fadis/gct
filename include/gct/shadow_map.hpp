#ifndef GCT_SHADOW_MAP_HPP
#define GCT_SHADOW_MAP_HPP
#include <tuple>
#include <array>
#include <optional>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace gct {
  glm::vec3 scale_vec( const glm::vec4 &v );

  std::array< glm::vec3, 8u > get_projection_box(
    glm::mat4 projection_matrix,
    glm::mat4 camera_matrix,
    float /*light_distance*/
  );

  std::tuple< glm::mat4, glm::mat4, float, float, float > get_aabb_light_matrix(
    const glm::vec3 &min,
    const glm::vec3 &max,
    const glm::vec3 &light_pos
  );

  std::optional< std::tuple< glm::mat4, glm::mat4, float, float > > get_projection_light_matrix(
    const glm::mat4 &camera_projection_matrix,
    const glm::mat4 &camera_view_matrix,
    const glm::vec3 &light_pos,
    float distance_offset
  );
  void test_light_matrix(
    const glm::mat4 &camera_projection_matrix,
    const glm::mat4 &camera_view_matrix,
    const glm::mat4 &light_projection_matrix,
    const glm::mat4 &light_view_matrix
  );
}

#endif

