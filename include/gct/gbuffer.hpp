#ifndef GCT_GBUFFER_HPP
#define GCT_GBUFFER_HPP

#include <memory>
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/render_pass_begin_info.hpp>
#include <gct/gbuffer_create_info.hpp>
#include <gct/property.hpp>

namespace gct {

class allocator_t;
class render_pass_t;
class image_view_t;
class image_t;
class command_buffer_recorder_t;
class gbuffer :
  public property< gbuffer_create_info >,
  public created_from< allocator_t >,
  public std::enable_shared_from_this< gbuffer > {
public:
  gbuffer(
    const gbuffer_create_info &ci
  );
  gbuffer(
    const std::shared_ptr< allocator_t > &allocator,
    unsigned int width,
    unsigned int height,
    unsigned int swapchain_image_count,
    unsigned int color_buffer_count
  );
  [[nodiscard]] const std::shared_ptr< render_pass_t > &get_render_pass() const {
    return render_pass;
  }
  [[nodiscard]] const render_pass_begin_info_t &get_render_pass_begin_info( unsigned int i ) const {
    return rpbis[ i % rpbis.size() ];
  }
  [[nodiscard]] const std::vector< render_pass_begin_info_t > &get_render_pass_begin_infos() const {
    return rpbis;
  }
  [[nodiscard]] const std::shared_ptr< image_view_t > &get_image_view( unsigned int i ) const {
    return image_views[ i % image_views.size() ];
  }
  [[nodiscard]] const std::vector< std::shared_ptr< image_view_t > > &get_image_views() const {
    return image_views;
  }
  [[nodiscard]] const std::vector< std::shared_ptr< image_view_t > > &get_depth_views() const {
    return depth_views;
  }
  [[nodiscard]] const std::shared_ptr< image_t > &get_image( unsigned int i ) const;
  [[nodiscard]] const vk::Viewport &get_viewport() const {
    return viewport;
  }
  [[nodiscard]] const vk::Rect2D &get_scissor() const {
    return scissor;
  }
  void clear_color(
    command_buffer_recorder_t &rec,
    unsigned int image_index,
    const std::vector< vk::ImageSubresourceRange > &range
  );
  void clear_color(
    command_buffer_recorder_t &rec,
    unsigned int image_index
  );
private:
  std::shared_ptr< render_pass_t > render_pass;
  std::vector< render_pass_begin_info_t > rpbis;
  std::vector< std::shared_ptr< image_view_t > > image_views;
  std::vector< std::shared_ptr< image_view_t > > depth_views;
  vk::Viewport viewport;
  vk::Rect2D scissor;
};

void to_json( nlohmann::json &dest, const gbuffer &src );

}

#endif

