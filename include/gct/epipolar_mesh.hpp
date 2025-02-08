#ifndef GCT_EPIPOLAR_MESH_HPP
#define GCT_EPIPOLAR_MESH_HPP

#include <memory>
#include <vector>
#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/mat2x2.hpp>
#include <gct/epipolar_mesh_create_info.hpp>

namespace gct {

class buffer_t;
class mappable_buffer_t;
class command_buffer_recorder_t;
class compute;
class epipolar_mesh {
  struct push_constants {
    LIBGCT_SETTER( local_to_screen_matrix );
    LIBGCT_SETTER( light_pos );
    LIBGCT_SETTER( stride )
    LIBGCT_SETTER( vertex_offset )
    LIBGCT_SETTER( texcoord_offset )
    glm::mat2 local_to_screen_matrix;
    glm::vec2 light_pos;
    std::uint32_t stride;
    std::uint32_t vertex_offset;
    std::uint32_t texcoord_offset;
  };
public:
  epipolar_mesh(
    const epipolar_mesh_create_info&
  );
  void init(
    command_buffer_recorder_t &rec
  ) const;
  void operator()(
    command_buffer_recorder_t &rec,
    unsigned int,
    const glm::vec2 &center
  ) const;
  [[nodiscard]] const std::shared_ptr< buffer_t > &get_index_buffer() const {
    return index_buffer;
  }
  [[nodiscard]] const std::vector< std::shared_ptr< mappable_buffer_t > > &get_vertex_buffer() const {
    return vertex_buffer;
  }
  [[nodiscard]] std::uint32_t get_index_count() const {
    return ( props.pole_count - 1u ) * ( props.vertex_count - 1u ) * 6u;
  }
private:
  epipolar_mesh_create_info props;
  mutable push_constants pc;
  std::vector< std::shared_ptr< mappable_buffer_t > > vertex_buffer;
  mutable std::shared_ptr< mappable_buffer_t > index_buffer_staging;
  std::shared_ptr< buffer_t > index_buffer;
  std::vector< compute > generate;
};

}

#endif

