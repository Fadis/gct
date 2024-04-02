#include <fstream>
#include <iterator>
#include <nlohmann/json.hpp>
#include <vulkan2json/DescriptorImageInfo.hpp>
#include <gct/descriptor_image_info.hpp>
#include <gct/image_view.hpp>
#include <gct/sampler.hpp>

namespace gct {
  descriptor_image_info_t &descriptor_image_info_t::rebuild_chain() {
    if( chained ) return *this;
    if( sampler )
      basic.setSampler( **sampler );
    if( image_view )
      basic.setImageView( **image_view );
    chained = true;
    return *this;
  }
  descriptor_image_info_t &descriptor_image_info_t::set_sampler( const std::shared_ptr< sampler_t > &v ) {
    sampler = v;
    chained = false;
    return *this;
  }
  descriptor_image_info_t &descriptor_image_info_t::clear_sampler() {
    sampler.reset();
    chained = false;
    return *this;
  }
  descriptor_image_info_t &descriptor_image_info_t::set_image_view( const std::shared_ptr< image_view_t > &v ) {
    image_view = v;
    chained = false;
    return *this;
  }
  descriptor_image_info_t &descriptor_image_info_t::clear_image_view() {
    image_view.reset();
    chained = false;
    return *this;
  }

void to_json( nlohmann::json &root, const descriptor_image_info_t &v ) {
  root = nlohmann::json::object();
  root[ "basic" ] = v.get_basic();
  if( v.get_image_view() ) {
    root[ "basic" ][ "imageView" ] = *v.get_image_view();
  }
  if( v.get_sampler() ) {
    root[ "basic" ][ "sampler" ] = *v.get_sampler();
  }
}

}


