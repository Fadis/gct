#include <fstream>
#include <iterator>
#include <nlohmann/json.hpp>
#include <gct/device.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/compute_pipeline_create_info.hpp>
#include <gct/shader_module.hpp>
#include <gct/get_device.hpp>
#include <gct/specialization_map.hpp>
#include <gct/shader_module_reflection.hpp>
#include <vulkan2json/ComputePipelineCreateInfo.hpp>
#ifdef VK_AMD_PIPELINE_COMPILER_CONTROL_EXTENSION_NAME
#include <vulkan2json/PipelineCompilerControlCreateInfoAMD.hpp>
#endif
#ifdef VK_KHR_MAINTENANCE_5_EXTENSION_NAME
#include <vulkan2json/PipelineCreateFlags2CreateInfoKHR.hpp>
#endif
#ifdef VK_VERSION_1_3
#include <vulkan2json/PipelineCreationFeedbackCreateInfo.hpp>
#include <vulkan2json/PipelineCreationFeedback.hpp>
#elif defined(VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME)
#include <vulkan2json/PipelineCreationFeedbackCreateInfoEXT.hpp>
#include <vulkan2json/PipelineCreationFeedbackEXT.hpp>
#endif
#ifdef VK_EXT_PIPELINE_ROBUSTNESS_EXTENSION_NAME
#include <vulkan2json/PipelineRobustnessCreateInfoEXT.hpp>
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
#ifdef VK_KHR_MAINTENANCE_5_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( flags2 )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( creation_feedback ) 
    LIBGCT_ARRAY_OF_TO_JSON( creation_feedback, pPipelineStageCreationFeedbacks, stage_creation_feedback )
#endif
#ifdef VK_EXT_PIPELINE_ROBUSTNESS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( robustness )
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
#ifdef VK_KHR_MAINTENANCE_5_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( flags2 )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( creation_feedback ) 
    LIBGCT_ARRAY_OF_FROM_JSON( creation_feedback, pPipelineStageCreationFeedbacks, stage_creation_feedback )
#endif
#ifdef VK_EXT_PIPELINE_ROBUSTNESS_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( robustness )
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
#ifdef VK_KHR_MAINTENANCE_5_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( flags2 )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME)
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( creation_feedback, PipelineStageCreationFeedbackCount, PPipelineStageCreationFeedbacks, stage_creation_feedback )
    LIBGCT_EXTENSION_REBUILD_CHAIN( creation_feedback ) 
#endif
#ifdef VK_EXT_PIPELINE_ROBUSTNESS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( robustness )
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
    const SpvReflectEntryPoint *entry_point = v->get_props().get_reflection()->entry_points;
    if( entry_point ) {
      if(
        entry_point->local_size.x != 0 &&
        entry_point->local_size.y != 0 &&
        entry_point->local_size.z != 0
      ) {
        set_dim( glm::ivec3(
          entry_point->local_size.x,
          entry_point->local_size.y,
          entry_point->local_size.z
        ) );
      }
    }
    return *this;
  }
  compute_pipeline_create_info_t &compute_pipeline_create_info_t::set_stage( const std::shared_ptr< shader_module_t > &v, const specialization_map &m ) {
    if( !v->get_props().has_reflection() )
      throw exception::invalid_argument( "Reflection is required to set shader directly to pipeline", __FILE__, __LINE__ );
    set_stage(
      pipeline_shader_stage_create_info_t()
        .set_shader_module( v )
        .set_specialization_info(
          encode( m )
        )
        .rebuild_chain()
    );
    const SpvReflectEntryPoint *entry_point = v->get_props().get_reflection()->entry_points;
    if( entry_point ) {
      if(
        entry_point->local_size.x != 0 &&
        entry_point->local_size.y != 0 &&
        entry_point->local_size.z != 0
      ) {
        set_dim( glm::ivec3(
          entry_point->local_size.x,
          entry_point->local_size.y,
          entry_point->local_size.z
        ) );
      }
    }
    return *this;
  }
  compute_pipeline_create_info_t &compute_pipeline_create_info_t::set_layout( const std::shared_ptr< pipeline_layout_t > &v ) {
    layout = v;
    chained = false;
    return *this;
  }
  compute_pipeline_create_info_t &compute_pipeline_create_info_t::set_layout( const std::shared_ptr< descriptor_set_layout_t > &v ) {
    return set_layout(
      get_device( *v ).get_pipeline_layout(
        gct::pipeline_layout_create_info_t()
          .add_descriptor_set_layout(
            v
          )
      )
    );
  }
  compute_pipeline_create_info_t &compute_pipeline_create_info_t::set_layout(
    const std::shared_ptr< descriptor_set_layout_t > &l,
    const std::shared_ptr< shader_module_t > &m
  ) {
    return set_layout(
      get_device( *l ).get_pipeline_layout(
        gct::pipeline_layout_create_info_t()
          .add_descriptor_set_layout(
            l
          )
          .add_push_constant_range(
            m
          )
      )
    );
  }
  compute_pipeline_create_info_t &compute_pipeline_create_info_t::set_layout(
    const std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_layout_t > > &l,
    const std::shared_ptr< shader_module_t > &m
  ) {
    gct::pipeline_layout_create_info_t ci;
    for( const auto &e: l ) {
      ci.add_descriptor_set_layout( e.first, e.second );
    }
    ci.add_push_constant_range( m );
    return set_layout(
      get_device( *m ).get_pipeline_layout(
        std::move( ci )
      )
    );
  }
  compute_pipeline_create_info_t &compute_pipeline_create_info_t::clear_layout() {
    layout.reset();
    chained = false;
    return *this;
  }
}

