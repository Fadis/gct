#ifndef GCT_LENS_FLARE_HPP
#define GCT_LENS_FLARE_HPP
#include <cmath>
#include <memory>
#include <glm/mat4x4.hpp>
#include <gct/setter.hpp>
#include <gct/aabb.hpp>
#include <gct/lens_flare_create_info.hpp>

namespace gct {
class image_view_t;
class graphics;
class gbuffer;
class command_buffer_recorder_t;
class lens_flare {
  struct push_constant_type {
    LIBGCT_SETTER( texture_id )
    LIBGCT_SETTER( lens_size )
    LIBGCT_SETTER( sensor_size )
    glm::vec2 sensor_size = glm::vec2( 0.036f, 0.036f*9.0f/16.0f );
    std::uint32_t texture_id = 0;
    float lens_size = 0.050f / 2.8f; // 50mm lens with F2.8
  };
public:
  lens_flare(
    const lens_flare_create_info&
  );
  void operator()(
    command_buffer_recorder_t&
  );
  const std::shared_ptr< image_view_t > &get_image_view() const;
private:
  lens_flare_create_info props;
  std::shared_ptr< gbuffer > output;
  std::shared_ptr< graphics > pipeline;
  std::shared_ptr< mappable_buffer_t > point_mesh;
};

}

#endif

