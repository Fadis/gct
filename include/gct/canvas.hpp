#ifndef LIBGCT_CANVAS_HPP
#define LIBGCT_CANVAS_HPP
/*
 * Copyright (C) 2022 Naomasa Matsubayashi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <memory>
#include <utility>
#include <thread>
#include <array>
#include <unordered_map>
#include <mutex>
#include <vulkan/vulkan.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <gct/setter.hpp>
#include <gct/render_pass_begin_info.hpp>
#include <gct/font.hpp>

namespace gct {
class device_t;
class allocator_t;
class shader_module_t;
class shader_module_t;
class render_pass_t;
class descriptor_set_layout_t;
class pipeline_layout_t;
class descriptor_set_t;
class image_t;
class image_view_t;
class framebuffer_t;
class graphics_pipeline_t;
class command_buffer_recorder_t;

class text_renderer {
  struct rect_info {
    LIBGCT_SETTER( color )
    LIBGCT_SETTER( offset )
    LIBGCT_SETTER( extent )
    LIBGCT_SETTER( semantic )
    LIBGCT_SETTER( texid )
    LIBGCT_SETTER( depth )
    glm::vec4 color;
    std::array< std::uint16_t, 2u > offset;
    std::array< std::uint16_t, 2u > extent;
    std::array< std::uint16_t, 2u > semantic;
    std::uint16_t texid;
    std::uint16_t depth;
  };
  struct uniform_t {
    struct spec_t {
      std::uint32_t image_count = 0u;
    };
    uniform_t( const vk::Extent2D &size ) {
      world_matrix =
        glm::scale(
          glm::scale(
            glm::translate(
              glm::mat4(
                1.f, 0.f, 0.f, 0.f,
                0.f, -1.f, 0.f, 0.f,
                0.f, 0.f, 1.f, 0.f,
                0.f, 0.f, 0.f, 1.f
              ),
              glm::vec3( -1.f, -1.f, 0.0f )
            ),
            glm::vec3( 2.f, 2.f, 1.f )
          ),
          glm::vec3( 1.f/size.width , 1.f/size.height, 1.f )
        );
    }
    LIBGCT_SETTER( world_matrix )
    LIBGCT_SETTER( rects )
    LIBGCT_SETTER( scale_factor )
    glm::mat4 world_matrix;
    std::array< rect_info, 65536u > rects;
    float scale_factor = 1.f;
  };
public:
  text_renderer(
    const std::shared_ptr< device_t > &device_,
    const std::shared_ptr< allocator_t > &allocator_,
    const std::shared_ptr< pipeline_cache_t > &pipeline_cache,
    const std::shared_ptr< descriptor_pool_t > &descriptor_pool,
    const std::shared_ptr< shader_module_t > &vs_,
    const std::shared_ptr< shader_module_t > &fs_,
    const std::shared_ptr< render_pass_t > &render_pass,
    const vk::Extent2D &size_,
    const font &font_,
    const std::vector< std::shared_ptr< image_view_t > > &input_images_
  );
  void reset();
  void add_glyph( std::uint32_t glyph_id, float x, float y, float size, const std::array< float, 4u > &color, std::uint16_t depth, std::uint16_t texid, std::uint16_t semid );
  void operator()(
    command_buffer_recorder_t &recorder
  );
  void load(
    command_buffer_recorder_t &recorder
  );
  bool full() const;
private:
  void transfer_uniforms( command_buffer_recorder_t &recorder );
  void transfer_indirect( command_buffer_recorder_t &recorder );
  void load_indirect_commands(
    command_buffer_recorder_t &recorder
  );
  void draw(
    command_buffer_recorder_t &recorder
  );
  std::shared_ptr< device_t > device;
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< shader_module_t > vs;
  std::shared_ptr< shader_module_t > fs;
  vk::Extent2D size;
  font font_info;
  std::shared_ptr< descriptor_set_layout_t > descriptor_set_layout;
  std::shared_ptr< pipeline_layout_t > pipeline_layout;
  std::shared_ptr< descriptor_set_t > descriptor_set;
  std::shared_ptr< graphics_pipeline_t > pipeline;
  uniform_t uniforms_host;
  std::shared_ptr< buffer_t > uniforms_staging;
  std::shared_ptr< buffer_t > uniforms_device;
  std::array< VkDrawIndexedIndirectCommand, 65536 > indirect_host;
  std::shared_ptr< buffer_t > indirect_staging;
  std::shared_ptr< buffer_t > indirect_device;
  std::vector< std::shared_ptr< image_view_t > > input_images;
  std::shared_ptr< sampler_t > sampler;
  unsigned int instance_count = 0u;
  unsigned int next_semantic_id = 0u;
  bool vertex_loaded = false;
  bool text_loaded = false;
  std::uint16_t scale = 1u;
};

struct simple_shape {
  std::shared_ptr< buffer_t > vertex_buffer;
  std::uint32_t vertex_count = 0u;
};

class rect_renderer {
  struct rect_info {
    LIBGCT_SETTER( color )
    LIBGCT_SETTER( offset )
    LIBGCT_SETTER( extent )
    LIBGCT_SETTER( semantic )
    LIBGCT_SETTER( texid )
    LIBGCT_SETTER( depth )
    glm::vec4 color;
    std::array< std::uint16_t, 2u > offset;
    std::array< std::uint16_t, 2u > extent;
    std::array< std::uint16_t, 2u > semantic;
    std::uint16_t texid;
    std::uint16_t depth;
  };
  struct uniform_t {
    struct spec_t {
      std::uint32_t image_count = 0u;
    };
    uniform_t( const vk::Extent2D &size ) {
      world_matrix =
        glm::scale(
          glm::scale(
            glm::translate(
              glm::mat4(
                1.f, 0.f, 0.f, 0.f,
                0.f, -1.f, 0.f, 0.f,
                0.f, 0.f, 1.f, 0.f,
                0.f, 0.f, 0.f, 1.f
              ),
              glm::vec3( -1.f, -1.f, 0.0f )
            ),
            glm::vec3( 2.f, 2.f, 1.f )
          ),
          glm::vec3( 1.f/size.width , 1.f/size.height, 1.f )
        );
    }
    LIBGCT_SETTER( world_matrix )
    LIBGCT_SETTER( rects )
    LIBGCT_SETTER( scale_factor )
    glm::mat4 world_matrix;
    std::array< rect_info, 65536u > rects;
    float scale_factor = 1.f;
  };
public:
  rect_renderer(
    const std::shared_ptr< device_t > &device_,
    const std::shared_ptr< allocator_t > &allocator_,
    const std::shared_ptr< pipeline_cache_t > &pipeline_cache,
    const std::shared_ptr< descriptor_pool_t > &descriptor_pool,
    const std::shared_ptr< shader_module_t > &vs_,
    const std::shared_ptr< shader_module_t > &fs_,
    const std::shared_ptr< render_pass_t > &render_pass,
    const vk::Extent2D &size_,
    const simple_shape &shape_,
    const std::vector< std::shared_ptr< image_view_t > > &input_images_
  );
  void reset();
  void add_rectangle( const vk::Rect2D &rect, const std::array< float, 4u > &color, std::uint16_t depth, bool fill, std::uint32_t border_width, std::uint16_t texid, std::uint16_t semid );
  void operator()(
    command_buffer_recorder_t &recorder
  );
  void load(
    command_buffer_recorder_t &recorder
  );
  bool full() const;
private:
  void add_rectangle_internal(
    const vk::Rect2D &rect,
    const std::array< float, 4u > &color, 
    std::uint16_t depth,
    std::uint16_t semantic_id,
    std::uint16_t texid
  );
  void transfer_uniforms( command_buffer_recorder_t &recorder );
  /*void load_vertex_buffer(
    command_buffer_recorder_t &recorder
  );*/
  void draw(
    command_buffer_recorder_t &recorder
  );
  std::shared_ptr< device_t > device;
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< shader_module_t > vs;
  std::shared_ptr< shader_module_t > fs;
  vk::Extent2D size;
  std::shared_ptr< descriptor_set_layout_t > descriptor_set_layout;
  std::shared_ptr< pipeline_layout_t > pipeline_layout;
  std::shared_ptr< descriptor_set_t > descriptor_set;
  std::shared_ptr< graphics_pipeline_t > pipeline;
  uniform_t uniforms_host;
  std::shared_ptr< buffer_t > uniforms_staging;
  std::shared_ptr< buffer_t > uniforms_device;
  //std::vector< std::uint8_t > host_vertex_buffer;
  simple_shape shape;
  std::vector< std::shared_ptr< image_view_t > > input_images;
  std::shared_ptr< sampler_t > sampler;
  unsigned int instance_count = 0u;
  bool vertex_loaded = false;
  bool text_loaded = false;
  std::uint16_t scale = 1u;
};

