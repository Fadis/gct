#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/image.hpp>
#include <gct/image_view.hpp>
#include <gct/write_descriptor_set.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/named_resource.hpp>
#include <gct/image_filter.hpp>
#include <gct/image_filter_create_info.hpp>

namespace gct {
  image_filter::image_filter_impl::image_filter_impl(
    const image_filter_create_info &ci
  ) :
    property_type( ci ) {
    std::tie(descriptor_set_layout,pipeline) = props.allocator_set.pipeline_cache->get_pipeline( ci.shader );

    props.reusable = true;
    if( props.output.size() != props.input.size() ) {
      props.output.resize( props.input.size() );
      for( unsigned int i = 0u; i != props.input.size(); ++i ) {
        if( !props.output[ i ] ) {
          props.output[ i ] = props.allocator_set.allocator->create_image(
            props.output_create_info ?
            *props.output_create_info :
            props.input[ i ]->get_factory()->get_props(),
            VMA_MEMORY_USAGE_GPU_ONLY
          )->get_view();
        }
        else {
          props.reusable = false;
        }
      }
    }
    else {
      props.reusable = false;
    }
    for( unsigned int i = 0u; i != props.input.size(); ++i ) {
      descriptor_set.push_back( props.allocator_set.descriptor_pool->allocate(
        descriptor_set_layout
      ) );
      if( props.resources.empty() ) {
        descriptor_set.back()->update({
          { props.input_name, props.input[ i ], vk::ImageLayout::eGeneral },
          { props.output_name, props.output[ i ], vk::ImageLayout::eGeneral }
        });
      }
      else {
        std::vector< write_descriptor_set_t > temp;
        for( const auto &r: props.resources ) {
          temp.push_back( { r, i } );
        }
        temp.push_back( { props.input_name, props.input[ i ], vk::ImageLayout::eGeneral } );
        temp.push_back( { props.output_name, props.output[ i ], vk::ImageLayout::eGeneral } );
        descriptor_set.back()->update( temp );
      }
    }
  }
  image_filter::image_filter_impl::image_filter_impl(
    const std::shared_ptr< image_filter::image_filter_impl > &prev_,
    const image_filter_create_info &ci
  ) :
    property_type( ci ),
    prev( prev_ ) {
    props.allocator_set = prev_->props.allocator_set;
    props.input = prev->props.output;

    std::tie(descriptor_set_layout,pipeline) = props.allocator_set.pipeline_cache->get_pipeline( props.shader );
  
    props.reusable = true;
    const bool reusable = prev->prev && prev->prev->props.reusable && prev->props.reusable;
    if( props.output.size() != props.input.size() ) {
      props.output.resize( props.input.size() );
      for( unsigned int i = 0u; i != props.input.size(); ++i ) {
        if( !props.output[ i ] ) {
          if( reusable ) {
            props.output[ i ] = prev->props.input[ i ];
            prev->prev->reused = true;
          }
          else {
            props.output[ i ] = props.allocator_set.allocator->create_image(
              props.input[ i ]->get_factory()->get_props(),
              VMA_MEMORY_USAGE_GPU_ONLY
            )->get_view();
          }
        }
        else {
          props.reusable = false;
        }
      }
    }
    else {
      props.reusable = false;
    }
    {
      for( unsigned int i = 0u; i != props.input.size(); ++i ) {
        descriptor_set.push_back( props.allocator_set.descriptor_pool->allocate(
          descriptor_set_layout
        ) );
        if( props.resources.empty() ) {
          descriptor_set.back()->update({
            { props.input_name, props.input[ i ], vk::ImageLayout::eGeneral },
            { props.output_name, props.output[ i ], vk::ImageLayout::eGeneral }
          });
        }
        else {
          std::vector< write_descriptor_set_t > temp;
          for( const auto &r: props.resources ) {
            temp.push_back( { r, i } );
          }
          temp.push_back( { props.input_name, props.input[ i ], vk::ImageLayout::eGeneral } );
          temp.push_back( { props.output_name, props.output[ i ], vk::ImageLayout::eGeneral } );
          descriptor_set.back()->update( temp );
        }
      }
    }
  }
  void image_filter::image_filter_impl::operator()(
    command_buffer_recorder_t &rec,
    unsigned int image_index
  ) const {
    if( prev ) {
      (*prev)( rec, image_index );
    }
    if( props.input[ image_index ]->get_factory()->get_layout().get_uniform_layout() != vk::ImageLayout::eGeneral ) {
      rec.set_image_layout( props.input[ image_index ], vk::ImageLayout::eGeneral );
    }
    if( props.output[ image_index ]->get_factory()->get_layout().get_uniform_layout() != vk::ImageLayout::eGeneral ) {
      rec.set_image_layout( props.output[ image_index ], vk::ImageLayout::eGeneral );
    }
    rec.bind(
      pipeline,
      { descriptor_set[ image_index ] }
    );
    const auto &ici = props.input[ image_index ]->get_factory()->get_props().get_basic();
    rec.dispatch_threads( ici.extent.width, ici.extent.height, ici.arrayLayers );
    rec.compute_barrier(
      {},
      { props.output[ image_index ]->get_factory() }
    );
  }
  const std::vector< std::shared_ptr< image_view_t > > &image_filter::image_filter_impl::get_output() const {
    if( reused ) {
      throw -1;
    }
    return props.output;
  }
  image_filter::image_filter(
    const image_filter_create_info &ci
  ) : resources( new image_filter_impl(
    ci
  ) ) {}
  void image_filter::operator()(
    command_buffer_recorder_t &rec,
    unsigned int image_index
  ) const {
    if( resources->is_reused() ) {
      throw -1;
    }
    (*resources)( rec, image_index );
  }
  image_filter image_filter::operator()(
    const image_filter_create_info &ci
  ) const {
    return image_filter(
      resources,
      ci
    );
  }

}
