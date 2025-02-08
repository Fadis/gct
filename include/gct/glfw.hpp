#ifndef GCT_GLFW_H
#define GCT_GLFW_H
/*
 * Copyright (C) 2021 Naomasa Matsubayashi
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

#define GLFW_INCLUDE_VULKAN
#include <string>
#include <vector>
#include <unordered_set>
#include <filesystem>
#include <glm/ext/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/instance.hpp>
#include <gct/surface.hpp>

namespace gct {
  struct glfw_window_deleter {
    void operator()( GLFWwindow *p ) const;
  };
  class glfw_window {
  public:
    glfw_window(
      unsigned int width_,
      unsigned height_,
      const std::string &title,
      bool fullscreen
    );
    void set_on_moved( const std::function< void( glfw_window&, int, int ) > &f );
    void set_on_resized( const std::function< void( glfw_window&, int, int ) > &f );
    void set_on_closed( const std::function< void( glfw_window& ) > &f );
    void set_on_refresh( const std::function< void( glfw_window& ) > &f );
    void set_on_focused( const std::function< void( glfw_window&, bool ) > &f );
    void set_on_iconified( const std::function< void( glfw_window&, bool ) > &f );
    void set_on_maximized( const std::function< void( glfw_window&, bool ) > &f );
    void set_on_framebuffer_size_changed( const std::function< void( glfw_window&, int, int ) > &f );
    void set_on_scaled( const std::function< void( glfw_window&, float, float ) > &f );
    void set_on_key( const std::function< void( glfw_window&, int, int, int, int ) > &f );
    void set_on_mouse_button( const std::function< void( glfw_window&, int, int, int ) > &f );
    void set_on_cursor_pos( const std::function< void( glfw_window&, double, double ) > &f );
    void set_on_cursor_entered( const std::function< void( glfw_window&, bool ) > &f );
    void set_on_dropped( const std::function< void( glfw_window&, const std::vector< std::filesystem::path >& ) > &f );
#ifdef VK_KHR_SURFACE_EXTENSION_NAME
    std::shared_ptr< surface_t > get_surface(
      const physical_device_t&
    );
#endif
  private:
    std::shared_ptr< GLFWwindow > handle;
    std::function< void( glfw_window&, int, int ) > on_moved;
    std::function< void( glfw_window&, int, int ) > on_resized;
    std::function< void( glfw_window& ) > on_closed;
    std::function< void( glfw_window& ) > on_refresh;
    std::function< void( glfw_window&, bool ) > on_focused;
    std::function< void( glfw_window&, bool ) > on_iconified;
    std::function< void( glfw_window&, bool ) > on_maximized;
    std::function< void( glfw_window&, int, int ) > on_framebuffer_size_changed;
    std::function< void( glfw_window&, float, float ) > on_scaled;
    std::function< void( glfw_window&, int, int, int, int ) > on_key;
    std::function< void( glfw_window&, int, int, int ) > on_mouse_button;
    std::function< void( glfw_window&, double, double ) > on_cursor_pos;
    std::function< void( glfw_window&, bool ) > on_cursor_entered;
    std::function< void( glfw_window&, const std::vector< std::filesystem::path >& ) > on_dropped;
    unsigned int width;
    unsigned int height;
  };
  class glfw {
  public:
    static glfw &get();
    void set_on_error( const std::function< void( int, const char* ) > &f ) {
      on_error = f;
    }
    glfw_window create_windowed_surface( int width, int height, const char *title );
    glfw_window create_full_screen_surface( int width, int height, const char *title );
    void poll();
  private:
    glfw();
    ~glfw();
    std::function< void( int, const char* ) > on_error;
  };
  class glfw_walk_camera {
  public:
    glfw_walk_camera(
      const glm::vec3 &center_,
      float scale_
    );
    [[nodiscard]] const glm::vec3 &get_camera_pos() const { return camera_pos; }
    [[nodiscard]] const glm::mat4 &get_lookat() const { return lookat; }
    void set_camera_pos( const glm::vec3 &v );
    void to_json( nlohmann::json& ) const;
    void from_json( const nlohmann::json& );
    [[nodiscard]] bool camera_moved() const { return camera_pos_changed; }
    void update(
      const std::unordered_set< int >&
    );
    void reset_flags();
    void touch() { camera_pos_changed = true; }
  private:
    glm::vec3 center;
    float scale;
    glm::vec3 camera_pos;
    float camera_angle_h;
    float camera_angle_v;
    float speed;
    glm::vec3 camera_direction;
    glm::mat4 lookat;
    bool camera_pos_changed = true;
  };
  void to_json( nlohmann::json&, const glfw_walk_camera& );
  void from_json( const nlohmann::json&, glfw_walk_camera& );

  class glfw_walk {
  public:
    glfw_walk(
      const glm::vec3 &center_,
      float scale_
    );
    glfw_walk(
      const glm::vec3 &center_,
      float scale_,
      const std::string &filename_
    ); 
    void operator()( glfw_window&, int key, int scancode, int action, int mods );
    void operator++();
    [[nodiscard]] const glm::vec3 &get_camera_pos() const;
    [[nodiscard]] const glm::vec3 &get_light_pos() const { return light_pos; }
    [[nodiscard]] float get_light_energy() const { return light_energy; }
    [[nodiscard]] const glm::mat4 &get_lookat() const;
    void set_camera_pos( const glm::vec3 &v );
    void set_light_pos( const glm::vec3 &v ) {
      light_pos = v;
    }
    void set_end() {
      end_ = true;
    }
    void set_light_energy( float v ) {
      light_energy = v;
    }
    bool end() const {
      return end_;
    }
    void to_json( nlohmann::json& );
    void from_json( const nlohmann::json& );
    void save( const std::string &filename_ );
    [[nodiscard]] bool light_moved() const { return light_pos_changed; }
    [[nodiscard]] bool camera_moved() const;
    void reset_flags();
    [[nodiscard]] const std::vector< glfw_walk_camera > get_camera() const {
      return camera;
    }
    [[nodiscard]] std::uint32_t get_current_camera() const {
      return current_camera;
    }
    [[nodiscard]] bool get_grid() const {
      return grid;
    }
    [[nodiscard]] bool get_grid_changed() const {
      return grid_changed;
    }
  private:
    glm::vec3 center;
    float scale;
    float speed;
    mutable std::vector< glfw_walk_camera > camera;
    std::uint32_t current_camera = 0u;
    glm::vec3 light_pos;
    float light_energy;
    bool end_;
    std::unordered_set< int > pressed_keys;
    bool light_pos_changed = true;
    bool grid = false;
    bool grid_changed = true;
  };
}

#endif

