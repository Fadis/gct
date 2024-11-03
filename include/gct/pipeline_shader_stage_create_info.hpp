#ifndef GCT_PIPELINE_SHADER_STAGE_CREATE_INFO_HPP
#define GCT_PIPELINE_SHADER_STAGE_CREATE_INFO_HPP
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>
#include <gct/specialization_info.hpp>

namespace gct {
  class shader_module_t;
  class pipeline_shader_stage_create_info_t : public chained_t {
  public:
    using self_type = pipeline_shader_stage_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::PipelineShaderStageCreateInfo )
#ifdef VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::DebugUtilsObjectNameInfoEXT , debug_utils_object_name )
#endif
#ifdef VK_EXT_PIPELINE_ROBUSTNESS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineRobustnessCreateInfoEXT , robustness )
#endif
#ifdef VK_EXT_SHADER_MODULE_IDENTIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineShaderStageModuleIdentifierCreateInfoEXT , module_identifier )
#endif
#ifdef VK_AMDX_SHADER_ENQUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineShaderStageNodeCreateInfoAMDX , node )
#endif
#ifdef VK_VERSION_1_3
    LIBGCT_EXTENSION_SETTER( vk::PipelineShaderStageRequiredSubgroupSizeCreateInfo , required_subgroup_size )
#elif defined(VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PipelineShaderStageRequiredSubgroupSizeCreateInfoEXT , required_subgroup_size )
#endif
#ifdef VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::ShaderModuleValidationCacheCreateInfoEXT , validation_cache )
#endif
  private:
    std::shared_ptr< shader_module_t > shader_module;
    std::shared_ptr< basic_specialization_info_t > spec;
  public:
    pipeline_shader_stage_create_info_t &set_shader_module( const std::shared_ptr< shader_module_t >& );
    pipeline_shader_stage_create_info_t &clear_shader_module();
    const std::shared_ptr< shader_module_t > get_shader_module() const {
      return shader_module;
    }
    pipeline_shader_stage_create_info_t &add_specialization_info( const vk::SpecializationInfo& );
    pipeline_shader_stage_create_info_t &clear_specialization_info();
    template< typename T >
    pipeline_shader_stage_create_info_t &set_specialization_info( const T &v ) {
      if( !v.empty() ) {
        spec = std::shared_ptr< T >( new T( v ) );
      }
      else {
        spec.reset();
      }
      chained = false;
      return *this;
    }
  };
  void to_json( nlohmann::json &root, const pipeline_shader_stage_create_info_t &v );
  void from_json( const nlohmann::json &root, pipeline_shader_stage_create_info_t &v );
}

#endif

