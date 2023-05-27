#ifndef GCT_GRAPHICS_PIPELINE_CREATE_INFO_HPP
#define GCT_GRAPHICS_PIPELINE_CREATE_INFO_HPP
#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>
#include <gct/pipeline_shader_stage_create_info.hpp>
#include <gct/pipeline_vertex_input_state_create_info.hpp>
#include <gct/pipeline_input_assembly_state_create_info.hpp>
#include <gct/pipeline_tessellation_state_create_info.hpp>
#include <gct/pipeline_viewport_state_create_info.hpp>
#include <gct/pipeline_rasterization_state_create_info.hpp>
#include <gct/pipeline_multisample_state_create_info.hpp>
#include <gct/pipeline_depth_stencil_state_create_info.hpp>
#include <gct/pipeline_color_blend_state_create_info.hpp>
#include <gct/pipeline_dynamic_state_create_info.hpp>

namespace gct {
  class pipeline_layout_t;
  class render_pass_t;
  class descriptor_set_layout_t;
  class graphics_pipeline_create_info_t : public chained_t {
    friend void to_json( nlohmann::json &root, const graphics_pipeline_create_info_t &v );
  public:
    using self_type = graphics_pipeline_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::GraphicsPipelineCreateInfo )
#ifdef VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::AttachmentSampleCountInfoAMD , attachment_sample_count )
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::GraphicsPipelineShaderGroupsCreateInfoNV , shader_group )
#endif
#if defined(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME) && defined(VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::MultiviewPerViewAttributesInfoNVX , multiview_per_view_attributes )
#endif
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineCreationFeedbackCreateInfoEXT, creation_feedback )
#endif
#ifdef VK_AMD_PIPELINE_COMPILER_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineCompilerControlCreateInfoAMD , compiler_control )
#endif
#ifdef VK_EXT_DISCARD_RECTANGLES_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineDiscardRectangleStateCreateInfoEXT , discard_rectangle )
#endif
#ifdef VK_NV_FRAGMENT_SHADING_RATE_ENUMS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineFragmentShadingRateEnumStateCreateInfoNV , fragment_shading_rate_nv )
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineFragmentShadingRateStateCreateInfoKHR, fragment_shading_rate )
#endif
#ifdef VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineRenderingCreateInfoKHR, rendering )
#endif
#ifdef VK_NV_REPRESENTATIVE_FRAGMENT_TEST_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineRepresentativeFragmentTestStateCreateInfoNV, representative_fragment_test )
#endif 
  private:
    std::vector< pipeline_shader_stage_create_info_t > stage;
    std::vector< vk::PipelineShaderStageCreateInfo > raw_stage;
    deep_copy_unique_ptr< pipeline_vertex_input_state_create_info_t > vertex_input;
    deep_copy_unique_ptr< pipeline_input_assembly_state_create_info_t > input_assembly;
    deep_copy_unique_ptr< pipeline_tessellation_state_create_info_t > tessellation;
    deep_copy_unique_ptr< pipeline_viewport_state_create_info_t > viewport;
    deep_copy_unique_ptr< pipeline_rasterization_state_create_info_t > rasterization;
    deep_copy_unique_ptr< pipeline_multisample_state_create_info_t > multisample;
    deep_copy_unique_ptr< pipeline_depth_stencil_state_create_info_t > depth_stencil;
    deep_copy_unique_ptr< pipeline_color_blend_state_create_info_t > color_blend;
    deep_copy_unique_ptr< pipeline_dynamic_state_create_info_t > dynamic;
    std::shared_ptr< pipeline_layout_t > layout;
    std::shared_ptr< render_pass_t > render_pass;
    std::uint32_t subpass = 0u;
  public:
    graphics_pipeline_create_info_t &add_stage( const pipeline_shader_stage_create_info_t& );
    graphics_pipeline_create_info_t &add_stage( const std::shared_ptr< shader_module_t >& );
    graphics_pipeline_create_info_t &clear_stage();
    graphics_pipeline_create_info_t &set_vertex_input( const pipeline_vertex_input_state_create_info_t& );
    graphics_pipeline_create_info_t &clear_vertex_input();
    graphics_pipeline_create_info_t &set_input_assembly( const pipeline_input_assembly_state_create_info_t& );
    graphics_pipeline_create_info_t &clear_input_assembly();
    graphics_pipeline_create_info_t &set_tessellation( const pipeline_tessellation_state_create_info_t& );
    graphics_pipeline_create_info_t &clear_tessellation();
    graphics_pipeline_create_info_t &set_viewport( const pipeline_viewport_state_create_info_t& );
    graphics_pipeline_create_info_t &clear_viewport();
    graphics_pipeline_create_info_t &set_rasterization( const pipeline_rasterization_state_create_info_t& );
    graphics_pipeline_create_info_t &clear_rasterization();
    graphics_pipeline_create_info_t &set_multisample( const pipeline_multisample_state_create_info_t& );
    graphics_pipeline_create_info_t &clear_multisample();
    graphics_pipeline_create_info_t &set_depth_stencil( const pipeline_depth_stencil_state_create_info_t& );
    graphics_pipeline_create_info_t &clear_depth_stencil();
    graphics_pipeline_create_info_t &set_color_blend( const pipeline_color_blend_state_create_info_t& );
    graphics_pipeline_create_info_t &clear_color_blend();
    graphics_pipeline_create_info_t &set_dynamic( const pipeline_dynamic_state_create_info_t& );
    graphics_pipeline_create_info_t &clear_dynamic();
    graphics_pipeline_create_info_t &set_layout( const std::shared_ptr< pipeline_layout_t >& );
    graphics_pipeline_create_info_t &set_layout( const std::shared_ptr< descriptor_set_layout_t >& );
    graphics_pipeline_create_info_t &clear_layout();
    const std::shared_ptr< pipeline_layout_t > &get_layout() const {
      return layout;
    }
    graphics_pipeline_create_info_t &set_render_pass( const std::shared_ptr< render_pass_t >&, std::uint32_t );
    graphics_pipeline_create_info_t &clear_render_pass();
    void to_json( nlohmann::json &root );
  };
  void to_json( nlohmann::json &root, const graphics_pipeline_create_info_t &v );
}

#endif

