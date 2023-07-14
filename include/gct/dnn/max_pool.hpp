#ifndef GCT_DNN_MAX_POOL_HPP
#define GCT_DNN_MAX_POOL_HPP
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

class max_pool : public basic_operation {
  struct spec_t {
    std::uint32_t filter_size_x = 1;
    std::uint32_t filter_size_y = 1;
    std::uint32_t filter_size_z = 1;
    std::uint32_t filter_size_w = 1;
    std::uint32_t lpadding = 0;
    std::uint32_t rpadding = 0;
    std::uint32_t tpadding = 0;
    std::uint32_t bpadding = 0;
    std::uint32_t stride_x = 1;
    std::uint32_t stride_y = 1;
    std::uint32_t stride_z = 1;
    std::uint32_t stride_w = 1;
    std::uint32_t input_dim_x = 0;
    std::uint32_t input_dim_y = 0;
    std::uint32_t input_dim_z = 0;
    std::uint32_t input_dim_w = 0;
    std::uint32_t output_dim_x = 0;
    std::uint32_t output_dim_y = 0;
    std::uint32_t output_dim_z = 0;
    std::uint32_t output_dim_w = 0;
    float border_value = 0.0f;
  };
public:
  max_pool(
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
  std::array< std::uint32_t, 3u > exec_dim;
  std::shared_ptr< descriptor_set_t > descriptor_set;
  std::shared_ptr< pipeline_layout_t > pipeline_layout;
  std::shared_ptr< compute_pipeline_t > pipeline;
};

}

}

}

#endif