class canvas {
public:
  canvas(
    const std::shared_ptr< device_t > &device_,
    const std::shared_ptr< allocator_t > &allocator_,
    const std::shared_ptr< pipeline_cache_t > &pipeline_cache,
    const std::shared_ptr< descriptor_pool_t > &descriptor_pool,
    const std::shared_ptr< shader_module_t > &vs_,
    const std::shared_ptr< shader_module_t > &fs_,
    const vk::Extent2D &size_,
    const std::array< float, 4u > &clear_color,
    const std::vector< simple_shape > &shape_,
    const std::vector< font > &font_,
    const std::vector< std::shared_ptr< image_view_t > > &input_images_
  );
  void reset();
  void add_rectangle( const vk::Rect2D &rect, const std::array< float, 4u > &color, std::uint16_t depth, bool fill, std::uint32_t border_width, std::uint16_t texid, std::uint16_t semid );
  void add_glyph( std::uint32_t font_id, std::uint32_t glyph_id, float x, float y, float size, const std::array< float, 4u > &color, std::uint16_t depth, std::uint16_t texid, std::uint16_t semid );
  [[nodiscard]] const std::shared_ptr< image_t > &get_color();
  [[nodiscard]] const std::shared_ptr< image_t > &get_semantic();
  [[nodiscard]] const std::shared_ptr< render_pass_t > &get_render_pass();
  void operator()( command_buffer_recorder_t &recorder );
  [[nodiscard]] bool is_modified() {
    return modified;
  }
  void set_modified() {
    modified = true;
  }
private:
  void draw(
    command_buffer_recorder_t &recorder,
    const render_pass_begin_info_t &render_pass_begin_info_
  );
  std::shared_ptr< device_t > device;
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< shader_module_t > vs;
  std::shared_ptr< shader_module_t > fs;
  vk::Extent2D size;
  std::vector< simple_shape > shape;
  std::vector< font > font_info;
  std::vector< std::shared_ptr< image_view_t > > input_images;
  std::shared_ptr< image_t > color;
  std::shared_ptr< image_t > semantic;
  std::shared_ptr< image_t > depth;
  std::shared_ptr< render_pass_t > render_pass;
  std::shared_ptr< image_view_t > color_view;
  std::shared_ptr< image_view_t > semantic_view;
  std::shared_ptr< image_view_t > depth_view;
  std::shared_ptr< framebuffer_t > framebuffer;
  render_pass_begin_info_t render_pass_begin_info;
  std::vector< std::vector< rect_renderer > > rects;
  std::unordered_map< std::uint32_t, std::vector< text_renderer > > texts;
  std::mutex guard;
  bool modified = false;
};

}

#endif

