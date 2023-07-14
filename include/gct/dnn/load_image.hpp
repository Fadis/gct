#ifndef GCT_DNN_LOAD_IMAGE_HPP
#define GCT_DNN_LOAD_IMAGE_HPP
#include <memory>
#include <filesystem>
#include <gct/nnef_data.hpp>
#include <gct/command_buffer_recorder.hpp>

namespace gct {
class allocator_t;

namespace dnn {

void load_image(
  const std::shared_ptr< allocator_t > &allocator,
  const std::filesystem::path &filename,
  const nnef_data_t &dest,
  command_buffer_recorder_t &rec
);

}

}

#endif

