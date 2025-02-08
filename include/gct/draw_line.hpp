#ifndef GCT_DRAW_LINE_HPP
#define GCT_DRAW_LINE_HPP
#include <memory>
#include <glm/mat4x4.hpp>
#include <gct/setter.hpp>
#include <gct/aabb.hpp>
#include <gct/draw_line_create_info.hpp>

namespace gct {
class image_view_t;
class graphics;
class gbuffer;
class command_buffer_recorder_t;
class query_pool_t;
using draw_line_id_t = std::uint32_t;
class draw_line {
  struct push_constant_type {
    LIBGCT_SETTER( matrix )
    glm::mat4 matrix;
  };
public:
  draw_line(
    const draw_line_create_info&
  );
  void push( const glm::vec3 &begin, const glm::vec3 &end );
  void operator()(
    command_buffer_recorder_t&,
    const glm::mat4 &matrix
  );
  void reset();
  [[nodiscard]] const std::shared_ptr< image_view_t > &get_image_view() const;
  [[nodiscard]] bool empty() const {
    return pushed.empty();
  }
private:
  draw_line_create_info props;
  std::shared_ptr< gbuffer > output;
  std::shared_ptr< graphics > pipeline;
  std::vector< glm::vec4 > pushed;
  std::shared_ptr< mappable_buffer_t > vertex_buffer;
  bool dirty = false;
};

}

#endif

