#include <nlohmann/json.hpp>
#include <vulkan2json/Format.hpp>
#include <vulkan2json/ImageCreateFlags.hpp>
#include <gct/gbuffer_create_info.hpp>
#include <gct/allocator.hpp>
#include <gct/image_view.hpp>
#include <gct/image.hpp>

namespace gct {

gbuffer_create_info &gbuffer_create_info::set_external_color( const std::vector< std::shared_ptr< image_view_t > > &v ) {
  if( !v.empty() && v[ 0 ] ) {
    set_width( v[ 0 ]->get_factory()->get_props().get_basic().extent.width );
    set_height( v[ 0 ]->get_factory()->get_props().get_basic().extent.height );
    set_layer( v[ 0 ]->get_props().get_basic().subresourceRange.layerCount / color_buffer_count );
    set_swapchain_image_count( v.size() );
    set_format(  v[ 0 ]->get_factory()->get_props().get_basic().format );
    set_flags( v[ 0 ]->get_factory()->get_props().get_basic().flags );
  }
  external_color = v;
  return *this;
}

gbuffer_create_info &gbuffer_create_info::set_external_color( std::vector< std::shared_ptr< image_view_t > > &&v ) {
  if( !v.empty() && v[ 0 ] ) {
    set_width( v[ 0 ]->get_factory()->get_props().get_basic().extent.width );
    set_height( v[ 0 ]->get_factory()->get_props().get_basic().extent.height );
    set_layer( v[ 0 ]->get_props().get_basic().subresourceRange.layerCount / color_buffer_count );
    set_swapchain_image_count( v.size() );
    set_format(  v[ 0 ]->get_factory()->get_props().get_basic().format );
    set_flags( v[ 0 ]->get_factory()->get_props().get_basic().flags );
  }
  external_color = std::move( v );
  return *this;
}

void to_json( nlohmann::json &dest, const gbuffer_create_info &src ) {
  dest = nlohmann::json::object();
  if( src.allocator ) {
    dest[ "allocator" ] = src.allocator->get_props();
  }
  else {
    dest[ "allocator" ] = nullptr;
  }
  dest[ "width" ] = src.width;
  dest[ "height" ] = src.height;
  dest[ "layer" ] = src.layer;
  dest[ "swapchain_image_count" ] = src.swapchain_image_count;
  dest[ "color_buffer_count" ] = src.color_buffer_count;
  dest[ "has_depth" ] = src.has_depth;
  dest[ "format" ] = src.format;
  dest[ "flags" ] = src.flags;
  dest[ "blend_mode" ] = src.blend_mode;
}

}

