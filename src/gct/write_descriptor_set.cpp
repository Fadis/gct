#include <fstream>
#include <iterator>
#include <gct/write_descriptor_set.hpp>
#include <gct/acceleration_structure.hpp>

namespace gct {
  write_descriptor_set_t &write_descriptor_set_t::rebuild_chain() {
    if( !image.empty() ) {
      raw_image.clear();
      raw_image.reserve( image.size() );
      std::transform(
        image.begin(),
        image.end(),
        std::back_inserter( raw_image ),
        []( auto &v ) {
          v.rebuild_chain();
          return v.get_basic();
        }
      );
      basic.setDescriptorCount( raw_image.size() );
      basic.setPImageInfo( raw_image.data() );
      basic.setPBufferInfo( nullptr );
      basic.setPTexelBufferView( nullptr );
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
      clear_acceleration_structure();
#endif
    }
    else if( !buffer.empty() ) {
      raw_buffer.clear();
      raw_buffer.reserve( buffer.size() );
      std::transform(
        buffer.begin(),
        buffer.end(),
        std::back_inserter( raw_buffer ),
        []( auto &v ) {
          v.rebuild_chain();
          return v.get_basic();
        }
      );
      basic.setDescriptorCount( raw_buffer.size() );
      basic.setPImageInfo( nullptr );
      basic.setPBufferInfo( raw_buffer.data() );
      basic.setPTexelBufferView( nullptr );
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
      clear_acceleration_structure();
#endif
    }
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
    else if( !acceleration_structure_handles.empty() ) {
      raw_acceleration_structure.clear();
      raw_acceleration_structure.reserve( acceleration_structure_handles.size() );
      std::transform(
        acceleration_structure_handles.begin(),
        acceleration_structure_handles.end(),
        std::back_inserter( raw_acceleration_structure ),
        []( auto &v ) {
          return **v;
        }
      );
      set_acceleration_structure(
        vk::WriteDescriptorSetAccelerationStructureKHR()
          .setAccelerationStructureCount( raw_acceleration_structure.size() )
          .setPAccelerationStructures( raw_acceleration_structure.data() )
      );
      basic.setDescriptorCount( raw_acceleration_structure.size() );
      basic.setPImageInfo( nullptr );
      basic.setPBufferInfo( nullptr );
      basic.setPTexelBufferView( nullptr );
    }
#endif
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
  write_descriptor_set_t &write_descriptor_set_t::add_image( const descriptor_image_info_t &v ) {
    image.push_back( v );
    buffer.clear();
    chained = false;
    return *this;
  }
  write_descriptor_set_t &write_descriptor_set_t::clear_image() {
    image.clear();
    chained = false;
    return *this;
  }
  write_descriptor_set_t &write_descriptor_set_t::add_buffer( const descriptor_buffer_info_t &v ) {
    buffer.push_back( v );
    image.clear();
    chained = false;
    return *this;
  }
  write_descriptor_set_t &write_descriptor_set_t::clear_buffer() {
    buffer.clear();
    chained = false;
    return *this;
  }
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
  write_descriptor_set_t &write_descriptor_set_t::add_acceleration_structure_handle( const std::shared_ptr< gct::acceleration_structure_t > &v ) {
    acceleration_structure_handles.push_back( v );
    chained = false;
    return *this;
  }
  write_descriptor_set_t &write_descriptor_set_t::clear_acceleration_structure_handle() {
    acceleration_structure_handles.clear();
    chained = false;
    return *this;
  }
#endif
}

