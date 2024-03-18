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
#include <gct/compute_create_info.hpp>

namespace gct {
  compute::compute(
    const compute_create_info &ci
  ) :
    props( ci ) {
    std::tie(descriptor_set_layout,pipeline) = props.pipeline_cache->get_pipeline( ci.shader );

    for( unsigned int i = 0u; i != props.swapchain_image_count; ++i ) {
      descriptor_set.push_back( props.descriptor_pool->allocate(
        descriptor_set_layout
      ) );
      std::vector< write_descriptor_set_t > temp;
      for( const auto &r: props.resources ) {
        temp.push_back( { r, i } );
      }
      descriptor_set.back()->update( temp );
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
      { descriptor_set[ image_index ] }
    );
    rec.dispatch_threads( x, y, z );
  }
  void to_json( nlohmann::json &dest, const compute &src ) {
    dest = nlohmann::json::object();
    dest[ "props" ] = src.get_props();
  }
}
