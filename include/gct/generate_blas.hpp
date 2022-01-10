#ifndef LIBGCT_INCLUDE_GCT_GENERATE_BLAS_HPP
#define LIBGCT_INCLUDE_GCT_GENERATE_BLAS_HPP
#include <memory>
#include <tuple>
#include <vector>
#include <fx/gltf.h>
#include <vulkan/vulkan.hpp>
namespace gct {
  class acceleration_structure_t;
  class command_buffer_recorder_t;
  class allocator_t;
  namespace gltf {
    std::tuple<
      std::vector< std::shared_ptr< acceleration_structure_t > >,
      std::vector< std::shared_ptr< gct::buffer_t > >,
      std::vector< std::uint32_t >
    >
    generate_blas(
      const document_t &doc,
      command_buffer_recorder_t &command_buffer,
      const std::shared_ptr< allocator_t > &allocator
    );
  }
}
#endif

