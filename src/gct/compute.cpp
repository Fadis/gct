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
    if( props.external_pipeline_layout ) {
      auto &device = get_device( *props.allocator_set.pipeline_cache );
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
    for( auto &d: descriptor_set_layout ) {
      for( unsigned int i = 0u; i != props.swapchain_image_count; ++i ) {
        if( descriptor_set[ i ].size() <= d.first ) {
          descriptor_set[ i ].resize( d.first + 1u );
        }
      }
      const auto ext = props.external_descriptor_set.find( d.first );
      if( ext != props.external_descriptor_set.end() ) {
        for( unsigned int i = 0u; i != props.swapchain_image_count; ++i ) {
          descriptor_set[ i ][ d.first ] = ext->second;
          descriptor_set_layout[ d.first ] = ext->second->get_props().get_layout()[ 0 ];
        }
      }
      else {
        for( unsigned int i = 0u; i != props.swapchain_image_count; ++i ) {
          descriptor_set[ i ][ d.first ] = props.allocator_set.descriptor_pool->allocate(
            d.second
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
      push_constant_mp = *pcmp;
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
    if( use_internal_push_constant && push_constant_mp && !push_constant.empty() ) {
      rec->pushConstants(
        **pipeline->get_props().get_layout(),
        pipeline->get_props().get_layout()->get_props().get_push_constant_range()[ 0 ].stageFlags,
        push_constant_mp->get_offset(),
        push_constant.size(),
        push_constant.data()
      );
    }
    rec.dispatch_threads( x, y, z );
  }
  const shader_module_reflection_t &compute::get_reflection() const {
    return pipeline->get_props().get_stage().get_shader_module()->get_props().get_reflection(); 
  }
  std::vector< std::uint8_t > &compute::get_push_constant() const {
    if( push_constant_mp ) {
      use_internal_push_constant = true;
      push_constant.resize( push_constant_mp->get_aligned_size(), 0u );
    }
    return push_constant;
  }
  void to_json( nlohmann::json &dest, const compute &src ) {
    dest = nlohmann::json::object();
    dest[ "props" ] = src.get_props();
  }
}
