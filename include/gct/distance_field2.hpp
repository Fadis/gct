#ifndef GCT_DISTANCE_FIELD2_HPP
#define GCT_DISTANCE_FIELD2_HPP

#include <memory>
#include <gct/distance_field2_create_info.hpp>
#include <gct/voxel_image.hpp>
#include <gct/voxel_buffer.hpp>

namespace gct {

class command_buffer_recorder_t;
class compute;
class image_view_t;
class distance_field2 {
public:
  distance_field2(
    const distance_field2_create_info &ci
  );
  void clear(
    command_buffer_recorder_t &rec
  ) const;
  void operator()(
    command_buffer_recorder_t &rec
  ) const;
  const distance_field2_create_info &get_props() const {
    return props;
  }
  const voxel_image &get_working_image() const {
    return working_image;
  }
  const voxel_image &get_distance_field_image() const {
    return distance_field_image;
  }
private:
  distance_field2_create_info props;
  voxel_image working_image;
  voxel_image distance_field_image;
  std::shared_ptr< compute > voronoi;
  std::shared_ptr< compute > distance_field_;
};

}

#endif

