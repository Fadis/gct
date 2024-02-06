#ifndef GCT_COMPUTE_PIPELINE_CREATE_INFO_HPP
#define GCT_COMPUTE_PIPELINE_CREATE_INFO_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>
#include <gct/pipeline_shader_stage_create_info.hpp>
#include <gct/array_of.hpp>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace gct {
  class pipeline_layout_t;
  class descriptor_set_layout_t;
  class compute_pipeline_create_info_t : public chained_t {
  public:
    using self_type = compute_pipeline_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::ComputePipelineCreateInfo )
#ifdef VK_AMD_PIPELINE_COMPILER_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineCompilerControlCreateInfoAMD , compiler_control )
#endif
#ifdef VK_KHR_MAINTENANCE_5_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineCreateFlags2CreateInfoKHR , flags2 )
#endif
#ifdef VK_VERSION_1_3
    LIBGCT_EXTENSION_SETTER( vk::PipelineCreationFeedbackCreateInfo, creation_feedback )
    LIBGCT_ARRAY_OF( vk::PipelineCreationFeedback, stage_creation_feedback )
#elif defined(VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PipelineCreationFeedbackCreateInfoEXT, creation_feedback )
    LIBGCT_ARRAY_OF( vk::PipelineCreationFeedbackEXT, stage_creation_feedback )
#endif
#ifdef VK_EXT_PIPELINE_ROBUSTNESS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineRobustnessCreateInfoEXT , robustness )
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
    const pipeline_shader_stage_create_info_t &get_stage() const {
      return stage;
    }
    compute_pipeline_create_info_t &set_layout( const std::shared_ptr< pipeline_layout_t >& );
    compute_pipeline_create_info_t &set_layout( const std::shared_ptr< descriptor_set_layout_t >& );
    compute_pipeline_create_info_t &set_layout(
      const std::shared_ptr< descriptor_set_layout_t >&,
      const std::shared_ptr< shader_module_t >&
    );
    compute_pipeline_create_info_t &clear_layout();
    const std::shared_ptr< pipeline_layout_t > &get_layout() const {
      return layout;
    }
  };
  void to_json( nlohmann::json &root, const compute_pipeline_create_info_t &v );
  void from_json( const nlohmann::json &root, compute_pipeline_create_info_t &v );
}

#endif

