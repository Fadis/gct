#include <gct/shader_module.hpp>
#include <gct/pipeline_shader_stage_create_info.hpp>
#include <vulkan2json/PipelineShaderStageCreateInfo.hpp>
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
#include <vulkan2json/PipelineShaderStageRequiredSubgroupSizeCreateInfoEXT.hpp>
#endif

namespace gct {
  vk::ShaderStageFlagBits spv2vk( SpvReflectShaderStageFlagBits v ) {
    if( v == SPV_REFLECT_SHADER_STAGE_VERTEX_BIT )
      return vk::ShaderStageFlagBits::eVertex;
    else if( v == SPV_REFLECT_SHADER_STAGE_TESSELLATION_CONTROL_BIT )
      return vk::ShaderStageFlagBits::eTessellationControl;
    else if( v == SPV_REFLECT_SHADER_STAGE_TESSELLATION_EVALUATION_BIT )
      return vk::ShaderStageFlagBits::eTessellationEvaluation;
    else if( v == SPV_REFLECT_SHADER_STAGE_GEOMETRY_BIT )
      return vk::ShaderStageFlagBits::eGeometry;
    else if( v == SPV_REFLECT_SHADER_STAGE_FRAGMENT_BIT )
      return vk::ShaderStageFlagBits::eFragment;
    else if( v == SPV_REFLECT_SHADER_STAGE_COMPUTE_BIT )
      return vk::ShaderStageFlagBits::eCompute;
    else if( v == SPV_REFLECT_SHADER_STAGE_TASK_BIT_NV )
      return vk::ShaderStageFlagBits::eTaskNV;
    else if( v == SPV_REFLECT_SHADER_STAGE_MESH_BIT_NV )
      return vk::ShaderStageFlagBits::eMeshNV;
    else if( v == SPV_REFLECT_SHADER_STAGE_RAYGEN_BIT_KHR )
      return vk::ShaderStageFlagBits::eRaygenKHR;
    else if( v == SPV_REFLECT_SHADER_STAGE_ANY_HIT_BIT_KHR )
      return vk::ShaderStageFlagBits::eAnyHitKHR;
    else if( v == SPV_REFLECT_SHADER_STAGE_CLOSEST_HIT_BIT_KHR )
      return vk::ShaderStageFlagBits::eClosestHitKHR;
    else if( v == SPV_REFLECT_SHADER_STAGE_MISS_BIT_KHR )
      return vk::ShaderStageFlagBits::eMissKHR;
    else if( v == SPV_REFLECT_SHADER_STAGE_INTERSECTION_BIT_KHR )
      return vk::ShaderStageFlagBits::eIntersectionKHR;
    else if( v == SPV_REFLECT_SHADER_STAGE_CALLABLE_BIT_KHR )
      return vk::ShaderStageFlagBits::eCallableKHR;
    else
      throw exception::invalid_argument( "unknown SpvReflectShaderStageFlagBits value", __FILE__, __LINE__ );
  }

  void to_json( nlohmann::json &root, const pipeline_shader_stage_create_info_t &v ) {
     root = nlohmann::json::object();
     root[ "basic" ] = v.get_basic();
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( required_subgroup_size ) 
#endif
  }
  void from_json( const nlohmann::json &root, pipeline_shader_stage_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to pipeline_shader_stage_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( required_subgroup_size ) 
#endif
  }
  pipeline_shader_stage_create_info_t &pipeline_shader_stage_create_info_t::rebuild_chain() {
    if( shader_module ) {
      basic.setModule( **shader_module );
      if( shader_module->get_props().has_reflection() ) {
        const auto &reflection = shader_module->get_props().get_reflection();
        basic.setStage( spv2vk( reflection->shader_stage ) );
        basic.setPName( reflection->entry_point_name );
      }
    }
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

