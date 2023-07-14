#include <gct/device.hpp>
#include <gct/shader_module.hpp>
#include <gct/allocator.hpp>
#include <gct/dnn/linear.hpp>
#include <gct/dnn/convolution.hpp>
#include <gct/dnn/relu.hpp>
#include <gct/dnn/softmax.hpp>
#include <gct/dnn/max_pool.hpp>
#include <gct/dnn/reshape.hpp>
#include <gct/dnn/nnef.hpp>
#include <gct/dnn/graph.hpp>

namespace gct::dnn {

namespace {
std::string get_output_name(
  const nnef::Operation &o
) {
  if( o.outputs.size() != 1 ) {
    throw -1;
  }
  return o.outputs[ 0 ].second.identifier();
}
}

graph::graph(
  const std::shared_ptr< device_t > &device,
  const std::shared_ptr< allocator_t > &allocator,
  const std::shared_ptr< descriptor_pool_t > &descriptor_pool,
  const std::shared_ptr< pipeline_cache_t > &pipeline_cache,
  const std::filesystem::path &dir,
  const std::filesystem::path &shader_dir,
  command_buffer_recorder_t &rec
) {
  nnef::Graph parsed;
  std::string error;
  if( !nnef::load_graph( dir.string(), parsed, error, "" ) ) {
    std::cerr << error << std::endl;
    throw -1;
  }
  if( !nnef::infer_shapes( parsed, error ) ) {
    std::cerr << error << std::endl;
    throw -1;
  }
  if( !nnef::allocate_buffers( parsed, error ) ) {
    std::cerr << error << std::endl;
    throw -1;
  }
  shaders = load_shaders(
    device,
    shader_dir
  );
  for( const auto &o: parsed.operations ) {
    if( o.name == "variable" ) {
      const std::string name = get_output_name( o );
      const auto label = std::find_if(
        o.attribs.begin(), o.attribs.end(),
        []( const auto &v ) {
          return v.first == "label";
        }
      );
      if( label == o.attribs.end() ) {
        throw -1;
      }
      const auto data_name = label->second.string();
      const auto data_filename = dir / ( data_name + ".dat" );
      auto nnef_data = rec.load_nnef_data(
        allocator,
        std::filesystem::absolute( data_filename ),
        vk::BufferUsageFlagBits::eStorageBuffer|
        vk::BufferUsageFlagBits::eTransferDst
      );
      bufs.insert(
        std::make_pair(
          name,
          nnef_data
        )
      );
    }
    else if( o.name == "external" ) {
      const std::string name = get_output_name( o );
      const auto shape = std::find_if(
        o.attribs.begin(), o.attribs.end(),
        []( const auto &v ) {
          return v.first == "shape";
        }
      );
      if( shape == o.attribs.end() ) {
        throw -1;
      }
      nnef_data_t buf;
      buf.type.component = numeric_component_type_t::float_;
      buf.type.depth = 32u;
      buf.type.rows = 1u;
      for( const auto &v: shape->second.array() ) {
        buf.dim.push_back( v.integer() );
      }
      buf.buffer = allocator->create_buffer(
        sizeof( float ) * get_length( buf.dim.begin(), buf.dim.end() ),
        vk::BufferUsageFlagBits::eStorageBuffer|
        vk::BufferUsageFlagBits::eTransferDst|
        vk::BufferUsageFlagBits::eTransferSrc,
        VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE
      );
      bufs.insert(
        std::make_pair(
          name,
          buf
        )
      );
    }
  }
  for( const auto &o: parsed.operations ) {
    if( o.name == "conv" ) {
      const std::string name = get_output_name( o );
      auto op = std::make_shared< operation::convolution >(
        allocator,
        descriptor_pool,
        pipeline_cache,
        o,
        shaders,
        bufs
      );
      bufs.insert(
        std::make_pair(
          name,
          op->get_output()
        )
      );
      ops.push_back( op );
    }
    else if( o.name == "linear" ) {
      const std::string name = get_output_name( o );
      auto op = std::make_shared< operation::linear >(
        allocator,
        descriptor_pool,
        pipeline_cache,
        o,
        shaders,
        bufs
      );
      bufs.insert(
        std::make_pair(
          name,
          op->get_output()
        )
      );
      ops.push_back( op );
    }
    else if( o.name == "relu" ) {
      const std::string name = get_output_name( o );
      auto op = std::make_shared< operation::relu >(
        allocator,
        descriptor_pool,
        pipeline_cache,
        o,
        shaders,
        bufs
      );
      bufs.insert(
        std::make_pair(
          name,
          op->get_output()
        )
      );
      ops.push_back( op );
    }
    else if( o.name == "relu" ) {
      const std::string name = get_output_name( o );
      auto op = std::make_shared< operation::relu >(
        allocator,
        descriptor_pool,
        pipeline_cache,
        o,
        shaders,
        bufs
      );
      bufs.insert(
        std::make_pair(
          name,
          op->get_output()
        )
      );
      ops.push_back( op );
    }
    else if( o.name == "softmax" ) {
      const std::string name = get_output_name( o );
      auto op = std::make_shared< operation::softmax >(
        allocator,
        descriptor_pool,
        pipeline_cache,
        o,
        shaders,
        bufs
      );
      bufs.insert(
        std::make_pair(
          name,
          op->get_output()
        )
      );
      ops.push_back( op );
    }
    else if( o.name == "max_pool" ) {
      const std::string name = get_output_name( o );
      auto op = std::make_shared< operation::max_pool >(
        allocator,
        descriptor_pool,
        pipeline_cache,
        o,
        shaders,
        bufs
      );
      bufs.insert(
        std::make_pair(
          name,
          op->get_output()
        )
      );
      ops.push_back( op );
    }
    else if( o.name == "reshape" ) {
      const std::string name = get_output_name( o );
      auto op = std::make_shared< operation::reshape >(
        o,
        bufs
      );
      bufs.insert(
        std::make_pair(
          name,
          op->get_output()
        )
      );
      ops.push_back( op );
      
    }
    else if( o.name == "variable" ) {}
    else if( o.name == "external" ) {}
    else {
      throw -1;
    }
  }
  if( parsed.inputs.size() != 1u ) {
    throw -1;
  }
  const auto input_buf = bufs.find( parsed.inputs[ 0 ] );
  if( input_buf == bufs.end() ) {
    throw -1;
  }
  input = input_buf->second;
  const auto output_buf = bufs.find( parsed.outputs[ 0 ] );
  if( output_buf == bufs.end() ) {
    throw -1;
  }
  output = output_buf->second;
}
void graph::operator()( command_buffer_recorder_t &rec ) {
  for( const auto &op: ops ) {
    (*op)( rec );
  }
}

}

