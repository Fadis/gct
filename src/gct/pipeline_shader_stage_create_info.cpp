#include <nlohmann/json.hpp>
#include <gct/shader_module.hpp>
#include <gct/shader_module_reflection.hpp>
#include <gct/pipeline_shader_stage_create_info.hpp>
#include <gct/spv2vk.hpp>
#include <vulkan2json/PipelineShaderStageCreateInfo.hpp>
#ifdef VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#include <vulkan2json/DebugUtilsObjectNameInfoEXT.hpp>
#endif
#ifdef VK_EXT_PIPELINE_ROBUSTNESS_EXTENSION_NAME
#include <vulkan2json/PipelineRobustnessCreateInfoEXT.hpp>
#endif
#ifdef VK_EXT_SHADER_MODULE_IDENTIFIER_EXTENSION_NAME
#include <vulkan2json/PipelineShaderStageModuleIdentifierCreateInfoEXT.hpp>
#endif
#ifdef VK_AMDX_SHADER_ENQUEUE_EXTENSION_NAME
#include <vulkan2json/PipelineShaderStageNodeCreateInfoAMDX.hpp>
#endif
#ifdef VK_VERSION_1_3
#include <vulkan2json/PipelineShaderStageRequiredSubgroupSizeCreateInfo.hpp>
#elif defined(VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME)
#include <vulkan2json/PipelineShaderStageRequiredSubgroupSizeCreateInfoEXT.hpp>
#endif
#ifdef VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
#include <vulkan2json/ShaderModuleValidationCacheCreateInfoEXT.hpp>
#endif

namespace gct {

  void to_json( nlohmann::json &root, const pipeline_shader_stage_create_info_t &v ) {
     root = nlohmann::json::object();
     root[ "basic" ] = v.get_basic();
#ifdef VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( debug_utils_object_name )
#endif
#ifdef VK_EXT_PIPELINE_ROBUSTNESS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( robustness )
#endif
#ifdef VK_EXT_SHADER_MODULE_IDENTIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( module_identifier )
#endif
#ifdef VK_AMDX_SHADER_ENQUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( node )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( required_subgroup_size )
#endif
#ifdef VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( validation_cache )
#endif
  }
  void from_json( const nlohmann::json &root, pipeline_shader_stage_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to pipeline_shader_stage_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( debug_utils_object_name )
#endif
#ifdef VK_EXT_PIPELINE_ROBUSTNESS_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( robustness )
#endif
#ifdef VK_EXT_SHADER_MODULE_IDENTIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( module_identifier )
#endif
#ifdef VK_AMDX_SHADER_ENQUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( node )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( required_subgroup_size )
#endif
#ifdef VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( validation_cache )
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
#ifdef VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( debug_utils_object_name )
#endif
#ifdef VK_EXT_PIPELINE_ROBUSTNESS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( robustness )
#endif
#ifdef VK_EXT_SHADER_MODULE_IDENTIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( module_identifier )
#endif
#ifdef VK_AMDX_SHADER_ENQUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( node )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( required_subgroup_size )
#endif
#ifdef VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( validation_cache )
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

