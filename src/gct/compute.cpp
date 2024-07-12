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

namespace gct {
  compute::compute(
    const compute_create_info &ci
  ) :
    props( ci ) {
    std::tie(descriptor_set_layout,pipeline) = props.pipeline_cache->get_pipeline( ci.shader );

    if( props.external_descriptor_set ) {
      std::vector< write_descriptor_set_t > temp;
      for( const auto &r: props.resources ) {
        temp.push_back( { r, 0 } );
      }
      props.external_descriptor_set->update( temp, props.ignore_unused_descriptor );
    }
    else {
      for( unsigned int i = 0u; i != props.swapchain_image_count; ++i ) {
        descriptor_set.push_back( props.descriptor_pool->allocate(
          descriptor_set_layout
        ) );
        std::vector< write_descriptor_set_t > temp;
        for( const auto &r: props.resources ) {
          temp.push_back( { r, i } );
        }
        descriptor_set.back()->update( temp, props.ignore_unused_descriptor );
      }
    }
  }
  void compute::operator()(
    command_buffer_recorder_t &rec,
    unsigned int image_index,
    unsigned int x,
    unsigned int y,
    unsigned int z
  ) const {
    if( props.external_descriptor_set ) {
      rec.bind(
        pipeline,
        { props.external_descriptor_set }
      );
    }
    else {
      rec.bind(
        pipeline,
        { descriptor_set[ image_index ] }
      );
    }
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
