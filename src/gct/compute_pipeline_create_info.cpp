#include <fstream>
#include <iterator>
#include <gct/pipeline_layout.hpp>
#include <gct/compute_pipeline_create_info.hpp>
#include <gct/shader_module.hpp>
#include <vulkan2json/ComputePipelineCreateInfo.hpp>
#ifdef VK_AMD_PIPELINE_COMPILER_CONTROL_EXTENSION_NAME
#include <vulkan2json/PipelineCompilerControlCreateInfoAMD.hpp>
#endif
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
#include <vulkan2json/PipelineCreationFeedbackCreateInfoEXT.hpp>
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
#include <vulkan2json/SubpassShadingPipelineCreateInfoHUAWEI.hpp>
#endif


namespace gct {
  void to_json( nlohmann::json &root, const compute_pipeline_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
#ifdef VK_AMD_PIPELINE_COMPILER_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( compiler_control ) 
#endif
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( creation_feedback ) 
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( subpass_shading ) 
#endif
  }
  void from_json( const nlohmann::json &root, compute_pipeline_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to compute_pipeline_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_AMD_PIPELINE_COMPILER_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( compiler_control ) 
#endif
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( creation_feedback ) 
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( subpass_shading ) 
#endif
  }
  compute_pipeline_create_info_t &compute_pipeline_create_info_t::rebuild_chain() {
    if( chained ) return *this;
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
  compute_pipeline_create_info_t &compute_pipeline_create_info_t::set_stage( const std::shared_ptr< shader_module_t > &v ) {
    if( !v->get_props().has_reflection() )
      throw exception::invalid_argument( "Reflection is required to set shader directly to pipeline", __FILE__, __LINE__ );
    set_stage(
      pipeline_shader_stage_create_info_t()
        .set_shader_module( v )
        .rebuild_chain()
    );
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

