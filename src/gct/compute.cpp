#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/write_descriptor_set.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/named_resource.hpp>
#include <gct/compute.hpp>
#include <gct/pipeline_shader_stage_create_info.hpp>
#include <gct/shader_module.hpp>
#include <gct/shader_module_create_info.hpp>
#include <gct/shader_module_reflection.hpp>
#include <gct/compute_create_info.hpp>
#include <gct/compute_pipeline_create_info.hpp>
#include <gct/get_device.hpp>
#include <gct/device.hpp>

namespace gct {
  compute::compute(
    const compute_create_info &ci
  ) :
    property_type( ci ) {
    auto &device = get_device( *props.allocator_set.pipeline_cache );
    if( props.external_pipeline_layout ) {
      auto shader = device.get_shader_module( props.shader );
      std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_layout_t > > set_layouts;
      const auto &dsl = props.external_pipeline_layout->get_props().get_descriptor_set_layout();
      for( unsigned int i = 0u; i != dsl.size(); ++i ) {
        set_layouts.insert( std::make_pair( i, dsl[ i ] ) );
      }
      pipeline = props.allocator_set.pipeline_cache->get_pipeline(
        compute_pipeline_create_info_t()
          .set_dim( props.dim )
          .set_stage( shader, props.specs )
          .set_layout( props.external_pipeline_layout )
      );
      descriptor_set_layout = set_layouts;
    }
    else {
      std::tie(descriptor_set_layout,pipeline) = props.allocator_set.pipeline_cache->get_pipeline2( props.shader, props.descriptor_set_layout, props.specs, props.dim );
    }
    descriptor_set.resize( props.swapchain_image_count );
    std::shared_ptr< descriptor_set_layout_t > null_descriptor_set_layout;
    const auto &descriptor_set_layout_list = pipeline->get_props().get_layout()->get_props().get_descriptor_set_layout();
    for( unsigned int i = 0u; i != props.swapchain_image_count; ++i ) {
      descriptor_set[ i ].resize( descriptor_set_layout_list.size() );
    }
    for( std::size_t descriptor_set_layout_index = 0u; descriptor_set_layout_index != descriptor_set_layout_list.size(); ++descriptor_set_layout_index ) {
      auto &d = descriptor_set_layout_list[ descriptor_set_layout_index ];
      const auto ext = props.external_descriptor_set.find( descriptor_set_layout_index );
      if( ext != props.external_descriptor_set.end() ) {
        for( unsigned int i = 0u; i != props.swapchain_image_count; ++i ) {
          descriptor_set[ i ][ descriptor_set_layout_index ] = ext->second;
          descriptor_set_layout[ descriptor_set_layout_index ] = ext->second->get_props().get_layout()[ 0 ];
        }
      }
      else {
        for( unsigned int i = 0u; i != props.swapchain_image_count; ++i ) {
          descriptor_set[ i ][ descriptor_set_layout_index ] = props.allocator_set.descriptor_pool->allocate(
            d
          );
        }
      }
    }
    for( unsigned int i = 0u; i != descriptor_set.size(); ++i ) {
      std::vector< write_descriptor_set_t > temp;
      for( const auto &r: props.resources ) {
        temp.push_back( { r, i } );
      }
      if( props.ignore_unused_descriptor ) {
        for( unsigned int did = 0u; did != descriptor_set[ i ].size(); ++did ) {
          if( descriptor_set[ i ][ did ] ) {
            const auto ext = props.external_descriptor_set.find( did );
            if( ext == props.external_descriptor_set.end() ) {
              descriptor_set[ i ][ did ]->update( temp, props.ignore_unused_descriptor );
            }
          }
        }
      }
      else {
        if( !descriptor_set[ i ].empty() ) {
          if( descriptor_set[ i ][ 0 ] ) {
            const auto ext = props.external_descriptor_set.find( 0u );
            if( ext == props.external_descriptor_set.end() ) {
              descriptor_set[ i ][ 0 ]->update( temp, props.ignore_unused_descriptor );
            }
          }
        }
      }
    }
    auto pcmp = get_reflection().get_push_constant_member_pointer_maybe( "PushConstants" );
    if( pcmp ) {
      enable_push_constant( *pcmp );
    }
  }
  void compute::operator()(
    command_buffer_recorder_t &rec,
    unsigned int image_index,
    unsigned int x,
    unsigned int y,
    unsigned int z
  ) const {
    rec.bind(
      pipeline,
      descriptor_set[ image_index ]
    );
    push_constant_storage::operator()( rec, pipeline );
    rec.dispatch_threads( x, y, z );
  }
  const shader_module_reflection_t &compute::get_reflection() const {
    return pipeline->get_props().get_stage().get_shader_module()->get_props().get_reflection(); 
  }
  void to_json( nlohmann::json &dest, const compute &src ) {
    dest = nlohmann::json::object();
    dest[ "props" ] = src.get_props();
  }
}
