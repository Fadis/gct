#ifndef GCT_CUBEMAP_MATRIX_GENERATOR_HPP
#define GCT_CUBEMAP_MATRIX_GENERATOR_HPP
#include <memory>
#include <vector>
#include <filesystem>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace gct {
  class allocator_t;
  class descriptor_pool_t;
  class pipeline_cache_t;
  class descriptor_set_layout_t;
  class compute_pipeline_t;
  class buffer_t;
  class command_buffer_recorder_t;
  class cubemap_matrix_generator {
    struct camera_state {
      glm::vec3 pos = glm::vec3( 0.f, 0.f, 0.f );
      float near = 0.f;
      float far = 0.f;
    };
  public:
    struct matrices_t {
      glm::mat4 camera_projection[ 6 ];
      glm::mat4 camera[ 6 ];
    };
    cubemap_matrix_generator(
      const std::shared_ptr< allocator_t > &allocator,
      const std::shared_ptr< descriptor_pool_t > &descriptor_pool,
      const std::shared_ptr< pipeline_cache_t > &pipeline_cache,
      const std::filesystem::path &mat_shader,
      unsigned int buffer_count = 1u
    );
    void operator()(
      command_buffer_recorder_t &rec,
      const glm::vec3 &pos,
      float near,
      float far,
      unsigned int buffer_index = 0u
    ) const;
    [[nodiscard]] const std::vector< std::shared_ptr< buffer_t > > &get_buffer() const {
      return storage;
    }
  private:
    std::shared_ptr< descriptor_set_layout_t > descriptor_set_layout;
    std::vector< std::shared_ptr< descriptor_set_t > > descriptor_set;
    std::shared_ptr< compute_pipeline_t > pipeline;
    std::vector< std::shared_ptr< buffer_t > > storage;
    mutable std::vector< camera_state > prev;
  };
}

#endif

