#ifndef GCT_CUBEMAP_MATRIX_GENERATOR2_HPP
#define GCT_CUBEMAP_MATRIX_GENERATOR2_HPP
#include <memory>
#include <gct/cubemap_matrix_generator2_create_info.hpp>
#include <gct/property.hpp>
#include <gct/shader_graph.hpp>

namespace gct {
  class image_view_t;
  class cubemap_matrix_generator2 :
    public property< cubemap_matrix_generator2_create_info > {
  public:
    cubemap_matrix_generator2(
      const cubemap_matrix_generator2_create_info &ci
    );
    void operator()(
      command_buffer_recorder_t &rec,
      const glm::vec3 &pos,
      float near,
      float far,
      const matrix_pool::matrix_descriptor &camera,
      const matrix_pool::matrix_descriptor &projection
    ) const;
  private:
    std::shared_ptr< compute > generate;
  };
}

#endif


