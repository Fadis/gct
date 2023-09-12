#ifndef GCT_EPIPOLAR_LIGHT_SCATTERING_HPP
#define GCT_EPIPOLAR_LIGHT_SCATTERING_HPP
#include <memory>
#include <vector>
#include <gct/epipolar_light_scattering_create_info.hpp>

namespace gct {


struct epipolar_light_scattering_texcoord_parameters_t {
  LIBGCT_SETTER( stride )
  LIBGCT_SETTER( vertex_offset )
  LIBGCT_SETTER( near )
  LIBGCT_SETTER( far )
  std::uint32_t stride;
  std::uint32_t vertex_offset;
  float near;
  float far;
};


struct epipolar_light_scattering_parameters_t {
  LIBGCT_SETTER( eye_pos )
  LIBGCT_SETTER( light_pos )
  LIBGCT_SETTER( g )
  LIBGCT_SETTER( fog_color )
  LIBGCT_SETTER( light_energy )
  LIBGCT_SETTER( stride )
  LIBGCT_SETTER( vertex_offset )
  glm::vec4 eye_pos;
  glm::vec4 light_pos;
  glm::vec4 g;
  glm::vec4 fog_color;
  float light_energy;
  std::uint32_t stride;
  std::uint32_t vertex_offset;
};

class gbuffer;
class image_view_t;
class graphics;
class epipolar_mesh;
class compute;
class epipolar_light_scattering {
public:
  epipolar_light_scattering(
    const epipolar_light_scattering_create_info&
  );
  void init(
    command_buffer_recorder_t&
  ) const;
  void operator()(
    command_buffer_recorder_t&,
    unsigned int,
    const epipolar_light_scattering_parameters_t&,
    const epipolar_light_scattering_texcoord_parameters_t&,
    const glm::vec2 &light_pos_on_screen
  ) const;
  const std::vector< std::shared_ptr< image_view_t > > &get_output() const;
private:
  epipolar_light_scattering_create_info props;
  std::shared_ptr< gbuffer > output;
  std::vector< std::shared_ptr< image_view_t > > volumetric_light;
  std::vector< std::shared_ptr< image_view_t > > volumetric_light_texcoord;
  std::vector< std::shared_ptr< image_view_t > > gauss_temp;
  std::shared_ptr< graphics > draw_mesh;
  std::shared_ptr< epipolar_mesh > mesh;
  std::shared_ptr< compute > calc_volumetric_light;
  std::shared_ptr< compute > clear_volumetric_light_texcoord;
  std::shared_ptr< compute > calc_volumetric_light_texcoord;
  std::shared_ptr< compute > fill_volumetric_light_texcoord;
  std::shared_ptr< compute > hgauss;
  std::shared_ptr< compute > vgauss;
  std::uint32_t stride;
  std::uint32_t vertex_offset;
};

}

#endif

