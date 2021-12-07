#ifndef GCT_PIPELINE_SHADER_STAGE_CREATE_INFO_HPP
#define GCT_PIPELINE_SHADER_STAGE_CREATE_INFO_HPP
#include <memory>
#include <optional>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>
#include <gct/specialization_info.hpp>

namespace gct {
  class shader_module_t;
  class pipeline_shader_stage_create_info_t {
  public:
    using self_type = pipeline_shader_stage_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::PipelineShaderStageCreateInfo )
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineShaderStageRequiredSubgroupSizeCreateInfoEXT , required_subgroup_size )
#endif
  private:
    std::shared_ptr< shader_module_t > shader_module;
    std::shared_ptr< basic_specialization_info_t > spec;
  public:
    pipeline_shader_stage_create_info_t &set_shader_module( const std::shared_ptr< shader_module_t >& );
    pipeline_shader_stage_create_info_t &clear_shader_module();
    pipeline_shader_stage_create_info_t &add_specialization_info( const vk::SpecializationInfo& );
    pipeline_shader_stage_create_info_t &clear_specialization_info();
    template< typename T >
    pipeline_shader_stage_create_info_t &set_specialization_info( const T &v ) {
      spec = std::shared_ptr< T >( new T( v ) );
      chained = false;
      return *this;
    }
  };
  void to_json( nlohmann::json &root, const pipeline_shader_stage_create_info_t &v );
  void from_json( const nlohmann::json &root, pipeline_shader_stage_create_info_t &v );
}

#endif

