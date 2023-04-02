#include <iostream>
#include <gct/glfw.hpp>
#include <gct/vulkanhpp.hpp>
#include <gct/io_context.hpp>
namespace gct {
  void glfw_window_deleter::operator()( GLFWwindow *p ) const {
    if( p ) glfwDestroyWindow( p );
  }
  glfw_window::glfw_window(
    unsigned int width_,
    unsigned height_,
    const std::string &title,
    bool fullscreen
  ) : width( width_ ), height( height_ ) {
    gct::glfw::get();
    glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
    glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );
    auto raw_window = glfwCreateWindow( width, height, title.c_str(), fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr );
    if( !raw_window )
      throw -1;
    int width_s;
    int height_s;
    glfwGetWindowSize( raw_window, &width_s, &height_s );
    width = width_s;
    height = height_s;
    glfwSetWindowUserPointer( raw_window, reinterpret_cast< void* >( this ) );
    handle.reset( raw_window, gct::glfw_window_deleter() );
  }
  void glfw_window::set_on_moved( const std::function< void( glfw_window&, int, int ) > &f ) {
    on_moved = f;
    glfwSetWindowPosCallback(
      handle.get(),
      []( GLFWwindow *raw_window, int xpos, int ypos ) {
        auto window = reinterpret_cast< glfw_window* >( glfwGetWindowUserPointer( raw_window ) );
        window->on_moved( *window, xpos, ypos );
      }
    );
  }
  void glfw_window::set_on_resized( const std::function< void( glfw_window&, int, int ) > &f ) {
    on_resized = f;
    glfwSetWindowSizeCallback(
      handle.get(),
      []( GLFWwindow *raw_window, int width, int height ) {
        auto window = reinterpret_cast< glfw_window* >( glfwGetWindowUserPointer( raw_window ) );
        window->on_resized( *window, width, height );
      }
    );
  }
  void glfw_window::set_on_closed( const std::function< void( glfw_window& ) > &f ) {
    on_closed = f;
    glfwSetWindowCloseCallback(
      handle.get(),
      []( GLFWwindow *raw_window ) {
        auto window = reinterpret_cast< glfw_window* >( glfwGetWindowUserPointer( raw_window ) );
        window->on_closed( *window );
      }
    );
  }
  void glfw_window::set_on_refresh( const std::function< void( glfw_window& ) > &f ) {
    on_refresh = f;
    glfwSetWindowRefreshCallback(
      handle.get(),
      []( GLFWwindow *raw_window ) {
        auto window = reinterpret_cast< glfw_window* >( glfwGetWindowUserPointer( raw_window ) );
        window->on_refresh( *window );
      }
    );
  }
  void glfw_window::set_on_focused( const std::function< void( glfw_window&, bool ) > &f ) {
    on_focused = f;
    glfwSetWindowFocusCallback(
      handle.get(),
      []( GLFWwindow *raw_window, int focused ) {
        auto window = reinterpret_cast< glfw_window* >( glfwGetWindowUserPointer( raw_window ) );
        window->on_focused( *window, focused );
      }
    );
  }
  void glfw_window::set_on_iconified( const std::function< void( glfw_window&, bool ) > &f ) {
    on_iconified = f;
    glfwSetWindowIconifyCallback(
      handle.get(),
      []( GLFWwindow *raw_window, int iconified ) {
        auto window = reinterpret_cast< glfw_window* >( glfwGetWindowUserPointer( raw_window ) );
        window->on_iconified( *window, iconified );
      }
    );
  }
  void glfw_window::set_on_maximized( const std::function< void( glfw_window&, bool ) > &f ) {
    on_maximized = f;
    glfwSetWindowMaximizeCallback(
      handle.get(),
      []( GLFWwindow *raw_window, int maximized ) {
        auto window = reinterpret_cast< glfw_window* >( glfwGetWindowUserPointer( raw_window ) );
        window->on_maximized( *window, maximized );
      }
    );
  }
  void glfw_window::set_on_framebuffer_size_changed( const std::function< void( glfw_window&, int, int ) > &f ) {
    on_framebuffer_size_changed = f;
    glfwSetFramebufferSizeCallback(
      handle.get(),
      []( GLFWwindow *raw_window, int width, int height ) {
        auto window = reinterpret_cast< glfw_window* >( glfwGetWindowUserPointer( raw_window ) );
        window->width = width;
        window->height = height;
        window->on_framebuffer_size_changed( *window, width, height );
      }
    );
  }
  void glfw_window::set_on_scaled( const std::function< void( glfw_window&, float, float ) > &f ) {
    on_scaled = f;
    glfwSetWindowContentScaleCallback(
      handle.get(),
      []( GLFWwindow *raw_window, float xscale, float yscale ) {
        auto window = reinterpret_cast< glfw_window* >( glfwGetWindowUserPointer( raw_window ) );
        window->on_scaled( *window, xscale, yscale );
      }
    );
  }
  void glfw_window::set_on_key( const std::function< void( glfw_window&, int, int, int, int ) > &f ) {
    on_key = f;
    glfwSetKeyCallback(
      handle.get(),
      []( GLFWwindow *raw_window, int key, int scancode, int action, int mods ) {
        auto window = reinterpret_cast< glfw_window* >( glfwGetWindowUserPointer( raw_window ) );
        window->on_key( *window, key, scancode, action, mods );
      }
    );
  }
  void glfw_window::set_on_mouse_button( const std::function< void( glfw_window&, int, int, int ) > &f ) {
    on_mouse_button = f;
    glfwSetMouseButtonCallback(
      handle.get(),
      []( GLFWwindow *raw_window, int button, int action, int mods ) {
        auto window = reinterpret_cast< glfw_window* >( glfwGetWindowUserPointer( raw_window ) );
        window->on_mouse_button( *window, button, action, mods );
      }
    );
  }
  void glfw_window::set_on_cursor_pos( const std::function< void( glfw_window&, double, double ) > &f ) {
    on_cursor_pos = f;
    glfwSetCursorPosCallback(
      handle.get(),
      []( GLFWwindow *raw_window, double xpos, double ypos ) {
        auto window = reinterpret_cast< glfw_window* >( glfwGetWindowUserPointer( raw_window ) );
        window->on_cursor_pos( *window, xpos, ypos );
      }
    );
  }
  void glfw_window::set_on_cursor_entered( const std::function< void( glfw_window&, bool ) > &f ) {
    on_cursor_entered = f;
    glfwSetCursorEnterCallback(
      handle.get(),
      []( GLFWwindow *raw_window, int entered ) {
        auto window = reinterpret_cast< glfw_window* >( glfwGetWindowUserPointer( raw_window ) );
        window->on_cursor_entered( *window, entered );
      }
    );
  }
  void glfw_window::set_on_dropped( const std::function< void( glfw_window&, const std::vector< std::filesystem::path >& ) > &f ) {
    on_dropped = f;
    glfwSetDropCallback(
      handle.get(),
      []( GLFWwindow *raw_window, int path_count, const char *raw_paths[] ) {
        auto window = reinterpret_cast< glfw_window* >( glfwGetWindowUserPointer( raw_window ) );
        std::vector< std::filesystem::path > paths;
        std::copy( raw_paths, std::next( raw_paths, path_count ), std::back_inserter( paths ) );
        window->on_dropped( *window, paths );
      }
    );
  }

