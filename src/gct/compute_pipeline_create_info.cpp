#include <fstream>
#include <iterator>
#include <gct/pipeline_layout.hpp>
#include <gct/compute_pipeline_create_info.hpp>

namespace gct {
  compute_pipeline_create_info_t &compute_pipeline_create_info_t::rebuild_chain() {
    stage.rebuild_chain();
    basic.setStage( stage.get_basic() );
    if( layout ) {
      basic.setLayout( **layout );
    }

    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_AMD_PIPELINE_COMPILER_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( compiler_control ) 
#endif
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( creation_feedback ) 
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( subpass_shading ) 
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  compute_pipeline_create_info_t &compute_pipeline_create_info_t::set_stage( const pipeline_shader_stage_create_info_t &v ) {
    stage = v;
    chained = false;
    return *this;
  }
  compute_pipeline_create_info_t &compute_pipeline_create_info_t::set_layout( const std::shared_ptr< pipeline_layout_t > &v ) {
    layout = v;
    chained = false;
    return *this;
  }
  compute_pipeline_create_info_t &compute_pipeline_create_info_t::clear_layout() {
    layout.reset();
    chained = false;
    return *this;
  }
}

