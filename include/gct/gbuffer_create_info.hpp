#ifndef GCT_GBUFFER_CREATE_INFO_HPP
#define GCT_GBUFFER_CREATE_INFO_HPP

#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/setter.hpp>
#include <gct/color.hpp>

namespace gct {

class allocator_t;
class image_view_t;
struct gbuffer_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( width )
  LIBGCT_SETTER( height )
  LIBGCT_SETTER( layer )
  LIBGCT_SETTER( swapchain_image_count )
  LIBGCT_SETTER( color_buffer_count )
  LIBGCT_SETTER( flags )
  LIBGCT_SETTER( format )
  LIBGCT_SETTER( final_layout )
  LIBGCT_SETTER( clear_color )
  LIBGCT_SETTER( initial_depth )
  LIBGCT_SETTER( external_depth )
  gbuffer_create_info &disable_depth() {
    has_depth = false;
    return *this;
  }
  gbuffer_create_info &enable_depth() {
    has_depth = false;
    return *this;
  }
  gbuffer_create_info &set_external_color( const std::vector< std::shared_ptr< image_view_t > > &v );
  gbuffer_create_info &set_external_color( std::vector< std::shared_ptr< image_view_t > > &&v );
  std::shared_ptr< allocator_t > allocator;
  unsigned int width = 64u;
  unsigned int height = 64u;
  unsigned int layer = 1u;
  unsigned int swapchain_image_count = 1u;
  unsigned int color_buffer_count = 1u;
  bool has_depth = true;
  vk::ImageCreateFlags flags;
  vk::Format format = vk::Format::eR32G32B32A32Sfloat;
  vk::ImageLayout final_layout = vk::ImageLayout::eGeneral;
  std::array< float, 4u > clear_color = gct::color::web::black;
  vk::ClearDepthStencilValue initial_depth = vk::ClearDepthStencilValue( 1.f, 0 );
  std::vector< std::shared_ptr< image_view_t > > external_color;
  std::vector< std::shared_ptr< image_view_t > > external_depth;
};

void to_json( nlohmann::json &dest, const gbuffer_create_info &src );

}

#endif