#ifdef VK_KHR_SURFACE_EXTENSION_NAME
  std::shared_ptr< surface_t > glfw_window::get_surface(
    const physical_device_t &pdev
  ) {
    VkSurfaceKHR raw_surface;
    VkResult err = glfwCreateWindowSurface( VkInstance( **pdev.get_factory() ), handle.get(), nullptr, &raw_surface );
    if( err )
      vk::throwResultException( vk::Result( err ), "glfwCreateWindowSurface failed" );
    return std::shared_ptr< surface_t >(
      new surface_t(
        pdev,
        vk::SurfaceKHR( raw_surface )
      ),
      [handle=handle] ( surface_t *p ) {
        if( p ) delete p;
      }
    );
  }
#endif
  glfw &glfw::get() {
    static glfw instance;
    return instance;
  }

  glfw::glfw() {
    vulkanhpp::init();
    thread_pool::init();
    if( !glfwInit() ) {
      const char *p;
      glfwGetError( &p );
      std::cerr << p << std::endl;
      throw -1;
    }
    on_error = []( int, const char* ) {};
    glfwSetErrorCallback( []( int c, const char *d ){
      glfw::get().on_error( c, d );
    } );
  }
  void glfw::poll() {
    glfwPollEvents();
  }
  glfw::~glfw() {
    glfwTerminate();
  }
  glfw_walk::glfw_walk(
    const glm::vec3 &center_,
    float scale_
  ) : 
    center( center_ ),
    scale( scale_ ),
    end_( false ) {
    camera_pos = center + glm::vec3{ 0.f, 0.f, 1.0f*scale };
    camera_angle = 0;
    speed = 0.01f*scale;
    light_pos = glm::vec3{ 0.0f*scale, -0.5f*scale, 0.0f*scale };
    light_energy = 5.0f;
    camera_direction = glm::vec3{ std::sin( camera_angle ), 0, -std::cos( camera_angle ) };
  }
  void glfw_walk::operator()( glfw_window&, int key, int scancode, int action, int mods ) {
    if( action == GLFW_RELEASE )
      pressed_keys.erase( key );
    else if( action == GLFW_PRESS )
      pressed_keys.insert( key );
  }
  void glfw_walk::operator++() {
    if( pressed_keys.find( GLFW_KEY_A ) != pressed_keys.end() )
      camera_angle -= 0.01 * M_PI/2;
    if( pressed_keys.find( GLFW_KEY_D ) != pressed_keys.end() )
      camera_angle += 0.01 * M_PI/2;
    camera_direction = glm::vec3{ std::sin( camera_angle ), 0, -std::cos( camera_angle ) };
    if( pressed_keys.find( GLFW_KEY_W ) != pressed_keys.end() )
      camera_pos += camera_direction * glm::vec3( speed );
    if( pressed_keys.find( GLFW_KEY_S ) != pressed_keys.end() )
      camera_pos -= camera_direction * glm::vec3( speed );
    if( pressed_keys.find( GLFW_KEY_E ) != pressed_keys.end() )
      camera_pos[ 1 ] -= speed;
    if( pressed_keys.find( GLFW_KEY_C ) != pressed_keys.end() )
      camera_pos[ 1 ] += speed;
    if( pressed_keys.find( GLFW_KEY_J ) != pressed_keys.end() )
      light_pos[ 0 ] -= speed;
    if( pressed_keys.find( GLFW_KEY_K ) != pressed_keys.end() )
      light_pos[ 2 ] -= speed;
    if( pressed_keys.find( GLFW_KEY_L ) != pressed_keys.end() )
      light_pos[ 0 ] += speed;
    if( pressed_keys.find( GLFW_KEY_I ) != pressed_keys.end() )
      light_pos[ 2 ] += speed;
    if( pressed_keys.find( GLFW_KEY_U ) != pressed_keys.end() )
      light_pos[ 1 ] -= speed;
    if( pressed_keys.find( GLFW_KEY_M ) != pressed_keys.end() )
      light_pos[ 1 ] += speed;
    if( pressed_keys.find( GLFW_KEY_UP ) != pressed_keys.end() )
      light_energy += 0.05f;
    if( pressed_keys.find( GLFW_KEY_DOWN ) != pressed_keys.end() )
      light_energy -= 0.05f;
    if( pressed_keys.find( GLFW_KEY_Q ) != pressed_keys.end() )
      end_ = true;
    lookat = glm::lookAt(
      camera_pos,
      camera_pos + camera_direction,
      glm::vec3{ 0.f, camera_pos[ 1 ] + 100.f*scale, 0.f }
    );
  }
  void glfw_walk::set_camera_pos( const glm::vec3 &v ) {
    camera_pos = v;
    lookat = glm::lookAt(
      camera_pos,
      camera_pos + camera_direction,
      glm::vec3{ 0.f, camera_pos[ 1 ] + 100.f*scale, 0.f }
    );
  }
}
