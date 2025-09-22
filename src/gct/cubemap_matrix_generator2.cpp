#include <gct/allocator.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/compute.hpp>
#include <gct/cubemap_matrix_generator2.hpp>
#include <gct/get_library_path.hpp>

namespace gct {

cubemap_matrix_generator2::cubemap_matrix_generator2(
  const cubemap_matrix_generator2_create_info &ci
) : property_type( ci ) {
  LIBGCT_SHADER_SET_USE_SYSTEM_SHADER_IF_NOT_EXIST( "cubemap_matrix_generator" );
  LIBGCT_SHADER_SET_VALIDATE( shader_set, "cubemap_matrix_generator2::cubemap_matrix_generator2", generate_path, "generate" )
  generate.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( generate_path )
      .set_resources( props.resources )
      .set_scene_graph( props.scene_graph )
  ) );
}
void cubemap_matrix_generator2::operator()(
  command_buffer_recorder_t &rec,
  const glm::vec3 &pos,
  float near,
  float far,
  const matrix_pool::matrix_descriptor &camera,
  const matrix_pool::matrix_descriptor &projection
) const {
  generate->set_push_constant( "offset", glm::vec4( pos, 1.0f ) );
  generate->set_push_constant( "far", far );
  generate->set_push_constant( "near", near );
  generate->set_push_constant( "projection", *projection );
  generate->set_push_constant( "camera", *camera );
  (*generate)( rec, 0u, 6u, 1u, 1u );
}

}

