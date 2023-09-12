#include <fstream>
#include <iterator>
#include <gct/write_descriptor_set.hpp>
#include <gct/acceleration_structure.hpp>
#include <gct/buffer.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/image_view.hpp>
#include <gct/image.hpp>
#include <gct/sampler.hpp>
#include <gct/named_resource.hpp>

namespace gct {
  write_descriptor_set_t::write_descriptor_set_t(
    const std::string &name,
    const std::shared_ptr< image_view_t > &image_view
  ) {
    set_name( name );
    add_image( image_view );
  }
  write_descriptor_set_t::write_descriptor_set_t(
    const std::string &name,
    const std::shared_ptr< sampler_t > &sampler,
    const std::shared_ptr< image_view_t > &image_view
  ) {
    set_name( name );
    add_image( sampler, image_view );
  }
  write_descriptor_set_t::write_descriptor_set_t(
    const std::string &name,
    const std::shared_ptr< image_view_t > &image_view,
    vk::ImageLayout layout
  ) {
    set_name( name );
    add_image( image_view, layout );
  }
  write_descriptor_set_t::write_descriptor_set_t(
    const std::string &name,
    const std::shared_ptr< sampler_t > &sampler,
    const std::shared_ptr< image_view_t > &image_view,
    vk::ImageLayout layout
  ) {
    set_name( name );
    add_image( sampler, image_view, layout );
  }
  write_descriptor_set_t::write_descriptor_set_t(
    const std::string &name,
    const std::shared_ptr< buffer_t > &buffer
  ) {
    set_name( name );
    add_buffer( buffer );
  }
  write_descriptor_set_t::write_descriptor_set_t(
    const std::string &name,
    const std::shared_ptr< mappable_buffer_t > &buffer
  ) {
    set_name( name );
    add_buffer( buffer );
  }
  write_descriptor_set_t::write_descriptor_set_t(
    const named_resource &r,
    unsigned int index
  ) {
    add( r, index );
  }
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
  write_descriptor_set_t &write_descriptor_set_t::add_image( const std::shared_ptr< image_view_t > &v ) {
    return add_image(
      gct::descriptor_image_info_t()
        .set_basic(
          vk::DescriptorImageInfo()
            .setImageLayout(
              v->get_factory()->get_layout().get_uniform_layout()
            )
        )
        .set_image_view( v )
    );
  }
  write_descriptor_set_t &write_descriptor_set_t::add_image(
    const std::shared_ptr< sampler_t > &sampler,
    const std::shared_ptr< image_view_t > &image_view
  ) {
    return add_image(
      gct::descriptor_image_info_t()
        .set_basic(
          vk::DescriptorImageInfo()
            .setImageLayout(
              image_view->get_factory()->get_layout().get_uniform_layout()
            )
        )
        .set_sampler( sampler )
        .set_image_view( image_view )
    );
  }
  write_descriptor_set_t &write_descriptor_set_t::add_image(
    const std::shared_ptr< image_view_t > &image_view,
    vk::ImageLayout layout
  ) {
    return add_image(
      gct::descriptor_image_info_t()
        .set_basic(
          vk::DescriptorImageInfo()
            .setImageLayout( layout )
        )
        .set_image_view( image_view )
    );
  }
  write_descriptor_set_t &write_descriptor_set_t::add_image(
    const std::shared_ptr< sampler_t > &sampler,
    const std::shared_ptr< image_view_t > &image_view,
    vk::ImageLayout layout
  ) {
    return add_image(
      gct::descriptor_image_info_t()
        .set_basic(
          vk::DescriptorImageInfo()
            .setImageLayout( layout )
        )
        .set_sampler( sampler )
        .set_image_view( image_view )
    );
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
  write_descriptor_set_t &write_descriptor_set_t::add_buffer( const std::shared_ptr< buffer_t > &v ) {
    return add_buffer(
      gct::descriptor_buffer_info_t()
        .set_buffer( v )
        .set_basic(
          vk::DescriptorBufferInfo()
            .setOffset( 0 )
            .setRange( v->get_props().get_basic().size )
        )
    );
  }
  write_descriptor_set_t &write_descriptor_set_t::add_buffer( const std::shared_ptr< mappable_buffer_t > &v ) {
    return add_buffer( v->get_buffer() );
  }
  write_descriptor_set_t &write_descriptor_set_t::clear_buffer() {
    buffer.clear();
    chained = false;
    return *this;
  }
  write_descriptor_set_t &write_descriptor_set_t::add(
    const named_resource &r,
    unsigned int index
  ) {
    set_name( r.get_name() );
    set_index( r.get_index() );
    if( r.is_uniform() ) {
      if( r.is_buffer() ) {
        add_buffer( r.get_uniform_buffer() );
      }
      else if( r.is_image() ) {
        add_image( r.get_uniform_image() );
      }
      else if( r.is_combined_image() ) {
        const auto & [s,i] = r.get_uniform_combined_image();
        add_image( s, i );
      }
      else if( r.is_image_with_layout() ) {
        const auto & [i,l] = r.get_uniform_image_with_layout();
        add_image( i, l );
      }
      else if( r.is_combined_image_with_layout() ) {
        const auto & [s,i,l] = r.get_uniform_combined_image_with_layout();
        add_image( s, i, l );
      }
      else {
        throw -1;
      }
    }
    else {
      if( r.is_buffer() ) {
        add_buffer( r.get_buffer()[ index ] );
      }
      else if( r.is_image() ) {
        add_image( r.get_image()[ index ] );
      }
      else if( r.is_combined_image() ) {
        const auto & [s,i] = r.get_combined_image()[ index ];
        add_image( s, i );
      }
      else if( r.is_image_with_layout() ) {
        const auto & [i,l] = r.get_image_with_layout()[ index ];
        add_image( i, l );
      }
      else if( r.is_combined_image_with_layout() ) {
        const auto & [s,i,l] = r.get_combined_image_with_layout()[ index ];
        add_image( s, i, l );
      }
      else {
        throw -1;
      }
    }
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

