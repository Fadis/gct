#ifndef GCT_SKYVIEW_FROXEL_HPP
#define GCT_SKYVIEW_FROXEL_HPP
#include <memory>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <gct/skyview_froxel_create_info.hpp>
#include <gct/skyview_froxel_param.hpp>

namespace gct {

class image_view_t;
class compute;
class command_buffer_recorder_t;
class mappable_buffer_t;
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

