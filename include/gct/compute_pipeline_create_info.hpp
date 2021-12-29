#ifndef GCT_COMPUTE_PIPELINE_CREATE_INFO_HPP
#define GCT_COMPUTE_PIPELINE_CREATE_INFO_HPP
#include <memory>
#include <optional>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>
#include <gct/pipeline_shader_stage_create_info.hpp>

namespace gct {
  class pipeline_layout_t;
  class compute_pipeline_create_info_t : public chained_t {
  public:
    using self_type = compute_pipeline_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::ComputePipelineCreateInfo )
#ifdef VK_AMD_PIPELINE_COMPILER_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineCompilerControlCreateInfoAMD , compiler_control )
#endif
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineCreationFeedbackCreateInfoEXT , creation_feedback )
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::SubpassShadingPipelineCreateInfoHUAWEI , subpass_shading )
#endif
  private:
    pipeline_shader_stage_create_info_t stage;
    std::shared_ptr< pipeline_layout_t > layout;
  public:
    compute_pipeline_create_info_t &set_stage( const pipeline_shader_stage_create_info_t& );
    compute_pipeline_create_info_t &set_stage( const std::shared_ptr< shader_module_t >& );
    compute_pipeline_create_info_t &set_layout( const std::shared_ptr< pipeline_layout_t >& );
    compute_pipeline_create_info_t &clear_layout();
  };
  void to_json( nlohmann::json &root, const compute_pipeline_create_info_t &v );
  void from_json( const nlohmann::json &root, compute_pipeline_create_info_t &v );
}

#endif

