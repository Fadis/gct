#ifndef LIBGCT_INCLUDE_GCT_GENERATE_TLAS_HPP
#define LIBGCT_INCLUDE_GCT_GENERATE_TLAS_HPP
#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>
namespace gct {
  class acceleration_structure_t;
  class command_buffer_recorder_t;
  class allocator_t;
  namespace gltf {
    [[nodiscard]] std::shared_ptr< acceleration_structure_t >
    generate_tlas(
      const document_t &doc,
      command_buffer_recorder_t &rec,
      const std::shared_ptr< allocator_t > &allocator,
      const std::vector< std::shared_ptr< acceleration_structure_t > > &as,
      const std::vector< std::shared_ptr< gct::buffer_t > > &as_buf,
      const std::vector< std::uint32_t > &mesh2instance
    );
    [[nodiscard]] std::shared_ptr< acceleration_structure_t >
    generate_tlas(
      command_buffer_recorder_t &rec,
      const std::shared_ptr< allocator_t > &allocator,
      const std::vector< std::shared_ptr< acceleration_structure_t > > &blas
    );
  }
}
#endif

