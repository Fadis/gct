#ifndef GCT_SKYVIEW_FROXEL_PARAM_HPP
#define GCT_SKYVIEW_FROXEL_PARAM_HPP
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>

namespace gct {

struct skyview_froxel_param {
  LIBGCT_SETTER( world_to_screen )
  LIBGCT_SETTER( screen_to_world )
  LIBGCT_SETTER( sigma_ma )
  LIBGCT_SETTER( sigma_oa )
  LIBGCT_SETTER( sigma_rs )
  LIBGCT_SETTER( sigma_ms )
  LIBGCT_SETTER( eye_pos )
  LIBGCT_SETTER( light_pos )
  LIBGCT_SETTER( g )
  LIBGCT_SETTER( ground_radius )
  LIBGCT_SETTER( top_radius )
  LIBGCT_SETTER( altitude )
  LIBGCT_SETTER( light_energy )
  glm::mat4 world_to_screen;
  glm::mat4 screen_to_world;
  glm::vec4 sigma_ma;
  glm::vec4 sigma_oa;
  glm::vec4 sigma_rs;
  glm::vec4 sigma_ms;
  glm::vec4 eye_pos;
  glm::vec4 light_pos;
  float g;
  float ground_radius;
  float top_radius;
  float altitude;
  float light_energy;
};

void to_json( nlohmann::json &dest, const skyview_froxel_param &src );

}

#endif

