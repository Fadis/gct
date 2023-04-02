#ifndef GCT_CUBEMAP_HPP
#define GCT_CUBEMAP_HPP
#include <memory>
#include <array>
#include <utility>
#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <gct/render_pass_begin_info.hpp>

namespace gct {

std::pair< glm::mat4, std::array< glm::mat4, 6u > >
get_cubemap_matrix(
  const glm::vec3 &light_pos,
  float near,
  float far
);

class allocator_t;
class render_pass_t;

class cubemap_images {
public:
  cubemap_images(
    const std::shared_ptr< allocator_t >&,
    const std::vector< image_create_info_t >&,
    const std::shared_ptr< render_pass_t > &render_pass,
    const std::vector< vk::ClearValue > &clear_color
  );
  const std::shared_ptr< image_t > &get_image( unsigned int i ) const {
    return images[ i % images.size() ];
  }
  const std::shared_ptr< image_view_t > &get_image_views( unsigned int i, unsigned int j ) const {
    return image_views[ i % image_views.size() ][ j % 6u ];
  }
  const std::shared_ptr< image_view_t > &get_cube_image_views( unsigned int i ) const {
    return cube_image_views[ i % image_views.size() ];
  }
  const render_pass_begin_info_t &get_render_pass_begin_info( unsigned int i ) const {
    return render_pass_begin_info[ i % 6u ];
  }
private:
  std::vector< std::shared_ptr< image_t > > images;
  std::vector< std::array< std::shared_ptr< image_view_t >, 6u > > image_views;
  std::vector< std::shared_ptr< image_view_t > > cube_image_views;
  std::array< std::shared_ptr< framebuffer_t >, 6u > framebuffers;
  std::array< gct::render_pass_begin_info_t, 6u > render_pass_begin_info;
};
class cubemap_matrix {
public:
  cubemap_matrix(
    const glm::vec3 &center,
    float near_,
    float far_
  );
  const glm::mat4 &get_projection_matrix() const { return proj; }
  const glm::mat4 &get_view_matrix( unsigned int i ) const { return view[ i % view.size() ]; }
  void move_center( const glm::vec3& );
private:
  float near;
  float far;
  glm::mat4 proj;
  std::array< glm::mat4, 6u > view;
};

}

#endif

