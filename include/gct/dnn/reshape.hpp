#ifndef GCT_DNN_RESHAPE_HPP
#define GCT_DNN_RESHAPE_HPP
#include <cstdint>
#include <memory>
#include <array>
#include <unordered_map>
#include <gct/nnef_data.hpp>
#include <gct/dnn/nnef.hpp>

namespace gct {

namespace dnn {

namespace operation {

class reshape : public basic_operation {
public:
  reshape(
    const nnef::Operation &op,
    const std::unordered_map< std::string, nnef_data_t > &bufs
  );
  void operator()( command_buffer_recorder_t &rec );
private:
  nnef_data_t input;
};

}

}

}

#endif

