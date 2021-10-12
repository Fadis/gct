#include <fstream>
#include <iterator>
#include <gct/buffer.hpp>
#include <gct/descriptor_buffer_info.hpp>

namespace gct {
  descriptor_buffer_info_t &descriptor_buffer_info_t::rebuild_chain() {
    if( chained ) return *this;
    if( buffer )
      basic.setBuffer( **buffer );
    else
      basic.setBuffer( nullptr );
    chained = true;
    return *this;
  }
  descriptor_buffer_info_t &descriptor_buffer_info_t::set_buffer( const std::shared_ptr< buffer_t > &v ) {
    buffer = v;
    chained = false;
    return *this;
  }
  descriptor_buffer_info_t &descriptor_buffer_info_t::clear_buffer() {
    buffer.reset();
    chained = false;
    return *this;
  }
}


