#ifndef GCT_DNN_GRAPH_HPP
#define GCT_DNN_GRAPH_HPP
#include <cstdint>
#include <string>
#include <filesystem>
#include <memory>
#include <unordered_map>
#include <nnef.h>
#include <gct/numeric_types.hpp>
#include <gct/nnef_data.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/dnn/nnef.hpp>

namespace gct {

class device_t;
class shader_module_t;
class allocator_t;
class descriptor_pool_t;
class pipeline_cache_t;

namespace dnn {

class graph {
public:
  graph(
    const std::shared_ptr< device_t > &device,
    const std::shared_ptr< allocator_t > &allocator,
    const std::shared_ptr< descriptor_pool_t > &descriptor_pool,
    const std::shared_ptr< pipeline_cache_t > &pipeline_cache,
    const std::filesystem::path &dir,
    const std::filesystem::path &shader_dir,
    command_buffer_recorder_t &rec
  );
  const nnef_data_t &get_input() const { return input; }
  const nnef_data_t &get_output() const { return output; }
  void operator()( command_buffer_recorder_t &rec );
private:
  std::unordered_map< std::string, std::shared_ptr< shader_module_t > > shaders;
  nnef_data_t input;
  nnef_data_t output;
  std::unordered_map< std::string, nnef_data_t > bufs;
  std::vector< std::shared_ptr< operation::basic_operation > > ops;

};

}

}

#endif

