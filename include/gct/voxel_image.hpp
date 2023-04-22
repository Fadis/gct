#ifndef GCT_VOXEL_IMAGE_HPP
#define GCT_VOXEL_IMAGE_HPP
#include <array>
#include <memory>
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include <gct/render_pass_begin_info.hpp>

namespace gct {
class allocator_t;
class image_view_t;
class render_pass_t;
class framebuffer_t;
class voxel_image {
public:
  voxel_image(
    const std::shared_ptr< allocator_t > &allocator,
    unsigned int size,
    vk::Format format
  );
  const std::shared_ptr< gct::image_view_t > &get_image() const {
    return image;
  }
  const std::shared_ptr< gct::render_pass_t > &get_render_pass() const {
    return render_pass;
  }
  const std::shared_ptr< gct::framebuffer_t > &get_framebuffer() const {
    return framebuffer;
  }
  const gct::render_pass_begin_info_t &get_render_pass_begin_info() const {
    return rpbi;
  }
  const vk::Viewport &get_viewport() const {
    return viewport;
  }
  const vk::Rect2D &get_scissor() const {
    return scissor;
  }
private:
  std::shared_ptr< gct::image_view_t > image;
  std::shared_ptr< gct::render_pass_t > render_pass;
  std::shared_ptr< gct::framebuffer_t > framebuffer;
  gct::render_pass_begin_info_t rpbi;
  vk::Viewport viewport;
  vk::Rect2D scissor;
};

class voxel_matrix {
public:
  voxel_matrix(
    glm::vec3 min,
    glm::vec3 max
  );
  const glm::mat4 &get_input_projection( unsigned int i ) const {
    return input_projection[ i % input_projection.size() ];
  }
  const glm::mat4 &get_output_projection() const {
    return output_projection;
  }
  const glm::mat4 &get_inversed_output_projection() const {
    return inversed_output_projection;
  }
private:
  glm::mat4 output_projection;
  glm::mat4 inversed_output_projection;
  std::array< glm::mat4, 3u > input_projection;
};


}

#endif

