#ifndef GCT_DNN_NNEF_HPP
#define GCT_DNN_NNEF_HPP
#include <cstdint>
#include <string>
#include <filesystem>
#include <memory>
#include <unordered_map>
#include <nnef.h>
#include <gct/numeric_types.hpp>
#include <gct/nnef_data.hpp>
#include <gct/command_buffer_recorder.hpp>

namespace gct {

class device_t;
class shader_module_t;

namespace dnn {

const nnef_data_t &get_buffer(
  const nnef::Operation &op,
  const std::unordered_map< std::string, nnef_data_t > &bufs,
  const std::string &identifier
);
const nnef_data_t &get_buffer_maybe(
  const nnef::Operation &op,
  const std::unordered_map< std::string, nnef_data_t > &bufs,
  const std::string &identifier
);

template< typename Iterator >
std::size_t get_length( Iterator begin, Iterator end ) {
  std::size_t temp = 1u;
  for( auto iter = begin; iter != end; ++iter ) {
    temp *= *iter;
  }
  return temp;
}

std::string numeric_type_to_suffix( const numeric_type_t &type );

std::unordered_map< std::string, std::shared_ptr< shader_module_t > > load_shaders(
  const std::shared_ptr< device_t > &, 
  const std::filesystem::path &
);

namespace operation {
class basic_operation {
public:
  virtual void operator()( command_buffer_recorder_t &rec ) = 0;
  const nnef_data_t &get_output() const { return output; }
protected:
  nnef_data_t output;
};

}

}

}

#endif

