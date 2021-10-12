#include <gct/to_json.hpp>

namespace gct {
  nlohmann::json to_json( const vk::Extent3D &v ) {
    auto root = nlohmann::json::object();
    root[ "width" ] = v.width;
    root[ "height" ] = v.height;
    root[ "depth" ] = v.depth;
    return root;
  }

  nlohmann::json to_json( const vk::Extent2D &v ) {
    auto root = nlohmann::json::object();
    root[ "width" ] = v.width;
    root[ "height" ] = v.height;
    return root;
  }
  nlohmann::json to_json( const vk::Offset2D &v ) {
    auto root = nlohmann::json::object();
    root[ "x" ] = v.x;
    root[ "y" ] = v.y;
    return root;
  }

#ifdef VK_KHR_SURFACE_EXTENSION_NAME
  nlohmann::json to_json( const vk::SurfaceFormatKHR &v ) {
    auto root = nlohmann::json::object();
    root[ "format" ] = std::uint32_t( v.format );
    root[ "colorSpace" ] = std::uint32_t( v.colorSpace );
    return root;
  }
  nlohmann::json to_json( const std::vector< vk::SurfaceFormatKHR > &v ) {
    auto root = nlohmann::json::array();
    for( const auto &e: v )
      root.push_back( to_json( e ) );
    return root;
  }
#endif

}

