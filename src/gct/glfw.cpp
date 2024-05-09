#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <gct/glfw.hpp>
#include <gct/vulkanhpp.hpp>
#include <gct/io_context.hpp>
#include <gct/exception.hpp>
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
    if( err ) {
#if VK_HEADER_VERSION >= 256
      vk::detail::throwResultException( vk::Result( err ), "glfwCreateWindowSurface failed" );
#else
      vk::throwResultException( vk::Result( err ), "glfwCreateWindowSurface failed" );
#endif
    }
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

  glfw_walk_camera::glfw_walk_camera(
    const glm::vec3 &center_,
    float scale_
  ) :
    center( center_ ),
    scale( scale_ ) {
    camera_pos = center + glm::vec3{ 0.f, 0.f, 1.0f*scale };
    camera_angle_h = 0;
    camera_angle_v = 0;
    speed = 0.01f*scale;
    camera_direction = glm::vec3{ std::sin( camera_angle_h ) * std::cos( camera_angle_v ), std::sin( camera_angle_v ), -std::cos( camera_angle_h ) * std::cos( camera_angle_v ) };
    lookat = glm::lookAt(
      camera_pos,
      camera_pos + camera_direction,
      glm::vec3{ 0.f, camera_pos[ 1 ] + 100.f*scale, 0.f }
    );
    camera_pos_changed = true;
  }
  void glfw_walk_camera::set_camera_pos( const glm::vec3 &v ) {
    camera_pos = v;
    lookat = glm::lookAt(
      camera_pos,
      camera_pos + camera_direction,
      glm::vec3{ 0.f, camera_pos[ 1 ] + 100.f*scale, 0.f }
    );
    camera_pos_changed = true;
  }
  void glfw_walk_camera::update(
    const std::unordered_set< int > &pressed_keys
  ) {
    if( pressed_keys.find( GLFW_KEY_A ) != pressed_keys.end() ) {
      camera_angle_h -= 0.02 * M_PI/2;
      camera_pos_changed = true;
    }
    if( pressed_keys.find( GLFW_KEY_D ) != pressed_keys.end() ) {
      camera_angle_h += 0.02 * M_PI/2;
      camera_pos_changed = true;
    }
    if( pressed_keys.find( GLFW_KEY_Q ) != pressed_keys.end() ) {
      camera_angle_v = std::max( camera_angle_v - 0.02 * M_PI/2, -M_PI/2 * 0.95 );
      camera_pos_changed = true;
    }
    if( pressed_keys.find( GLFW_KEY_Z ) != pressed_keys.end() ) {
      camera_angle_v = std::min( camera_angle_v + 0.02 * M_PI/2, M_PI/2 * 0.95 );
      camera_pos_changed = true;
    }
    if( pressed_keys.find( GLFW_KEY_X ) != pressed_keys.end() ) {
      camera_angle_v = 0.0;
      camera_pos_changed = true;
    }
    camera_direction = glm::vec3{ std::sin( camera_angle_h ) * std::cos( camera_angle_v ), std::sin( camera_angle_v ), -std::cos( camera_angle_h ) * std::cos( camera_angle_v ) };
    if( pressed_keys.find( GLFW_KEY_W ) != pressed_keys.end() ) {
      camera_pos += camera_direction * glm::vec3( speed );
      camera_pos_changed = true;
    }
    if( pressed_keys.find( GLFW_KEY_S ) != pressed_keys.end() ) {
      camera_pos -= camera_direction * glm::vec3( speed );
      camera_pos_changed = true;
    }
    if( pressed_keys.find( GLFW_KEY_E ) != pressed_keys.end() ) {
      camera_pos[ 1 ] -= speed;
      camera_pos_changed = true;
    }
    if( pressed_keys.find( GLFW_KEY_C ) != pressed_keys.end() ) {
      camera_pos[ 1 ] += speed;
      camera_pos_changed = true;
    }
    lookat = glm::lookAt(
      camera_pos,
      camera_pos + camera_direction,
      glm::vec3{ 0.f, camera_pos[ 1 ] + 100.f*scale, 0.f }
    );
  }
  void glfw_walk_camera::reset_flags() {
    camera_pos_changed = false;
  }
  void glfw_walk_camera::to_json( nlohmann::json &dest ) const {
    dest = nlohmann::json::object();
    dest[ "center" ] = nlohmann::json::array();
    dest[ "center" ].push_back( center[ 0 ] );
    dest[ "center" ].push_back( center[ 1 ] );
    dest[ "center" ].push_back( center[ 2 ] );
    dest[ "scale" ] = scale;
    dest[ "speed" ] = speed;
    dest[ "camera_pos" ] = nlohmann::json::array();
    dest[ "camera_pos" ].push_back( camera_pos[ 0 ] );
    dest[ "camera_pos" ].push_back( camera_pos[ 1 ] );
    dest[ "camera_pos" ].push_back( camera_pos[ 2 ] );
    dest[ "camera_angle_h" ] = camera_angle_h;
    dest[ "camera_angle_v" ] = camera_angle_v;
  }
  void glfw_walk_camera::from_json( const nlohmann::json &src ) {
    if( !src.is_object() )
      throw gct::exception::runtime_error( "incompatible walk data", __FILE__, __LINE__ );
    if( src.find( "center" ) == src.end() )
      throw gct::exception::runtime_error( "incompatible walk data", __FILE__, __LINE__ );
    if( !src[ "center" ].is_array() )
      throw gct::exception::runtime_error( "incompatible walk data", __FILE__, __LINE__ );
    if( src[ "center" ].size() != 3 )
      throw gct::exception::runtime_error( "incompatible walk data", __FILE__, __LINE__ );
    if( src.find( "camera_pos" ) == src.end() )
      throw gct::exception::runtime_error( "incompatible walk data", __FILE__, __LINE__ );
    if( !src[ "camera_pos" ].is_array() )
      throw gct::exception::runtime_error( "incompatible walk data", __FILE__, __LINE__ );
    if( src[ "camera_pos" ].size() != 3 )
      throw gct::exception::runtime_error( "incompatible walk data", __FILE__, __LINE__ );
    if( src.find( "camera_angle_h" ) == src.end() )
      throw gct::exception::runtime_error( "incompatible walk data", __FILE__, __LINE__ );
    if( src.find( "camera_angle_v" ) == src.end() )
      throw gct::exception::runtime_error( "incompatible walk data", __FILE__, __LINE__ );
    center = glm::vec3( src[ "center" ][ 0 ], src[ "center" ][ 1 ], src[ "center" ][ 2 ] );
    scale = src[ "scale" ];
    speed = src[ "speed" ];
    camera_pos = glm::vec3( src[ "camera_pos" ][ 0 ], src[ "camera_pos" ][ 1 ], src[ "camera_pos" ][ 2 ] );
    camera_angle_h = src[ "camera_angle_h" ];
    camera_angle_v = src[ "camera_angle_v" ];
    camera_direction = glm::vec3{ std::sin( camera_angle_h ) * std::cos( camera_angle_v ), std::sin( camera_angle_v ), -std::cos( camera_angle_h ) * std::cos( camera_angle_v ) };
    lookat = glm::lookAt(
      camera_pos,
      camera_pos + camera_direction,
      glm::vec3{ 0.f, camera_pos[ 1 ] + 100.f*scale, 0.f }
    );
    camera_pos_changed = true;
  }
  void to_json( nlohmann::json &dest, const glfw_walk_camera &src ) {
    src.to_json( dest );
  }
  void from_json( const nlohmann::json &src, glfw_walk_camera &dest ) {
    dest.from_json( src );
  }

  glfw_walk::glfw_walk(
    const glm::vec3 &center_,
    float scale_
  ) : 
    center( center_ ),
    scale( scale_ ),
    end_( false ) {
    speed = 0.01f*scale;
    light_pos = glm::vec3{ 0.0f*scale, -0.5f*scale, 0.0f*scale };
    light_energy = 5.0f;
    if( current_camera >= camera.size() ) {
      camera.resize( current_camera + 1u, glfw_walk_camera( center, scale ) );
    }
  }
  glfw_walk::glfw_walk(
    const glm::vec3 &center_,
    float scale_,
    const std::string &filename
  ) : 
    center( center_ ),
    scale( scale_ ),
    end_( false ) {
    speed = 0.01f*scale;
    light_pos = glm::vec3{ 0.0f*scale, -0.5f*scale, 0.0f*scale };
    light_energy = 5.0f;
    if( std::filesystem::is_regular_file( filename ) ) {
      try {
        std::ifstream fd( filename );
        from_json( nlohmann::json::parse( fd ) );
      }
      catch(...) {}
    }
    if( current_camera >= camera.size() ) {
      camera.resize( current_camera + 1u, glfw_walk_camera( center, scale ) );
    }
  }
  void glfw_walk::operator()( glfw_window&, int key, int scancode, int action, int mods ) {
    if( action == GLFW_RELEASE )
      pressed_keys.erase( key );
    else if( action == GLFW_PRESS )
      pressed_keys.insert( key );
  }
  void glfw_walk::reset_flags() {
    for( auto &c: camera ) {
      c.reset_flags();
    }
    light_pos_changed = false;
  }
  const glm::vec3 &glfw_walk::get_camera_pos() const {
    if( current_camera >= camera.size() ) {
      camera.resize( current_camera + 1u, glfw_walk_camera( center, scale ) );
    }
    return camera[ current_camera ].get_camera_pos();
  }
  const glm::mat4 &glfw_walk::get_lookat() const {
    if( current_camera >= camera.size() ) {
      camera.resize( current_camera + 1u, glfw_walk_camera( center, scale ) );
    }
    return camera[ current_camera ].get_lookat();
  }
  void glfw_walk::operator++() {
    if( current_camera >= camera.size() ) {
      camera.resize( current_camera + 1u, glfw_walk_camera( center, scale ) );
    }
    camera[ current_camera ].update( pressed_keys );
    bool camera_changed = false;
    bool old_grid = grid;
    if( pressed_keys.find( GLFW_KEY_1 ) != pressed_keys.end() ) {
      current_camera = 0;
      camera_changed = true;
    }
    if( pressed_keys.find( GLFW_KEY_2 ) != pressed_keys.end() ) {
      current_camera = 1;
      camera_changed = true;
    }
    if( pressed_keys.find( GLFW_KEY_3 ) != pressed_keys.end() ) {
      current_camera = 2;
      camera_changed = true;
    }
    if( pressed_keys.find( GLFW_KEY_4 ) != pressed_keys.end() ) {
      current_camera = 3;
      camera_changed = true;
    }
    if( pressed_keys.find( GLFW_KEY_J ) != pressed_keys.end() ) {
      light_pos[ 0 ] -= speed;
      light_pos_changed = true;
    }
    if( pressed_keys.find( GLFW_KEY_K ) != pressed_keys.end() ) {
      light_pos[ 2 ] -= speed;
      light_pos_changed = true;
    }
    if( pressed_keys.find( GLFW_KEY_L ) != pressed_keys.end() ) {
      light_pos[ 0 ] += speed;
      light_pos_changed = true;
    }
    if( pressed_keys.find( GLFW_KEY_I ) != pressed_keys.end() ) {
      light_pos[ 2 ] += speed;
      light_pos_changed = true;
    }
    if( pressed_keys.find( GLFW_KEY_U ) != pressed_keys.end() ) {
      light_pos[ 1 ] -= speed;
      light_pos_changed = true;
    }
    if( pressed_keys.find( GLFW_KEY_M ) != pressed_keys.end() ) {
      light_pos[ 1 ] += speed;
      light_pos_changed = true;
    }
    if( pressed_keys.find( GLFW_KEY_UP ) != pressed_keys.end() ) {
      light_energy += 0.05f;
      light_pos_changed = true;
    }
    if( pressed_keys.find( GLFW_KEY_DOWN ) != pressed_keys.end() ) {
      light_energy -= 0.05f;
      light_pos_changed = true;
    }
    if( pressed_keys.find( GLFW_KEY_G ) != pressed_keys.end() ) {
      grid = true;
    }
    else {
      grid = false;
    }
    if( pressed_keys.find( GLFW_KEY_ESCAPE ) != pressed_keys.end() )
      end_ = true;
    if( current_camera >= camera.size() ) {
      camera.resize( current_camera + 1u, glfw_walk_camera( center, scale ) );
    }
    if( camera_changed ) {
      camera[ current_camera ].touch();
    }
    grid_changed = grid != old_grid;
  }
  void glfw_walk::set_camera_pos( const glm::vec3 &v ) {
    if( current_camera >= camera.size() ) {
      camera.resize( current_camera + 1u, glfw_walk_camera( center, scale ) );
    }
    camera[ current_camera ].set_camera_pos( v );
  }
  bool glfw_walk::camera_moved() const {
    if( current_camera >= camera.size() ) {
      camera.resize( current_camera + 1u, glfw_walk_camera( center, scale ) );
    }
    return camera[ current_camera ].camera_moved();
  }
  void glfw_walk::to_json( nlohmann::json &dest ) {
    dest = nlohmann::json::object();
    dest[ "camera" ] = camera;
    dest[ "center" ] = nlohmann::json::array();
    dest[ "center" ].push_back( center[ 0 ] );
    dest[ "center" ].push_back( center[ 1 ] );
    dest[ "center" ].push_back( center[ 2 ] );
    dest[ "scale" ] = scale;
    dest[ "speed" ] = speed;
    dest[ "light_pos" ] = nlohmann::json::array();
    dest[ "light_pos" ].push_back( light_pos[ 0 ] );
    dest[ "light_pos" ].push_back( light_pos[ 1 ] );
    dest[ "light_pos" ].push_back( light_pos[ 2 ] );
    dest[ "light_energy" ] = light_energy;
    dest[ "grid" ] = grid;
  }
  void glfw_walk::from_json( const nlohmann::json &src ) {
    if( !src.is_object() )
      throw gct::exception::runtime_error( "incompatible walk data", __FILE__, __LINE__ );
    if( src.find( "speed" ) == src.end() )
      throw gct::exception::runtime_error( "incompatible walk data", __FILE__, __LINE__ );
    if( src.find( "center" ) == src.end() )
      throw gct::exception::runtime_error( "incompatible walk data", __FILE__, __LINE__ );
    if( !src[ "center" ].is_array() )
      throw gct::exception::runtime_error( "incompatible walk data", __FILE__, __LINE__ );
    if( src[ "center" ].size() != 3 )
      throw gct::exception::runtime_error( "incompatible walk data", __FILE__, __LINE__ );
    if( src.find( "scale" ) == src.end() )
      throw gct::exception::runtime_error( "incompatible walk data", __FILE__, __LINE__ );
    if( src.find( "camera" ) == src.end() )
      throw gct::exception::runtime_error( "incompatible walk data", __FILE__, __LINE__ );
    if( src.find( "light_pos" ) == src.end() )
      throw gct::exception::runtime_error( "incompatible walk data", __FILE__, __LINE__ );
    if( !src[ "light_pos" ].is_array() )
      throw gct::exception::runtime_error( "incompatible walk data", __FILE__, __LINE__ );
    if( src[ "light_pos" ].size() != 3 )
      throw gct::exception::runtime_error( "incompatible walk data", __FILE__, __LINE__ );
    if( src.find( "light_energy" ) == src.end() )
      throw gct::exception::runtime_error( "incompatible walk data", __FILE__, __LINE__ );
    if( src.find( "grid" ) == src.end() )
      throw gct::exception::runtime_error( "incompatible walk data", __FILE__, __LINE__ );
    camera.clear();
    for( const auto &c: src[ "camera" ] ) {
      glfw_walk_camera temp( center, scale );
      temp.from_json( c );
      camera.push_back( temp );
    }
    center = glm::vec3( src[ "center" ][ 0 ], src[ "center" ][ 1 ], src[ "center" ][ 2 ] );
    scale = src[ "scale" ];
    speed = src[ "speed" ];
    light_pos = glm::vec3( src[ "light_pos" ][ 0 ], src[ "light_pos" ][ 1 ], src[ "light_pos" ][ 2 ] );
    light_energy = src[ "light_energy" ];
    grid = src[ "grid" ];
    light_pos_changed = true;
  }
  void glfw_walk::save( const std::string &filename ) {
    nlohmann::json root;
    to_json( root );
    if( std::filesystem::is_regular_file( filename ) || !std::filesystem::exists( filename ) ) {
      std::ofstream fd( filename );
      fd << root.dump( 4 ) << std::endl;
    }
  }
}
