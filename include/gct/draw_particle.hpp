#ifndef GCT_DRAW_PARTICLE_HPP
#define GCT_DRAW_PARTICLE_HPP
#include <memory>
#include <vector>
#include <cstdint>
#include <gct/vertex_attributes.hpp>
#include <gct/draw_particle_create_info.hpp>

namespace gct {


class gbuffer;
class image_view_t;
class graphics;
class mappable_buffer_t;
class particle_buffer;
class draw_particle {
public:
  draw_particle(
    const draw_particle_create_info&
  );
  void operator()(
    command_buffer_recorder_t&,
    unsigned int,
    const particle_buffer&
  ) const;
  [[nodiscard]] const std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t > &get_vamap() const {
    return vamap;
  }
  [[nodiscard]] std::uint32_t get_stride() const {
    return stride;
  }
  [[nodiscard]] const std::shared_ptr< graphics > get_graphics() const {
    return draw;
  }
private:
  draw_particle_create_info props;
  std::shared_ptr< graphics > draw;
  std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t > vamap;
  std::uint32_t stride;
};

}

#endif

