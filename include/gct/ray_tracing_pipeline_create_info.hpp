#ifndef GCT_RAY_TRACING_PIPELINE_CREATE_INFO_HPP
#define GCT_RAY_TRACING_PIPELINE_CREATE_INFO_HPP
#include <vulkan/vulkan.hpp>
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
#include <memory>
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <gct/deep_copy.hpp>
#include <gct/extension.hpp>
#include <gct/pipeline_shader_stage_create_info.hpp>
#include <gct/ray_tracing_shader_group_create_info.hpp>
#include <gct/pipeline_library_create_info.hpp>
#include <gct/ray_tracing_pipeline_interface_create_info.hpp>
#include <gct/pipeline_dynamic_state_create_info.hpp>
#include <gct/array_of.hpp>
namespace gct {
  class pipeline_layout_t;
  class descriptor_set_layout_t;
  class ray_tracing_pipeline_create_info_t : public chained_t {
  public:
    using self_type = ray_tracing_pipeline_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::RayTracingPipelineCreateInfoKHR )
#ifdef VK_KHR_MAINTENANCE_5_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineCreateFlags2CreateInfoKHR , create_flags2 )
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
  private:
    std::vector< pipeline_shader_stage_create_info_t > stage;
    std::vector< vk::PipelineShaderStageCreateInfo > raw_stage;
    std::vector< ray_tracing_shader_group_create_info_t > group;
    std::vector< vk::RayTracingShaderGroupCreateInfoKHR > raw_group;
    deep_copy_unique_ptr< pipeline_library_create_info_t > library;
    deep_copy_unique_ptr< ray_tracing_pipeline_interface_create_info_t > library_interface;
    deep_copy_unique_ptr< pipeline_dynamic_state_create_info_t > dynamic;
    std::shared_ptr< pipeline_layout_t > layout;
  public:
    ray_tracing_pipeline_create_info_t &add_stage( const pipeline_shader_stage_create_info_t&, const ray_tracing_shader_group_create_info_t& );
    ray_tracing_pipeline_create_info_t &add_stage( const pipeline_shader_stage_create_info_t&, vk::RayTracingShaderGroupTypeKHR );
    ray_tracing_pipeline_create_info_t &add_stage( const std::shared_ptr< shader_module_t >&, vk::RayTracingShaderGroupTypeKHR );
    ray_tracing_pipeline_create_info_t &add_stage( const pipeline_shader_stage_create_info_t& );
    ray_tracing_pipeline_create_info_t &add_stage( const std::shared_ptr< shader_module_t >& );
    ray_tracing_pipeline_create_info_t &clear_stage();
    [[nodiscard]] std::vector< pipeline_shader_stage_create_info_t > get_stage() const { return stage; }
    [[nodiscard]] std::vector< ray_tracing_shader_group_create_info_t > get_group() const { return group; }
    ray_tracing_pipeline_create_info_t &set_library( const pipeline_library_create_info_t& );
    ray_tracing_pipeline_create_info_t &clear_library();
    ray_tracing_pipeline_create_info_t &set_library_interface( const ray_tracing_pipeline_interface_create_info_t& );
    ray_tracing_pipeline_create_info_t &clear_library_interface();
    ray_tracing_pipeline_create_info_t &set_dynamic( const pipeline_dynamic_state_create_info_t& );
    ray_tracing_pipeline_create_info_t &clear_dynamic();
    ray_tracing_pipeline_create_info_t &set_layout( const std::shared_ptr< pipeline_layout_t >& );
    ray_tracing_pipeline_create_info_t &set_layout( const std::shared_ptr< descriptor_set_layout_t >& );
    ray_tracing_pipeline_create_info_t &clear_layout();
    [[nodiscard]] const std::shared_ptr< pipeline_layout_t > &get_layout() const {
      return layout;
    }
  };
  void to_json( nlohmann::json &root, const ray_tracing_pipeline_create_info_t &v );
  void from_json( const nlohmann::json &root, ray_tracing_pipeline_create_info_t &v );
}

#endif

#endif
