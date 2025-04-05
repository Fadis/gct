#ifndef GCT_SKYVIEW_FROXEL2_PARAM_HPP
#define GCT_SKYVIEW_FROXEL2_PARAM_HPP
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>

namespace gct {

struct skyview_froxel2_param {
  LIBGCT_SETTER( sigma )
  LIBGCT_SETTER( g )
  LIBGCT_SETTER( ground_radius )
  LIBGCT_SETTER( top_radius )
  LIBGCT_SETTER( altitude )
  std::uint32_t sigma;
  float ground_radius = 6360000.f;
  float top_radius = 6460000.f;
  float altitude = 100.f;
  float g = 0.8f;
};

void to_json( nlohmann::json &dest, const skyview_froxel2_param &src );

glm::mat4 get_default_skyview_sigma();

}

#endif

