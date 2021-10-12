#include <fstream>
#include <iterator>
#include <gct/descriptor_image_info.hpp>
#include <gct/image_view.hpp>
#include <gct/sampler.hpp>

namespace gct {
  descriptor_image_info_t &descriptor_image_info_t::rebuild_chain() {
    if( chained ) return *this;
    if( sampler )
      basic.setSampler( **sampler );
    else
      basic.setSampler( nullptr );
    if( image_view )
      basic.setImageView( **image_view );
    else
      basic.setImageView( nullptr );
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
}


