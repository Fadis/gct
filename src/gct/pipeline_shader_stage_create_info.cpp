#include <gct/shader_module.hpp>
#include <gct/pipeline_shader_stage_create_info.hpp>

namespace gct {
  pipeline_shader_stage_create_info_t &pipeline_shader_stage_create_info_t::rebuild_chain() {
    if( shader_module )
      basic.setModule( **shader_module );
    if( spec ) {
      spec->rebuild_chain();
      basic.setPSpecializationInfo( &spec->get_basic() );
    }
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( required_subgroup_size ) 
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  pipeline_shader_stage_create_info_t &pipeline_shader_stage_create_info_t::set_shader_module( const std::shared_ptr< shader_module_t > &v ) {
    shader_module = v;
    chained = false;
    return *this;
  }
  pipeline_shader_stage_create_info_t &pipeline_shader_stage_create_info_t::clear_shader_module() {
    shader_module.reset();
    chained = false;
    return *this;
  }
  pipeline_shader_stage_create_info_t &pipeline_shader_stage_create_info_t::clear_specialization_info() {
    spec.reset();
    chained = false;
    return *this;
  }
}

