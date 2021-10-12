#include <fstream>
#include <iterator>
#include <gct/write_descriptor_set.hpp>

namespace gct {
  write_descriptor_set_t &write_descriptor_set_t::rebuild_chain() {
    if( image ) {
      image->rebuild_chain();
      basic.setPImageInfo( &image->get_basic() );
    }
    else
      basic.setPImageInfo( nullptr );
    if( buffer ) {
      buffer->rebuild_chain();
      basic.setPBufferInfo( &buffer->get_basic() );
    }
    else
      basic.setPBufferInfo( nullptr );
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( acceleration_structure )
#endif
#ifdef VK_NV_RAY_TRACING_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( acceleration_structure_nv )
#endif
#ifdef VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( inline_uniform_block )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  write_descriptor_set_t &write_descriptor_set_t::set_image( const descriptor_image_info_t &v ) {
    image.reset( new descriptor_image_info_t( v ) );
    chained = false;
    return *this;
  }
  write_descriptor_set_t &write_descriptor_set_t::clear_image() {
    image.reset();
    chained = false;
    return *this;
  }
  write_descriptor_set_t &write_descriptor_set_t::set_buffer( const descriptor_buffer_info_t &v ) {
    buffer.reset( new descriptor_buffer_info_t( v ) );
    chained = false;
    return *this;
  }
  write_descriptor_set_t &write_descriptor_set_t::clear_buffer() {
    buffer.reset();
    chained = false;
    return *this;
  }
}

