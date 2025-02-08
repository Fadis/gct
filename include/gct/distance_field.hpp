#ifndef GCT_DISTANCE_FIELD_HPP
#define GCT_DISTANCE_FIELD_HPP

#include <gct/distance_field_create_info.hpp>
#include <gct/voxel_image.hpp>
#include <gct/property.hpp>

namespace gct {

class command_buffer_recorder_t;
class compute;
class image_view_t;
class distance_field :
  public property< distance_field_create_info > {
public:
  distance_field(
    const distance_field_create_info &ci
  );
  void clear(
    command_buffer_recorder_t &rec
  ) const;
  void generate(
    command_buffer_recorder_t &rec
  ) const;
  [[nodiscard]] const voxel_image &get_working_image() const {
    return working_image;
  }
  [[nodiscard]] const voxel_image &get_distance_field_image() const {
    return distance_field_image;
  }
private:
  voxel_image working_image;
  voxel_image distance_field_image;
  std::shared_ptr< compute > clear_;
  std::shared_ptr< compute > voronoi;
  std::shared_ptr< compute > distance_field_;
};

}

#endif

