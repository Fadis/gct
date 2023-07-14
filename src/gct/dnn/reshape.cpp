#include <gct/nnef_data.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/pipeline_layout_create_info.hpp>
#include <gct/buffer_view_create_info.hpp>
#include <gct/shader_module.hpp>
#include <gct/compute_pipeline_create_info.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/write_descriptor_set.hpp>
#include <gct/device.hpp>
#include <gct/get_device.hpp>
#include <gct/buffer.hpp>
#include <gct/image.hpp>
#include <gct/dnn/nnef.hpp>
#include <gct/dnn/reshape.hpp>

namespace gct::dnn::operation {

reshape::reshape(
  const nnef::Operation &op,
  const std::unordered_map< std::string, nnef_data_t > &bufs
) :
  input( get_buffer( op, bufs, "input" ) ) {
  if( input.dim.size() < 1u ) {
    throw -1;
  }

  const auto shape = std::find_if(
    op.attribs.begin(), op.attribs.end(),
    []( const auto &v ) {
      return v.first == "shape";
    }
  );
  if( shape == op.attribs.end() ) {
    throw -1;
  }
  {
    auto len = get_length( input.dim.begin(), input.dim.end() );
    for( const auto &v: shape->second.array() ) {
      const auto div = v.integer();
      if( div != -1 ) {
        output.dim.push_back( div );
        len /= div;
      }
      else {
        output.dim.push_back( len );
        break;
      }
    }
  }
  output.type = input.type;
  output.buffer = input.buffer;
}
void reshape::operator()( command_buffer_recorder_t& ) {
}

}

