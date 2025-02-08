#ifndef GCT_VOXEL_FRAMEBUFFER_HPP
#define GCT_VOXEL_FRAMEBUFFER_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include <gct/render_pass_begin_info.hpp>

namespace gct {
class allocator_t;
class render_pass_t;
class framebuffer_t;
class voxel_framebuffer {
public:
  voxel_framebuffer(
    const std::shared_ptr< allocator_t > &allocator,
    unsigned int size
  );
  [[nodiscard]] const std::shared_ptr< gct::render_pass_t > &get_render_pass() const {
    return render_pass;
  }
  [[nodiscard]] const std::shared_ptr< gct::framebuffer_t > &get_framebuffer() const {
    return framebuffer;
  }
  [[nodiscard]] const gct::render_pass_begin_info_t &get_render_pass_begin_info() const {
    return rpbi;
  }
  [[nodiscard]] const vk::Viewport &get_viewport() const {
    return viewport;
  }
  [[nodiscard]] const vk::Rect2D &get_scissor() const {
    return scissor;
  }
private:
  std::shared_ptr< gct::render_pass_t > render_pass;
  std::shared_ptr< gct::framebuffer_t > framebuffer;
  gct::render_pass_begin_info_t rpbi;
  vk::Viewport viewport;
  vk::Rect2D scissor;
};

}

#endif

