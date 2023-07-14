#ifndef GCT_DNN_CONVOLUTION_HPP
#define GCT_DNN_CONVOLUTION_HPP
#include <cstdint>
#include <memory>
#include <array>
#include <unordered_map>
#include <gct/nnef_data.hpp>
#include <gct/dnn/nnef.hpp>

namespace gct {

class allocator_t;
class descriptor_pool_t;
class pipeline_cache_t;
class shader_module_t;
class descriptor_set_t;
class pipeline_layout_t;
class compute_pipeline_t;

namespace dnn {

namespace operation {

class convolution : public basic_operation {
  struct spec_t {
    std::int32_t filter_size_x = 0;
    std::int32_t filter_size_y = 0;
    std::int32_t filter_size_z = 0;
    std::int32_t lpadding = 0;
    std::int32_t rpadding = 0;
    std::int32_t tpadding = 0;
    std::int32_t bpadding = 0;
    std::int32_t stride_x = 0;
    std::int32_t stride_y = 0;
    std::int32_t dilation_x = 0;
    std::int32_t dilation_y = 0;
    std::int32_t input_dim_x = 0;
    std::int32_t input_dim_y = 0;
    std::int32_t input_dim_z = 0;
    std::int32_t output_dim_x = 0;
    std::int32_t output_dim_y = 0;
    std::int32_t output_dim_z = 0;
    float border_value = 0.0f;
    std::int32_t bias_mode = 0;
    float bias_value = 0.0f;
  };
public:
  convolution(
    const std::shared_ptr< allocator_t > &allocator,
    const std::shared_ptr< descriptor_pool_t > &descriptor_pool,
    const std::shared_ptr< pipeline_cache_t > &pipeline_cache,
    const nnef::Operation &op,
    const std::unordered_map< std::string, std::shared_ptr< shader_module_t > > shaders,
    const std::unordered_map< std::string, nnef_data_t > &bufs
  );
  void operator()( command_buffer_recorder_t &rec );
private:
  nnef_data_t input;
  nnef_data_t weight;
  nnef_data_t bias;
  std::array< std::uint32_t, 3u > exec_dim;
  std::shared_ptr< descriptor_set_t > descriptor_set;
  std::shared_ptr< pipeline_layout_t > pipeline_layout;
  std::shared_ptr< compute_pipeline_t > pipeline;
};

}

}

}

#endif

