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
#include <vector>
#include <filesystem>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
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

}

#endif

