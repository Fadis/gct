#ifndef GCT_SKYVIEW_HPP
#define GCT_SKYVIEW_HPP
#include <cmath>
#include <memory>
#include <glm/vec4.hpp>
#include <gct/setter.hpp>
#include <gct/skyview_create_info.hpp>

namespace gct {

struct skyview_parameter {
  LIBGCT_SETTER( sigma_ma )
  LIBGCT_SETTER( sigma_oa )
  LIBGCT_SETTER( sigma_rs )
  LIBGCT_SETTER( sigma_ms )
  LIBGCT_SETTER( ground_radius )
  LIBGCT_SETTER( top_radius )
  LIBGCT_SETTER( altitude )
  LIBGCT_SETTER( light_energy )
  LIBGCT_SETTER( g )
  LIBGCT_SETTER( sun_angle )
  glm::vec4 sigma_ma = glm::vec4( 4.40e-6, 4.40e-6, 4.40e-6, 0.0 );
  glm::vec4 sigma_oa = glm::vec4( 0.650e-6, 1.881e-6, 0.085e-6, 0.0 );
  glm::vec4 sigma_rs = glm::vec4( 5.802e-6, 13.558e-6, 33.1e-6, 0.0 );
  glm::vec4 sigma_ms = glm::vec4( 3.996e-6, 3.996e-6, 3.996e-6, 0.0 );
  float ground_radius = 6360000.f;
  float top_radius = 6460000.f;
  float altitude = 100.f;
  float light_energy = 20.f;
  float g = 0.8f;
  float sun_angle = M_PI/3.0;
};

class image_view_t;
class compute;
class command_buffer_recorder_t;
class skyview {
  struct transmittance_pc {
    LIBGCT_SETTER( sigma_ma )
    LIBGCT_SETTER( sigma_oa )
    LIBGCT_SETTER( sigma_rs )
    LIBGCT_SETTER( sigma_ms )
    LIBGCT_SETTER( ground_radius )
    LIBGCT_SETTER( top_radius )
    glm::vec4 sigma_ma;
    glm::vec4 sigma_oa;
    glm::vec4 sigma_rs;
    glm::vec4 sigma_ms;
    float ground_radius;
    float top_radius;
  };
  struct multiscat_pc {
    LIBGCT_SETTER( sigma_ma )
    LIBGCT_SETTER( sigma_oa )
    LIBGCT_SETTER( sigma_rs )
    LIBGCT_SETTER( sigma_ms )
    LIBGCT_SETTER( g )
    LIBGCT_SETTER( ground_radius )
    LIBGCT_SETTER( top_radius )
    LIBGCT_SETTER( light_energy )
    glm::vec4 sigma_ma;
    glm::vec4 sigma_oa;
    glm::vec4 sigma_rs;
    glm::vec4 sigma_ms;
    float g;
    float ground_radius;
    float top_radius;
    float light_energy;
  };
  struct skyview_pc {
    LIBGCT_SETTER( sigma_ma )
    LIBGCT_SETTER( sigma_oa )
    LIBGCT_SETTER( sigma_rs )
    LIBGCT_SETTER( sigma_ms )
    LIBGCT_SETTER( light_pos )
    LIBGCT_SETTER( g )
    LIBGCT_SETTER( ground_radius )
    LIBGCT_SETTER( top_radius )
    LIBGCT_SETTER( altitude )
    LIBGCT_SETTER( light_energy )
    glm::vec4 sigma_ma;
    glm::vec4 sigma_oa;
    glm::vec4 sigma_rs;
    glm::vec4 sigma_ms;
    glm::vec4 light_pos;
    float g;
    float ground_radius;
    float top_radius;
    float altitude;
    float light_energy;
  };
public:
  skyview(
    const skyview_create_info&
  );
  void operator()(
    command_buffer_recorder_t&,
    const skyview_parameter &
  ) const;
  const std::shared_ptr< image_view_t > &get_output() const {
    return output;
  }
  const std::shared_ptr< image_view_t > &get_transmittance() const {
    return transmittance;
  }
  const std::shared_ptr< image_view_t > &get_multiscat() const {
    return multiscat;
  }
private:
  skyview_create_info props;
  std::shared_ptr< image_view_t > transmittance;
  std::shared_ptr< image_view_t > multiscat;
  std::shared_ptr< image_view_t > output;
  std::shared_ptr< compute > generate_transmittance;
  std::shared_ptr< compute > generate_multiscat;
  std::shared_ptr< compute > generate_skyview;
};

}

#endif

