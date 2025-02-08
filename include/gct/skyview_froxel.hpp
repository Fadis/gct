#ifndef GCT_SKYVIEW_FROXEL_HPP
#define GCT_SKYVIEW_FROXEL_HPP
#include <memory>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <gct/skyview_froxel_create_info.hpp>

namespace gct {

class image_view_t;
class compute;
class command_buffer_recorder_t;
class mappable_buffer_t;
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
class skyview_froxel {
public:
  skyview_froxel(
    const skyview_froxel_create_info&
  );
  void operator()(
    command_buffer_recorder_t&,
    const skyview_froxel_param&
  ) const;
  [[nodiscard]] const std::shared_ptr< image_view_t > &get_output() const {
    return rendered;
  }
private:
  skyview_froxel_create_info props;
  std::shared_ptr< image_view_t > froxel;
  std::shared_ptr< image_view_t > rendered;
  std::shared_ptr< compute > generate;
  std::shared_ptr< compute > render;
  std::shared_ptr< mappable_buffer_t > state;
};

}

#endif

