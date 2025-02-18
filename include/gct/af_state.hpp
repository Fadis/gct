#ifndef GCT_AF_STATE_HPP
#define GCT_AF_STATE_HPP
#include <cmath>
#include <iostream>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>

namespace gct {

struct af_state {
  LIBGCT_SETTER( history )
  LIBGCT_SETTER( max_history )
  LIBGCT_SETTER( depth )
  LIBGCT_SETTER( znear )
  LIBGCT_SETTER( zfar )
  LIBGCT_SETTER( focal_distance )
  LIBGCT_SETTER( lens_size )
  LIBGCT_SETTER( sensor_size )
  LIBGCT_SETTER( apeture_rotation )
  LIBGCT_SETTER( visible_range )
  std::uint32_t history = 0u;
  std::uint32_t max_history = 60u;
  float depth = 0.0f;
  float znear = 0.0f;
  float zfar = 0.0f;
  float focal_distance = 0.05f;
  float lens_size = 0.050f / 2.8f; // 50mm lens with F2.8
  float sensor_size = 0.036f;
  float apeture_rotation = M_PI*13.f/180.f;
  float visible_range = 2.0f; // 2m
};

void to_json( nlohmann::json &dest, const af_state &src );
void from_json( const nlohmann::json &dest, af_state &src );

}

#endif

